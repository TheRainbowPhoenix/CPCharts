#include "../include/peg.hpp"
#include <sdk/calc/calc.hpp>
#include <sdk/os/lcd.hpp>
#include <sdk/os/mem.hpp>

// The global factory function that creates an instance of our screen driver.
PegScreen *CreatePegScreen(void) {
  PegRect rect;
  rect.Set(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
  return new L16Screen(rect);
}

// L16Screen Constructor
L16Screen::L16Screen(const PegRect &Rect) : PegScreen(Rect) {
  mwHRes = Rect.Width();
  mwVRes = Rect.Height();
  // Allocate an array of pointers, one for each scanline
  /* mpScanPointers = (WORD **)LCD_GetVRAMAddress();

  // Get the base address of our software buffer
  WORD PEGFAR *CurrentPtr = GetVideoAddress();

  // Initialize each pointer in the array to the start of a scanline
  for (SIGNED iLoop = 0; iLoop < mwVRes; iLoop++) {
    mpScanPointers[iLoop] = CurrentPtr;
    CurrentPtr += mwHRes; // Pitch is the screen width for 16bpp
  }
  */

  mwDrawNesting = 0;
  ConfigureController();
}

L16Screen::~L16Screen() {
  // Clean up any allocated memory
  // delete[] mpScanPointers;
}

inline void PlotPointView(int x, int y, uint16_t color) {
  if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
    *(FRAME_BUFFER_BASE + (y * FRAME_BUFFER_PITCH) + x) = color;
  }
}

WORD *L16Screen::GetVideoAddress(void) { return LCD_GetVRAMAddress(); }

void L16Screen::ConfigureController(void) {
  LCD_ClearScreen();
  // memset(FRAME_BUFFER_BASE, 0, SCREEN_WIDTH * SCREEN_HEIGHT * 2);
}

void L16Screen::MemoryToScreen(void) { LCD_Refresh(); }

// --- Stubbed out functions (we will implement these later) ---

void L16Screen::BeginDraw(PegThing *) { mwDrawNesting++; }
void L16Screen::EndDraw() {
  mwDrawNesting--;
  if (!mwDrawNesting) {
    MemoryToScreen();
  }
}
void L16Screen::SetPointerType(UCHAR bType) { (void)bType; } // Do nothing
void L16Screen::SetPalette(SIGNED iFirst, SIGNED iNum, const UCHAR *pGet) {
  (void)iFirst;
  (void)iNum;
  (void)pGet;
}

void L16Screen::HorizontalLine(SIGNED xStart, SIGNED xEnd, SIGNED y,
                               COLORVAL color, SIGNED width) {
  while (width-- > 0) {
    WORD *pPut = mpScanPointers[y] + xStart;
    for (int i = xStart; i <= xEnd; i++) {
      *pPut++ = color;
    }
    y++;
  }
}
void L16Screen::VerticalLine(SIGNED yStart, SIGNED yEnd, SIGNED x,
                             COLORVAL color, SIGNED width) {
  while (width-- > 0) {
    for (int i = yStart; i <= yEnd; i++) {
      PlotPointView(x, i, color);
    }
    x++;
  }
}

void L16Screen::LineView(SIGNED, SIGNED, SIGNED, SIGNED, PegRect &, PegColor,
                         SIGNED) {}
void L16Screen::BitmapView(const PegPoint, const PegBitmap *, const PegRect &) {
}
void L16Screen::RectMoveView(PegThing *, const PegRect &, const SIGNED,
                             const SIGNED) {}
COLORVAL L16Screen::GetPixelView(SIGNED x, SIGNED y) { return 0; }

void L16Screen::DrawTextView(PegPoint Where, const TCHAR *Text, PegColor &Color,
                             PegFont *Font, SIGNED iCount, PegRect &Rect) {
#ifdef PEG_UNICODE
  TCHAR PEGFAR *pCurrentChar = (TCHAR PEGFAR *)Text;
  PegFont *pFontStart = Font;
#else
  UCHAR PEGFAR *pCurrentChar = (UCHAR PEGFAR *)Text;
#endif

  UCHAR PEGFAR *pGetData;
  UCHAR PEGFAR *pGetDataBase;
  WORD wIndex;
  WORD wBitOffset;
  SIGNED wXpos = Where.x;
  WORD cVal = *pCurrentChar++;
  SIGNED iCharWidth;

#ifdef DO_OUTLINE_TEXT
  if (IS_OUTLINE(Font)) {
    DrawOutlineText(Where, Text, Color, Font, iCount, Rect);
    return;
  }
#endif

  while (cVal && wXpos <= Rect.wRight) {
    if (iCount == 0) {
      return;
    }
    iCount--;

#ifdef PEG_UNICODE
    Font = pFontStart;

    while (Font) {
      if (cVal >= Font->wFirstChar && cVal <= Font->wLastChar) {
        break;
      }
      Font = Font->pNext;
    }
    if (!Font) // this font doesn't contain this glyph?
    {
      cVal = *pCurrentChar++; // just skip to next char
      continue;
    }

    wIndex = cVal - (WORD)Font->wFirstChar;

    if (IS_VARWIDTH(Font)) {
      wBitOffset = Font->pOffsets[wIndex];
      iCharWidth = Font->pOffsets[wIndex + 1] - wBitOffset;
    } else {
      iCharWidth = (SIGNED)Font->pOffsets;
      wBitOffset = iCharWidth * wIndex;
    }

#else

    wIndex = cVal - (WORD)Font->wFirstChar;
    wBitOffset = Font->pOffsets[wIndex];
    iCharWidth = Font->pOffsets[wIndex + 1] - wBitOffset;

#endif

    if (wXpos + iCharWidth > Rect.wRight) {
      iCharWidth = Rect.wRight - wXpos + 1;
    }

    WORD ByteOffset = wBitOffset / 8;
    pGetDataBase = Font->pData + ByteOffset;
    pGetDataBase += (Rect.wTop - Where.y) * Font->wBytesPerLine;

    for (SIGNED ScanRow = Rect.wTop; ScanRow <= Rect.wBottom; ScanRow++) {
      pGetData = pGetDataBase;
      UCHAR InMask = 0x80 >> (wBitOffset & 7);
      WORD wBitsOutput = 0;
      UCHAR cData;

#ifdef PEG_UNICODE
      if (ScanRow - Where.y < Font->uHeight) {
        cData = *pGetData++;
      } else {
        cData = 0;
      }
#else
      cData = *pGetData++;
#endif

      while (wBitsOutput < iCharWidth) {
        if (!InMask) {
          InMask = 0x80;
          // read a byte:

#ifdef PEG_UNICODE
          if (ScanRow - Where.y < Font->uHeight) {
            cData = *pGetData++;
          } else {
            cData = 0;
          }
#else
          cData = *pGetData++;
#endif
        }

        if (wXpos >= Rect.wLeft) {
          if (cData & InMask) // is this bit a 1?
          {
            PlotPointView(wXpos, ScanRow, Color.uForeground);
          } else {
            if (Color.uFlags & CF_FILL) {
              PlotPointView(wXpos, ScanRow, Color.uBackground);
            }
          }
        }
        InMask >>= 1;
        wXpos++;
        wBitsOutput++;
        if (wXpos > Rect.wRight) {
          break;
        }
      }
      pGetDataBase += Font->wBytesPerLine;
      wXpos -= iCharWidth;
    }
    wXpos += iCharWidth;
    cVal = *pCurrentChar++;
  }
}
