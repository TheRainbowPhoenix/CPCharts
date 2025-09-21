#include "../include/peg.hpp"
#include <string.h>

// Constructor for the base screen
PegScreen::PegScreen(const PegRect &Rect) {
  mwDrawNesting = 0;
  mwHRes = Rect.Width();
  mwVRes = Rect.Height();
  miInvalidCount = 0;
  mwTotalViewports = 0;
  mpScanPointers = nullptr;

#ifdef PEG_FULL_CLIPPING
  mpFreeListStart = nullptr;
  mpFreeListEnd = nullptr;
  AllocateViewportBlock();
#endif
}

PegScreen::~PegScreen() {
  if (mpScanPointers) {
    // The derived class (L16Screen) is responsible for the actual video buffer
    // but the array of pointers is owned by the base class.
    delete[] mpScanPointers;
  }
}

void PegScreen::Invalidate(const PegRect &Rect) {
  LOCK_PEG
  if (miInvalidCount) {
    mInvalid |= Rect;
  } else {
    mInvalid = Rect;
  }
  miInvalidCount++;
}

// Get the height of a font
SIGNED PegScreen::TextHeight(const TCHAR *Text, PegFont *Font) {
  if (!Font)
    return 0;
  return Font->uHeight;
}

// Get the width of a string of text
SIGNED PegScreen::TextWidth(const TCHAR *Text, PegFont *Font, SIGNED iLen) {
  SIGNED iWidth = 0;
  if (!Text || !Font)
    return 0;

  WORD cVal;
  PegFont *pFont;

  /*
      if (!IS_VARWIDTH(Font))     // fixed width font? Not supported...
      {
          WORD wCharWidth = (WORD) Font->pOffsets;
          cVal = *Text++;
          while(cVal && iLen--)
          {
              iWidth += wCharWidth;
              cVal = *Text++;
          }
          return iWidth;
      }
  */

  cVal = *Text++;

  while (cVal && iLen--) {
    pFont = Font;
    while (pFont) {
      if (cVal >= pFont->wFirstChar && cVal <= pFont->wLastChar) {
        break;
      }
      pFont = pFont->pNext;
    }

    if (!pFont) {
      cVal = *Text++;
      continue;
    }

    WORD *pOffset = pFont->pOffsets;
    WORD wOffset = cVal - pFont->wFirstChar;
    iWidth += *(pOffset + wOffset + 1) - *(pOffset + wOffset);
    cVal = *Text++;
  }
  return iWidth;
}

// High-level DrawText that handles clipping and then calls the driver-specific
// DrawTextView
void PegScreen::DrawText(PegThing *Caller, PegPoint Where, const TCHAR *Text,
                         PegColor &Color, PegFont *Font, SIGNED iCount) {
  if (!Text || !*Text)
    return;

  PegRect Rect;
  Rect.wLeft = Where.x;
  Rect.wTop = Where.y;
  Rect.wRight = Rect.wLeft + TextWidth(Text, Font) - 1;
  Rect.wBottom = Rect.wTop + TextHeight(Text, Font) - 1;

  if (!ClipRect(Rect, Caller->mClip)) {
    return;
  }

#ifdef PEG_FULL_CLIPPING
  // Find the parent that owns the viewports
  while (!Caller->StatusIs(PSF_VIEWPORT)) {
    Caller = Caller->Parent();
  }

  Viewport *pView = Caller->ViewportList();
  PegRect tRect;

  while (pView) {
    if (pView->mView.Overlap(Rect)) {
      tRect = Rect & pView->mView;
      // Call the low-level driver function for each overlapping viewport
      DrawTextView(Where, Text, Color, Font, iCount, tRect);
    }
    pView = pView->pNext;
  }
#else

  DrawTextView(Where, Text, Color, Font, iCount, Rect);
#endif
}

#ifdef PEG_FULL_CLIPPING

void PegScreen::GenerateViewportList(PegThing *pStart) {
  while (!pStart->StatusIs(PSF_VIEWPORT)) {
    pStart = pStart->Parent();
  }
  FreeViewports(pStart);
  AddViewport(pStart, pStart->mReal);
}

Viewport *PegScreen::GetFreeViewport(void) {
  Viewport *pNew;
  if (!mpFreeListStart) {
    AllocateViewportBlock();
  }
  pNew = mpFreeListStart;
  mpFreeListStart = pNew->pNext;
  return pNew;
}

void PegScreen::AllocateViewportBlock() {
  mpFreeListStart = new Viewport[VIEWPORT_LIST_INCREMENT];
  mpFreeListEnd = mpFreeListStart;
  for (SIGNED i = 0; i < VIEWPORT_LIST_INCREMENT - 1; i++) {
    mpFreeListEnd->pNext = mpFreeListEnd + 1;
    mpFreeListEnd++;
  }
  mpFreeListEnd->pNext = NULL;
  mwTotalViewports += VIEWPORT_LIST_INCREMENT;
}

void PegScreen::FreeViewports(PegThing *pCaller) {
  Viewport *pStart = pCaller->mpViewportList;

  if (pStart) {
    if (!mpFreeListStart) {
      mpFreeListStart = pStart;
    } else {
      mpFreeListEnd->pNext = pStart;
    }
    mpFreeListEnd = pStart;
    while (mpFreeListEnd->pNext) {
      mpFreeListEnd = mpFreeListEnd->pNext;
    }
  }
  pCaller->mpViewportList = NULL;

  PegThing *pChild = pCaller->First();

  while (pChild) {
    if (pChild->StatusIs(PSF_VIEWPORT)) {
      FreeViewports(pChild);
    }
    pChild = pChild->Next();
  }
}

void PegScreen::AddViewport(PegThing *pTarget, PegRect NewView) {
  PegThing *Child;

  // This loop checks to see if any sibling objects that also have
  // VIEWPORT status are on top of this viewport:

  if (pTarget->Parent()) {
    Child = pTarget->Parent()->First();

    while (Child && Child != pTarget) {
      if (Child->StatusIs(PSF_VIEWPORT)) {
        if (Child->mReal.Contains(NewView)) {
          // In this case, a sibling window is completely
          // covering the target window, just return;
          return;
        }
        if (Child->mReal.Overlap(NewView)) {
          SplitView(pTarget, Child->mReal, NewView);
          return;
        }
      }
      Child = Child->Next();
    }
  }

  // This loop checks to see if any children of the current object
  // have viewport status. If they do, and they overlap the current
  // viewport, the viewport has to be split up:

  Child = pTarget->First();
  while (Child) {
    if (Child->StatusIs(PSF_VIEWPORT)) {
      if (Child->mClip.Overlap(NewView)) {
        SplitView(pTarget, Child, NewView);
        return;
      }
    }
    Child = Child->Next();
  }

  // we made it through the list, add this guy in:

  Viewport *pNew = GetFreeViewport();
  pNew->mView = NewView;

  if (pTarget->mpViewportList) {
    pNew->pNext = pTarget->mpViewportList;
  } else {
    pNew->pNext = NULL;
  }
  pTarget->mpViewportList = pNew;
}

// Dummy stubs for the complex splitting logic
void PegScreen::SplitView(PegThing *pTarget, PegThing *Child, PegRect Under) {}
void PegScreen::SplitView(PegThing *pTarget, PegRect OnTop, PegRect Under) {}

#endif