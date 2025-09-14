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
UCHAR DummyFontData[] = {
    // Char 'A'
    0x18, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x00,
    // Char 'B'
    0x7C, 0x42, 0x42, 0x7C, 0x42, 0x42, 0x7C, 0x00};

// The PegFont structure describing our dummy font
PegFont DummyFont = {
    0,            // uType: 0 = Fixed-width, no outline/alias
    7,            // uAscent:  Ascent above baseline
    1,            // uDescent: Descent below baseline
    8,            // uHeight:  Total height of character
    1,            // wBytesPerLine: 1 byte for an 8-pixel wide character
    'A',          // wFirstChar:  First character is 'A'
    'B',          // wLastChar:   Last character is 'B'
    NULL,         // pOffsets:    NULL for fixed-width fonts
    NULL,         // pNext:       NULL for single-page fonts
    DummyFontData // pData:       Pointer to the bitmap data
};

// =================================================================
// Helper Functions
// =================================================================

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

extern "C" int __attribute__((section(".bootstrap.text"))) main(void) {
  calcInit();

  PegRect rectA, rectB, rectC, resultRect, expectedRect;
  PegPoint p1, p2;

  // --- Test Flow ---
  do {
    // ======================== Test 1: Initialization ========================
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
    PegTextThing::SetDefaultFont(PEG_DEFAULT_FONT, &DummyFont);
    PegFont *pFont = PegTextThing::GetDefaultFont(PEG_DEFAULT_FONT);
    printTestResult("GetDefaultFont returns set font", pFont == &DummyFont,
                    true);

    // Test PegTextThing instantiation
    PegTextThing myText("AB", TT_COPY);
    myText.SetFont(pFont);
    printf("PegTextThing created with text: '%s'\n", myText.DataGet());

    printf("Now rendering to screen...\n");
    fflush(stdout);

    // Visual test: Draw the characters to the screen
    fillScreen(color(0, 0, 0));                         // Black background
    drawChar('A', {10, 20}, pFont, color(255, 255, 0)); // Yellow 'A'
    drawChar('B', {20, 20}, pFont, color(0, 255, 255)); // Cyan 'B'
    LCD_Refresh();
    Debug_WaitKey();

    fillScreen(color(0, 0, 0)); // Black background

    // Create a PegTextThing with a string to display
    PegTextThing line1("PEG TEST", TT_COPY);
    line1.SetFont(&Latin_Font_9);

    PegTextThing line2("0123456789", TT_COPY);
    line2.SetFont(&Latin_Font_9);

    // Get the text to render
    TCHAR *text1 = line1.DataGet();
    TCHAR *text2 = line2.DataGet();

    PegPoint pos = {10, 20}; // Starting position

    // Draw the first line
    for (int i = 0; text1[i] != '\0'; i++) {
      drawChar(text1[i], pos, &Latin_Font_9, color(255, 255, 0)); // Yellow
      pos.x += 8; // Advance X position for next char (8 pixels wide)
    }

    // Move to the next line
    pos.x = 10;
    pos.y += 12; // Move Y down by 12 pixels

    // Draw the second line
    for (int i = 0; text2[i] != '\0'; i++) {
      drawChar(text2[i], pos, &Latin_Font_9, color(0, 255, 255)); // Cyan
      pos.x += 8;
    }

    LCD_Refresh();
    Debug_WaitKey();

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