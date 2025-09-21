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

  const UCHAR *pt = (const UCHAR *)Text;
  WORD cVal = *pt++;
  WORD *pOffset = Font->pOffsets;
  WORD wStart = Font->wFirstChar;

  while (cVal && iLen--) {
    if (cVal >= Font->wFirstChar && cVal <= Font->wLastChar) {
      WORD wOffset = cVal - wStart;
      iWidth += *(pOffset + wOffset + 1) - *(pOffset + wOffset);
    }
    cVal = *pt++;
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