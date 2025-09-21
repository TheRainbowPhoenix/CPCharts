
#ifndef _PEGMESSAGE_
#define _PEGMESSAGE_

/*--------------------------------------------------------------------------*/
// Standard PEG generated message types:

enum PegSystemMessage {
  PM_ADD = 1,
  PM_REMOVETHING,
  PM_DESTROY,
  PM_SIZE,
  PM_MOVE,
  PM_CLOSE,
  PM_HIDE,
  PM_SHOW,
  PM_POINTER_MOVE,
  PM_LBUTTONDOWN,
  PM_LBUTTONUP,
  PM_RBUTTONDOWN,
  PM_RBUTTONUP,
  PM_DRAW,
  PM_CURRENT,
  PM_NONCURRENT,
  PM_POINTER_ENTER,
  PM_POINTER_EXIT,
  PM_EXIT,
  PM_ADDICON,
  PM_BEGIN_MOVE,
  PM_PARENTSIZED,
  PM_VSCROLL,
  PM_HSCROLL,
  PM_MAXIMIZE,
  PM_MINIMIZE,
  PM_RESTORE,
  PM_CLOSE_SIBLINGS,
  PM_TIMER,
  PM_KEY,
  PM_KEY_HOLD,
  PM_KEY_RELEASE,
  PM_CUT,
  PM_COPY,
  PM_PASTE,
  PM_SLIDER_DRAG,
  PM_MWCOMPLETE,
  PM_DIALOG_NOTIFY,
  PM_DIALOG_APPLY,
  PM_MOVE_FOCUS

};

const WORD FIRST_SIGNAL = 128;
const WORD FIRST_USER_MESSAGE = 0x4000;

class PegThing; // forward reference

// clang-format off

struct PegMessage
{
	public:
    PegMessage() {Next = NULL; pTarget = NULL; pSource = NULL;}
    PegMessage(WORD wVal) {Next = NULL; pTarget = NULL; pSource = NULL; wType = wVal;}
    PegMessage(PegThing *pTo, WORD wVal) {Next = NULL; pTarget = pTo; pSource = NULL; wType = wVal;}
    WORD wType;
    SIGNED iData;
    PegThing *pTarget;
    PegThing *pSource;
    PegMessage *Next;
 
    union
    {
        void *pData;
        LONG lData;
        PegRect Rect;
        PegPoint Point;
        LONG    lUserData[2];
        DWORD   dUserData[2];
        SIGNED  iUserData[4];
        WORD    wUserData[4];
        UCHAR   uUserData[8];
    };
};


#endif
