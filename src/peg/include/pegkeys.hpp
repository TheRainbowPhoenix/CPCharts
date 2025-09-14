#ifndef _PEGKEYDEFS_
#define _PEGKEYDEFS_

/*--------------------------------------------------------------------------*/
// The general form of PM_KEY and PM_KEY_RELEASE messages is:
//
// Mesg.pTarget = NULL;     // i.e. routed by PresentationManager
// Mesg.wType   = PM_KEY;   // or PM_KEY_RELEASE
// Mesg.iData   = keyval;   // ASCII or extended key value, defined below.
// Mesg.lData   = flags;    // <ctrl>, <alt>, <shift>, etc, defined below.
//
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
// Non-printable key definitions. The are the values passed for extended
// keys like function keys, home, end, etc... regardless of the keyboard or
// input device in use. The Keyboard driver must convert raw scan codes into
// these defined values using a lookup table or other means. The PEG controls
// do not interpret raw scan codes, that is the job of the keyboard driver.
// The key definitions below never change, but you can add new key definitions
// if you want to for your application. The PEG controls are only looking for
// the following key values.
//
// The extended key codes are defined so as not to overlap printable Unicode
// characters- i.e. they are in the IPA extensions block.
/*--------------------------------------------------------------------------*/

#define PK_BACKSPACE 0x08
#define PK_TAB 0x09
#define PK_CR 0x0d
#define PK_ESC 0x1b

//
#define PK_DELETE 600     // delete key
#define PK_INSERT 601     // insert key
#define PK_HOME 602       // home key
#define PK_END 603        // end key
#define PK_PGUP 604       // page up
#define PK_PGDN 605       // page down
#define PK_LNUP 606       // line up
#define PK_LNDN 607       // line down
#define PK_LEFT 608       // left arrow
#define PK_RIGHT 609      // right arrow
#define PK_ALT 610        // alt key
#define PK_CTRL 611       // control key
#define PK_SHIFT 612      // shift key
#define PK_CAPS 613       // caps lock key
#define PK_NUMLOCK 614    // num lock key
#define PK_SCROLLLOCK 615 // scroll lock key

#define PK_F1 650  // F1 function key
#define PK_F2 651  // F2 function key
#define PK_F3 652  // F3 function key
#define PK_F4 653  // F4 function key
#define PK_F5 654  // F5 function key
#define PK_F6 655  // F6 function key
#define PK_F7 656  // F7 function key
#define PK_F8 657  // F8 function key
#define PK_F9 658  // F9 function key
#define PK_F10 659 // F10 function key
#define PK_F11 660 // F11 function key
#define PK_F12 661 // F12 function key

/*--------------------------------------------------------------------------*/
// The default menu activation key- can be changed to anything, but
// defaults to "F1". We advise against using the ALT key, as this is
// intercepted by Windows when running in the Win32 environment.
/*--------------------------------------------------------------------------*/

#define PK_MENU_KEY PK_F1

/*--------------------------------------------------------------------------*/
// For ASCII keys, the driver should place the actual key value (i.e. 'a' or
// 'A' in the message.iData member. The key-modifiers are placed in the
// low byte of the PM_KEY message lData member, according to the following
// format:
/*--------------------------------------------------------------------------*/

#ifndef PHARLAP
#define KF_RSHIFT 0x01 // right shift
#define KF_LSHIFT 0x02 // left shift
#define KF_SHIFT 0x03  // either shift
#define KF_CTRL 0x04   // control key
#define KF_ALT 0x08    // alt key
#define KF_SCROLL 0x10 // scroll lock
#define KF_NUML 0x20   // num lock
#define KF_CAPS 0x40   // caps lock
#define KF_INS 0x80    // insert
#else

// We re-define the control flags for running with ETS so that flag
// translation is done at compile time, rather than shifting at run time.
// These definitions are masks that can be applied to the lData parameter
// of PM_KEY messages to determine control key status.

#define KF_SHIFT 0x10
#define KF_CTRL 0x0c
#define KF_ALT 0x03
#define KF_SCROLL 0x40
#define KF_NUML 0x20
#define KF_CAPS 0x80

#endif

/*--------------------------------------------------------------------------*/
// Note: for mouse messages, if keyboard modifiers are supported, the key
// modifiers are placed in the mouse message iData member. Keyboard modifiers
// are not required in mouse messages unless your application requires them.
/*--------------------------------------------------------------------------*/

#endif