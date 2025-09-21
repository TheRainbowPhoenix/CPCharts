/*--------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------

Notes:

  ********************************************************************
  ***                                                              ***
  *** If you are looking for the PEG library configuration flags,  ***
  *** they have been moved to the new header file                  ***
  *** \peg\include\pconfig.hpp                                     ***
  ***                                                              ***
  ********************************************************************

  This is the only file that needs to be included by the application level
  software in order to use PEG.

----------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

#ifndef _PEGDEFS_
#define _PEGDEFS_

#include "pconfig.hpp"

#define NUM_PEG_FREE_MSGS 40 // how many messages are available

#define LOCK_PEG
#define UNLOCK_PEG
#define LOCK_MESSAGE_QUEUE
#define UNLOCK_MESSAGE_QUEUE
#define LOCK_TIMER_LIST
#define UNLOCK_TIMER_LIST
#define ENQUEUE_TASK_MESSAGE(a, b)

#include "pegtypes.hpp"

#include "pegkeys.hpp"
#include "pfonts.hpp"
// #include "psysfont.hpp"

#include "pscreen.hpp"
#include "pthing.hpp"

// #include "pmessage.hpp"
#include "l16screen.hpp"

// #include "pmenu.hpp"
// #include "pbutton.hpp"
// #include "ptoolbar.hpp"
// #include "pscroll.hpp"
// #include "pspin.hpp"
// #include "ptitle.hpp"
// #include "pprompt.hpp"
// #include "pstatbar.hpp"
// #include "pwindow.hpp"
// #include "pnotebk.hpp"
// #include "ppresent.hpp"
// #include "pstring.hpp"
// #include "plist.hpp"
// #include "pcombo.hpp"
// #include "ptextbox.hpp"
// #include "peditbox.hpp"
// #include "pdecwin.hpp"
// #include "pdialog.hpp"
// #include "pmesgwin.hpp"
// #include "pcursors.hpp"
// #include "pprogbar.hpp"
// #include "pprogwin.hpp"
// #include "PegActionKeys.hpp"

#ifndef ASSERT

#define ASSERT(x)
#define CP_ASSERT(x)
#define CP_VALIDATE(x) (x)
#endif

#endif