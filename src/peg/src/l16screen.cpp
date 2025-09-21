#include "../include/peg.hpp"
#include <sdk/calc/calc.hpp>
#include <sdk/os/lcd.hpp>

int getCharPixelWidth_int(TCHAR ch, const PegFont *pFont) {
  if (!pFont || ch < pFont->wFirstChar || ch >= pFont->wLastChar) {
    return 0;
  }
  int charIndex = ch - pFont->wFirstChar;

  // Width is the difference between the next char's offset and this char's
  // offset.
  int width = pFont->pOffsets[charIndex + 1] - pFont->pOffsets[charIndex];
  return width;
}

// The global factory function that creates an instance of our screen driver.
PegScreen *CreatePegScreen(void) {
  PegRect rect;
  rect.Set(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
  return new L16Screen(rect);
}

// L16Screen Constructor
L16Screen::L16Screen(const PegRect &Rect) : PegScreen(Rect) {
  // For a real RGB565 screen, there's no palette to set up.
  // mpScanPointers will point directly to the hardware buffer if available,
  // or a memory buffer. For now, we don't need to allocate it.
}

L16Screen::~L16Screen() {
  // Clean up any allocated memory
}

// --- Stubbed out functions (we will implement these later) ---

void L16Screen::BeginDraw(PegThing *) { mwDrawNesting++; }
void L16Screen::EndDraw() {
  mwDrawNesting--;
  if (!mwDrawNesting)
    LCD_Refresh();
}
void L16Screen::SetPointerType(UCHAR bType) { (void)bType; } // Do nothing
void L16Screen::SetPalette(SIGNED iFirst, SIGNED iNum, const UCHAR *pGet) {
  (void)iFirst;
  (void)iNum;
  (void)pGet;
} // Do nothing for RGB565
void L16Screen::LineView(SIGNED, SIGNED, SIGNED, SIGNED, PegRect &, PegColor,
                         SIGNED) {}
void L16Screen::BitmapView(const PegPoint, const PegBitmap *, const PegRect &) {
}
void L16Screen::RectMoveView(PegThing *, const PegRect &, const SIGNED,
                             const SIGNED) {}
void L16Screen::HorizontalLine(SIGNED, SIGNED, SIGNED, COLORVAL, SIGNED) {}
void L16Screen::VerticalLine(SIGNED, SIGNED, SIGNED, COLORVAL, SIGNED) {}
COLORVAL L16Screen::GetPixelView(SIGNED x, SIGNED y) { return 0; }

// --- The IMPORTANT function: DrawTextView for our 1bpp font ---
void L16Screen::DrawTextView(PegPoint Where, const TCHAR *Text, PegColor &Color,
                             PegFont *pFont, SIGNED iCount, PegRect &Rect) {
  PegPoint currentPos = Where;
  const UCHAR *pt = (const UCHAR *)Text;
  WORD cVal = *pt++;

  while (cVal && iCount--) {
    int width = getCharPixelWidth_int(cVal, pFont);
    if (width <= 0) {
      cVal = *pt++;
      continue;
    }

    int startBitOffset = pFont->pOffsets[cVal - pFont->wFirstChar];

    for (int y = 0; y < pFont->uHeight; y++) {
      int startBitOfScanline = startBitOffset + (y * pFont->wBytesPerLine * 8);

      for (int x = 0; x < width; x++) {
        int currentBit = startBitOfScanline + x;
        int byteIndex = currentBit / 8;
        int bitInByte = 7 - (currentBit % 8);

        if ((pFont->pData[byteIndex] >> bitInByte) & 1) {
          // Check if pixel is within the clipped rectangle
          if (currentPos.x + x >= Rect.wLeft &&
              currentPos.x + x <= Rect.wRight &&
              currentPos.y + y >= Rect.wTop &&
              currentPos.y + y <= Rect.wBottom) {

            setPixel(currentPos.x + x, currentPos.y + y, Color.uForeground);
          }
        }
      }
    }
    currentPos.x += width;
    cVal = *pt++;
  }
}
