#include "../include/peg.hpp"

// clang-format off

PegThing::PegThing(const PegRect &Rect, WORD wId, WORD wStyle) :
    mReal(Rect),
    mClient(Rect),
    mClip(Rect),
    muType(TYPE_THING),
    mwStyle(wStyle),
    mwId(wId),
    mwStatus(PSF_SELECTABLE | PSF_ACCEPTS_FOCUS),
    mwSignalMask(0),
    mpParent(NULL),
    mpFirst(NULL),
    mpNext(NULL),
    mpPrev(NULL)
{
#ifdef PEG_FULL_CLIPPING
    mpViewportList = NULL;
#endif
}

PegThing::PegThing(WORD wId, WORD wStyle) :
    muType(TYPE_THING),
    mwStyle(wStyle),
    mwId(wId),
    mwStatus(PSF_SELECTABLE|PSF_ACCEPTS_FOCUS),
    mwSignalMask(0),
    mpParent(NULL),
    mpFirst(NULL),
    mpNext(NULL),
    mpPrev(NULL)

{
   #ifdef PEG_FULL_CLIPPING
    mpViewportList = NULL;
   #endif
    mReal.Set(0, 0, 0, 0);
    mClient = mReal;
    mClip = mReal;
}

/*--------------------------------------------------------------------------*/
// Destructor- Remove and delete any children.
/*--------------------------------------------------------------------------*/
PegThing::~PegThing()
{
    while (mpFirst)
    {
        PegThing *DelPtr = mpFirst;
        mpFirst = DelPtr->mpNext;
        delete DelPtr;
    }

   #ifdef PEG_FULL_CLIPPING
    LOCK_PEG
    if (mpViewportList)
    {
        Screen()->FreeViewports(this);
    }
    UNLOCK_PEG
   #endif
}

// Dummy implementations for now
SIGNED PegThing::Message(const PegMessage &Mesg) { return 0; }
void PegThing::Draw(void) {}
void PegThing::Add(PegThing* Who, bool bDraw) {}
PegThing* PegThing::Remove(PegThing* Who, bool bDraw) { return nullptr; }

// Define the static member variables for PegThing
PegScreen *PegThing::mpScreen = nullptr;
PegPresentationManager *PegThing::mpPresentation = nullptr;
PegMessageQueue *PegThing::mpMessageQueue = nullptr;