#include <appdef.hpp>
#include <sdk/calc/calc.hpp>
#include <sdk/os/debug.hpp>
#include <sdk/os/input.hpp>
#include <sdk/os/lcd.hpp>
#include <stdio.h>
#include <stdlib.h>

#include "peg/include/peg.hpp"

APP_NAME("PEG Test")
APP_DESCRIPTION("PegRect Test")
APP_AUTHOR("Phoebe")
APP_VERSION("1.0.0")
// =================================================================
// Dummy Font Definition
// =================================================================
// Bitmap data for an 8x8 font containing 'A' and 'B'
#define FONT_ADDRESS 0x8c1a70cc
PegFont *pSystemFont = (PegFont *)FONT_ADDRESS;

// =================================================================
// Helper Functions
// =================================================================

int getCharPixelWidth(TCHAR ch, const PegFont *pFont) {
  if (!pFont || ch < pFont->wFirstChar || ch >= pFont->wLastChar) {
    return 0;
  }
  int charIndex = ch - pFont->wFirstChar;

  // Width is the difference between the next char's offset and this char's
  // offset.
  int width = pFont->pOffsets[charIndex + 1] - pFont->pOffsets[charIndex];
  return width;
}

// CORRECTED: Renders a single 1bpp character from a pitched font atlas.
void drawVariableChar(TCHAR ch, PegPoint pos, const PegFont *pFont,
                      uint16_t color) {
  int width = getCharPixelWidth(ch, pFont);
  if (width <= 0) {
    return;
  }

  // Get the starting bit position for the top-left pixel of this character.
  int startBitOffset = pFont->pOffsets[ch - pFont->wFirstChar];

  // Loop for each row of the character
  for (int y = 0; y < pFont->uHeight; y++) {
    // Calculate the starting bit for the current row by adding the pitch for
    // each row.
    int startBitOfScanline = startBitOffset + (y * pFont->wBytesPerLine * 8);

    // Loop for each pixel in the row
    for (int x = 0; x < width; x++) {
      int currentBit = startBitOfScanline + x;
      int byteIndex = currentBit / 8;
      int bitInByte = 7 - (currentBit % 8);

      // Check if the bit is set in the data blob
      if ((pFont->pData[byteIndex] >> bitInByte) & 1) {
        // Draw the pixel at the correct screen coordinate
        setPixel(pos.x + x, pos.y + y, color);
      }
    }
  }
}

// Renders a string of text using the variable-width font renderer.
void drawString(const char *str, PegPoint pos, const PegFont *pFont,
                uint16_t color) {
  PegPoint currentPos = pos;
  for (int i = 0; str[i] != '\0'; i++) {
    TCHAR ch = str[i];
    drawVariableChar(ch, currentPos, pFont, color);
    // Advance the X position by the width of the character just drawn
    currentPos.x += getCharPixelWidth(ch, pFont);
  }
}

// Prints a formatted header for each test page.
void printHeader(const char *title) {
  // Simulate a screen clear with newlines and print a title bar.
  printf("========================================\n");
  printf("  %s\n", title);
  printf("========================================\n");
}

// Prints PegRect details for easy debugging.
void printRect(const char *label, const PegRect &rect) {
  printf("%-26s: L:%-4d T:%-4d R:%-4d B:%-4d (W:%d, H:%d)\n", label, rect.wLeft,
         rect.wTop, rect.wRight, rect.wBottom, rect.Width(), rect.Height());
}

// Compares actual vs. expected results and prints PASS or FAIL.
void printTestResult(const char *label, bool actual, bool expected) {
  printf("%-28s: (%s)\n", label, (actual == expected) ? "PASS" : "FAIL");
}

// Overloaded version for comparing PegRect objects.
void printTestResult(const char *label, const PegRect &actual,
                     const PegRect &expected) {
  bool passed = (actual == expected);
  printf("%-28s: (%s)\n", label, passed ? "PASS" : "FAIL");
  if (!passed) {
    printRect("...Actual", actual);
    printRect("...Expected", expected);
  }
}

