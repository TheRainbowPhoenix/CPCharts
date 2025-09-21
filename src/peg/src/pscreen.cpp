#include "../include/peg.hpp"
#include <string.h>

// Constructor for the base screen
PegScreen::PegScreen(const PegRect &Rect) {
  mwHRes = Rect.Width();
  mwVRes = Rect.Height();
  mwDrawNesting = 0;
  mpScanPointers = nullptr;
}

PegScreen::~PegScreen() {
  if (mpScanPointers) {
    // The derived class (L16Screen) is responsible for the actual video buffer
    // but the array of pointers is owned by the base class.
    delete[] mpScanPointers;
  }
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

  // Call the pure virtual function that L16Screen will implement
  DrawTextView(Where, Text, Color, Font, iCount, Rect);
}