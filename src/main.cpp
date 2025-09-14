#include <appdef.hpp>
#include <sdk/calc/calc.hpp>
#include <sdk/os/input.hpp>
#include <stdio.h>
#include <stdlib.h>

#include "peg/include/peg.hpp"

APP_NAME("PEG Test")
APP_DESCRIPTION("PegRect Test")
APP_AUTHOR("Phoebe")
APP_VERSION("1.0.0")

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

// Waits for any key press to continue, or CLEAR to exit the program.
// Returns 'false' if the user wants to exit, 'true' otherwise.
bool waitForKeyPress() {
  printf("\n> [EXE] = continue, [CLEAR] = exit\n");
  fflush(stdout);

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