bool _wait() {
  struct InputEvent event;
  while (true) {
    GetInput(&event, 0xFFFFFFFF, 0x10);

    // On any key press...
    if (event.type == EVENT_KEY) {
      return (event.data.key.keyCode != KEYCODE_POWER_CLEAR);
    }
    LCD_Refresh(); // Keep the system responsive while waiting
  }
}

// Waits for any key press to continue, or CLEAR to exit the program.
// Returns 'false' if the user wants to exit, 'true' otherwise.
bool waitForKeyPress() {
  printf("\n> [EXE] = continue, [CLEAR] = exit\n");
  fflush(stdout);
  return _wait();
}

// --- New Drawing Helper ---
void drawChar(TCHAR ch, PegPoint pos, const PegFont *pFont, uint16_t color) {
  if (!pFont || ch < pFont->wFirstChar || ch > pFont->wLastChar) {
    return; // Character not in font
  }

  // Calculate the starting position in the bitmap data array
  WORD charOffset =
      (ch - pFont->wFirstChar) * pFont->uHeight * pFont->wBytesPerLine;
  const UCHAR *pData = pFont->pData + charOffset;

  // Loop through each row (scanline) of the character
  for (UCHAR y = 0; y < pFont->uHeight; y++) {
    UCHAR rowData = *pData;
    // Loop through each bit (pixel) in the row
    for (UCHAR x = 0; x < 8; x++) {
      if (rowData & (0x80 >> x)) { // Check if the bit is set
        setPixel(pos.x + x, pos.y + y, color);
      }
    }
    pData++; // Move to the next row of data
  }
}

class ViewportThing : public PegThing {
public:
  ViewportThing(const PegRect &Rect) : PegThing(Rect) {
    mwStatus |= PSF_VIEWPORT; // This is the crucial flag
  }

  // Add a public way to set our viewport list for this demo
  void SetViewportList(Viewport *pList) { mpViewportList = pList; }
};

#define BASE_TEST 0

