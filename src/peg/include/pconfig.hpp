// File generated on Sun 14 Sep 2025 17:09:57
/*--------------------------------------------------------------------------*/
//
// pconfig.hpp- PEG configuration flags.
//
// Author:Phoebe
//
/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
// These flags control which features are included in your PEG library.
// When any of these flags are changed, you must rebuilt your working version
// of the library.
//
// These flags also define the target operating system, input devices,
// and to some extent the target video controller.
/*--------------------------------------------------------------------------*/

#define PEG_TOUCH_SUPPORT

#define ONE_SECOND 2 // How many PegMessageQueue::TimerTicks in one second?

#define HALF_SECOND (ONE_SECOND / 2)

#define PEG_PRIORITY
#define PEG_HEAP_SIZE 16384
#define PEG_STACK_SIZE 4096

#define PEG_VIRTUAL_XSIZE 320
#define PEG_VIRTUAL_YSIZE 510
#define PEG_BITBLIT
#define PEG_DOUBLE_BUFFER
#define PEG_HARDWARE_CURSOR
#define PEG_LCD_MODE
#define PEG_MAX_DRAW_NESTING 20

#define PEG_ORIENTATION_NATIVE 0
#define PEG_ORIENTATION_ROTATE_CW 1
#define PEG_ORIENTATION_ROTATE_CCW 2

#define PEG_ORIENTATION PEG_ORIENTATION_NATIVE

/*----------------------------------------------------------------------------*/
// Target CPU Configuration
/*----------------------------------------------------------------------------*/
#define PEG_CPU_SH
#define PEG_BIG_ENDIAN
#define PEGFAR

/*----------------------------------------------------------------------------*/
// Color Depth Configuration
/*----------------------------------------------------------------------------*/
#define PEG_NUM_COLORS 65535
#define PEG_NUM_PAL_ENTRIES 248
#define PEG_GRAYSCALE_PAL_START 232
#define PEG_RGB_USE_565
#define PEG_ATMEL_SAM9

/*----------------------------------------------------------------------------*/
// Standard Color Defines
/*----------------------------------------------------------------------------*/
#define PEG_NUM_C0LORS 65535
#define PEG_COLOR_MODE 8
#define PEG_DEF_TRANSPARENCY 0x0100
#define PEG_DEF_BLACK 0x0000
#define PEG_DEF_RED 0x00b8
#define PEG_DEF_GREEN 0xe005
#define PEG_DEF_BROWN 0xe0bd
#define PEG_DEF_BLUE 0x1700
#define PEG_DEF_MAGENTA 0x17b8
#define PEG_DEF_CYAN 0xf705
#define PEG_DEF_LIGHTGRAY 0x18c6
#define PEG_DEF_DARKGRAY 0x1084
#define PEG_DEF_LIGHTRED 0x00f8
#define PEG_DEF_LIGHTGREEN 0xe007
#define PEG_DEF_YELLOW 0xe0ff
#define PEG_DEF_LIGHTBLUE 0x1f00
#define PEG_DEF_LIGHTMAGENTA 0x1ff8
#define PEG_DEF_LIGHTCYAN 0xff07
#define PEG_DEF_WHITE 0xffff

// 16 Gray color defines.

#define PEG_DEF_GRAY1 1
#define PEG_DEF_GRAY2 2
#define PEG_DEF_GRAY3 3
#define PEG_DEF_GRAY4 4
#define PEG_DEF_GRAY5 5
#define PEG_DEF_GRAY6 6
#define PEG_DEF_GRAY7 7
#define PEG_DEF_GRAY8 8
#define PEG_DEF_GRAY9 9
#define PEG_DEF_GRAY10 10
#define PEG_DEF_GRAY11 11
#define PEG_DEF_GRAY12 12
#define PEG_DEF_GRAY13 13
#define PEG_DEF_GRAY14 14

/*----------------------------------------------------------------------------*/
// Input Device Configuration
/*----------------------------------------------------------------------------*/
#define PEG_KEYBOARD_SUPPORT
#define PEG_ARROW_KEY_SUPPORT
#define PEG_MOUSE_SUPPORT

/*----------------------------------------------------------------------------*/
// Printer Configuration
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
// Drawing Configuration
/*----------------------------------------------------------------------------*/
#define PEG_FULL_GRAPHICS
#define PEG_VIEWPORTS
#define PEG_ARC_GRAPHICS
#define PEG_FULL_CLIPPING

#define PEG_TBUTTON_PADDING 3
#define PEG_RBUTTON_SPACING 10
#define PEG_CBOX_SPACING 10
#define PEG_BMP_TXT_PADDING 2
#define PEG_MENU_CHK_WIDTH 8
#define PEG_TB_WHITESPACE 2
#define PEG_SYS_BTN_SIZE 16
#define PEG_TOOLBAR_PAD_HEIGHT 2
#define PEG_FRAME_WIDTH 5

/*----------------------------------------------------------------------------*/
// String Configuration
/*----------------------------------------------------------------------------*/
// #define PEG_STRLIB
// #define USE_PEG_LTOA
#define PEGCT_IS_CHAR

/*----------------------------------------------------------------------------*/
// Miscellaneous Configuration
/*----------------------------------------------------------------------------*/
#define PEG_MAX_LINE_OFFSETS 100
#define PEG_MAX_PTR_CAPTURE_NESTING 8
#define PEG_SCREEN_STACK_DEPTH 20
#define PEG_USER_PTR_TYPES 5
#define PEG_BTN_INITIAL_REPEAT 20
#define PEG_BTN_REPEAT_RATE 5
#define PEG_DEF_ML_MARKER 0x7c

/*----------------------------------------------------------------------------*/
// This is how many new Viewport or InvalidEntry structures to allocate
// at one time when or if we run out of free entries. We allocate these
// structures in blocks to save time, and we never de-allocate this memory
// unless PEG is terminated.
/*----------------------------------------------------------------------------*/

#define PEG_VIEWPORT_LIST_INC 32

/*----------------------------------------------------------------------------*/
// User Defined Configuration
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
// End of pconfig.hpp file.
/*----------------------------------------------------------------------------*/
