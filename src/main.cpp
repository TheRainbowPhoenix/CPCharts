#include <appdef.hpp>
#include <sdk/calc/calc.hpp>
#include <sdk/os/input.hpp>
#include <stdio.h>
#include <stdlib.h>

#include "peg/include/peg.hpp"

APP_NAME("PEG Test")
APP_DESCRIPTION("WiP PEG Test")
APP_AUTHOR("Phoebe")
APP_VERSION("1.0.0")

extern "C" int __attribute__((section(".bootstrap.text"))) main(void) {
  calcInit();

  printf("Starting PEG Demo...\n");
  PegRect myButton;
  myButton.Set(10, 20, 100, 50); // Left, Top, Right, Bottom

  printf("Button defined at: (%d, %d) to (%d, %d)\n", myButton.wLeft,
         myButton.wTop, myButton.wRight, myButton.wBottom);
  printf("Button width: %d, height: %d\n", myButton.Width(), myButton.Height());

  // 2. Create some test points (like a screen touch)
  PegPoint pointInside = {50, 30};
  PegPoint pointOutside = {5, 5};

  // 3. Use the Contains() method to check for hits
  if (myButton.Contains(pointInside)) {
    printf("Point (%d, %d) is INSIDE the button.\n", pointInside.x,
           pointInside.y);
  } else {
    printf("Point (%d, %d) is OUTSIDE the button.\n", pointInside.x,
           pointInside.y);
  }

  if (myButton.Contains(pointOutside)) {
    printf("Point (%d, %d) is INSIDE the button.\n", pointOutside.x,
           pointOutside.y);
  } else {
    printf("Point (%d, %d) is OUTSIDE the button.\n", pointOutside.x,
           pointOutside.y);
  }

  // 4. Demonstrate the Shift() method
  myButton.Shift(100, 50); // Move the rectangle 100px right and 50px down
  printf("After shifting, button is now at: (%d, %d)\n", myButton.wLeft,
         myButton.wTop);

  LCD_Refresh();

  struct InputEvent event;
  bool running = true;
  while (running) {
    GetInput(&event, 0xFFFFFFFF, 0x10);

    if (event.type == EVENT_KEY &&
        event.data.key.keyCode == KEYCODE_POWER_CLEAR) {
      running = false;
    }

    LCD_Refresh();
  }

  calcEnd();
  exit(EXIT_SUCCESS);
}