extern "C" int __attribute__((section(".bootstrap.text"))) main(void) {
  calcInit();

  do {
    PegRect rectA, rectB, rectC, resultRect, expectedRect;
    PegPoint p1, p2;
    if (BASE_TEST) {

      // --- Test Flow ---
      // ======================== Test 1: Initialization
      // ========================
      printHeader("Test 1: Initialization");
      rectA.Set(10, 10, 100, 60);
      p1.x = 20;
      p1.y = 20;
      p2.x = 120;
      p2.y = 70;
      rectB.Set(p1, p2);
      printRect("Rect A (from integers)", rectA);
      printRect("Rect B (from PegPoints)", rectB);
      printTestResult("Rect A Width", rectA.Width() == 91, true);
      printTestResult("Rect A Height", rectA.Height() == 51, true);
      printTestResult("Rect B Width", rectB.Width() == 101, true);
      if (!waitForKeyPress())
        break;

      // =================== Test 2: Contains & Overlap ===================
      printHeader("Test 2: Contains & Overlap");
      rectC.Set(20, 20, 40, 40);
      p1.x = 30;
      p1.y = 30;
      p2.x = 0;
      p2.y = 0;
      printRect("Container Rect A", rectA);
      printRect("Contained Rect C", rectC);
      printTestResult("A.Contains(30,30)", rectA.Contains(p1), true);
      printTestResult("A.Contains(0,0)", rectA.Contains(p2), false);
      printTestResult("A.Contains(C)", rectA.Contains(rectC), true);
      printTestResult("A.Overlap(B)", rectA.Overlap(rectB), true);
      if (!waitForKeyPress())
        break;

      // ====================== Test 3: Movement ======================
      printHeader("Test 3: Movement");
      rectA.Set(10, 10, 100, 60); // Reset A
      printRect("Rect A initial", rectA);
      rectA.Shift(10, 10);
      expectedRect.Set(20, 20, 110, 70);
      printTestResult("A after Shift(10,10)", rectA, expectedRect);
      rectA.MoveTo(0, 0);
      expectedRect.Set(0, 0, 90, 50);
      printTestResult("A after MoveTo(0,0)", rectA, expectedRect);
      if (!waitForKeyPress())
        break;

      // =================== Test 4: Comparison Operators ===================
      printHeader("Test 4: Comparison Operators");
      rectA.Set(10, 10, 50, 50);
      rectB.Set(10, 10, 50, 50);
      rectC.Set(0, 0, 1, 1);
      printRect("Rect A", rectA);
      printRect("Rect B", rectB);
      printRect("Rect C", rectC);
      printTestResult("A == B", (rectA == rectB), true);
      printTestResult("A != C", (rectA != rectC), true);
      if (!waitForKeyPress())
        break;

      // =================== Test 5: Geometric Operators ===================
      printHeader("Test 5: Geometric Operators");
      rectA.Set(0, 0, 50, 50);
      rectB.Set(30, 30, 80, 80);
      printRect("Rect A", rectA);
      printRect("Rect B", rectB);
      resultRect = rectA & rectB;
      expectedRect.Set(30, 30, 50, 50);
      printTestResult("Intersection (A & B)", resultRect, expectedRect);
      rectA |= rectB;
      expectedRect.Set(0, 0, 80, 80);
      printTestResult("Union (A |= B)", rectA, expectedRect);
      if (!waitForKeyPress())
        break;

      // =================== Test 6: Arithmetic & Growth ===================
      printHeader("Test 6: Arithmetic & Growth");
      rectA.Set(20, 20, 40, 40);
      p1.x = 100;
      p1.y = 50;
      printRect("Rect A", rectA);
      printf("Shift Point P1 at (%d, %d)\n", p1.x, p1.y);
      resultRect = rectA + p1;
      printRect("New Rect (A + P1)", resultRect);
      printRect("Start Rect A", rectA);
      rectA.Set(20, 20, 40, 40);
      rectA++;
      expectedRect.Set(19, 19, 41, 41);
      printTestResult("A after ++", rectA, expectedRect);
      rectA--;
      expectedRect.Set(20, 20, 40, 40);
      printTestResult("A after --", rectA, expectedRect);
      rectA += 10;
      expectedRect.Set(10, 10, 50, 50);
      printTestResult("A after += 10", rectA, expectedRect);
      rectA -= 5;
      expectedRect.Set(15, 15, 45, 45);
      printTestResult("A after -= 5", rectA, expectedRect);
      if (!waitForKeyPress())
        break;

      // =================== Test 7: Subtraction Operator ===================
      printHeader("Test 7: Subtraction Operator (^=)");
      rectA.Set(0, 0, 100, 100);
      rectB.Set(50, 50, 150, 150);
      printRect("Original A", rectA);
      printRect("Clip Rect B", rectB);
      rectA ^= rectB; // Subtract B from A
      expectedRect.Set(0, 0, 49, 100);
      printTestResult("Result A ^= B", rectA, expectedRect);

      // =================== Test 8: PegFont & PegTextThing ===================
      printHeader("Test 8: PegFont & PegTextThing");

      // Test the static font functions
      PegTextThing::SetDefaultFont(PEG_DEFAULT_FONT, pSystemFont);
      PegFont *pFont = PegTextThing::GetDefaultFont(PEG_DEFAULT_FONT);
      printTestResult("GetDefaultFont returns set font", pFont == pSystemFont,
                      true);

      // Test PegTextThing instantiation
      PegTextThing myText("AB", TT_COPY);
      myText.SetFont(pFont);
      printf("PegTextThing created with text: '%s'\n", myText.DataGet());

      printf("Now rendering to screen...\n");
      fflush(stdout);

      // --- Display Font Information ---
      printHeader("In-Memory Font Test");
      printf("Accessing PegFont struct at 0x%08X\n", pSystemFont);
      printf("Font Height: %d\n", pSystemFont->uHeight);
      printf("Char Range: %d - %d\n", pSystemFont->wFirstChar,
             pSystemFont->wLastChar);
      printf("Atlas Pitch (Bytes/Line): %d\n", pSystemFont->wBytesPerLine);

      // Visual test: Draw the characters to the screen
      fillScreen(color(0, 0, 0)); // Black background
      // --- Visual Test ---
      printf("\nRendering font to screen...\n");
      fflush(stdout);

      fillScreen(color(255, 255, 255)); // White background

      // Define some text to draw
      const char *line1 = "Hello, World!";
      const char *line2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
      const char *line3 = "abcdefghijklmnopqrstuvwxyz";
      const char *line4 = "0123456789 `~!@#$%^&*()";
      const char *line5 = "-_=+[]{}|;:'\",./<>?";

      // Draw the text using the in-memory font
      drawString(line1, {10, 10}, pSystemFont, color(0, 0, 0));
      drawString(line2, {10, 30}, pSystemFont, color(255, 0, 0));   // Red
      drawString(line3, {10, 50}, pSystemFont, color(0, 128, 0));   // Green
      drawString(line4, {10, 70}, pSystemFont, color(0, 0, 255));   // Blue
      drawString(line5, {10, 90}, pSystemFont, color(128, 0, 128)); // Purple

      LCD_Refresh();
      Debug_WaitKey();
    }

    printf("Start PegScreen test...\n");

    PegScreen *pScreen = CreatePegScreen();
    PegThing::SetScreenPtr(pScreen);

    PegRect screenRect;
    screenRect.Set(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
    PegThing RootThing(screenRect);

    // --- Start Drawing ---
    // pScreen->BeginDraw(&RootThing);
    fillScreen(color(255, 255, 255)); // White background

    // Define text and colors
    const char *line1 = "Hello, PEG Screen!";
    const char *line2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char *line3 = "0123456789!@#$%^&*()";

    PegColor Black(color(0, 0, 0));
    PegColor Red(color(255, 0, 0));
    PegColor Blue(color(0, 0, 255));
    PegColor Green(color(100, 255, 100));

    // pScreen->DrawText(&RootThing, {10, 10}, line1, Black, pSystemFont);
    // pScreen->DrawText(&RootThing, {10, 30}, line2, Red, pSystemFont);
    // pScreen->DrawText(&RootThing, {10, 50}, line3, Blue, pSystemFont);
    // pScreen->DrawText(&RootThing, {238, 50}, line2, Black, pSystemFont);

    // 2. Create two "windows" which will own viewports
    rectA.Set(10, 10, 150, 100);
    ViewportThing Vp1(rectA);

    rectB.Set(170, 50, 310, 200);
    ViewportThing Vp2(rectB);

    pScreen->GenerateViewportList(&Vp1);
    pScreen->GenerateViewportList(&Vp2);

    pScreen->BeginDraw(&Vp1);
    // fillScreen(color(50, 50, 50));
    const char *text = "This text is clipped by Viewport 1";
    pScreen->DrawText(&Vp1, {5, 5}, text, Blue, pSystemFont);
    pScreen->DrawText(&Vp2, {160, 60}, "This text is clipped by Viewport 2",
                      Green, pSystemFont);

    // --- End Drawing ---
    pScreen->EndDraw();
    LCD_Refresh();
    Debug_WaitKey();
    // Clean up
    delete pScreen;

    if (!waitForKeyPress())
      break;
  } while (false); // Loop only once, using 'break' to exit early.

  // --- Final Exit Message ---
  printf("\n--- All tests complete. Press CLEAR to exit. ---\n");
  fflush(stdout);

  // Wait specifically for the CLEAR key to terminate the app
  struct InputEvent event;
  while (true) {
    GetInput(&event, 0xFFFFFFFF, 0x10);
    if (event.type == EVENT_KEY &&
        event.data.key.keyCode == KEYCODE_POWER_CLEAR) {
      break;
    }
    LCD_Refresh();
  }

  calcEnd();
  exit(EXIT_SUCCESS);
}