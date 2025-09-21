#ifndef _PEGTHING_
#define _PEGTHING_

// Forward declarations to break circular dependencies
class PegScreen;
struct Viewport;
class PegPresentationManager;
class PegMessageQueue;

class PegThing {
public:
  PegThing(const PegRect &Rect, WORD wId = 0, WORD wStyle = FF_NONE);
  PegThing(WORD wId = 0, WORD wStyle = FF_NONE);
  virtual ~PegThing();
  virtual SIGNED Message(const PegMessage &Mesg);
  virtual void Draw(void);

  bool StatusIs(WORD wMask) { return (mwStatus & wMask); }
  PegThing *Parent() { return mpParent; }
  void Add(PegThing *Who, bool bDraw = true);
  PegThing *Remove(PegThing *Who, bool bDraw = true);

  PegThing *Parent(void) const { return mpParent; }
  PegThing *First(void) const { return mpFirst; }
  PegThing *Next(void) const { return mpNext; }
  PegThing *Previous(void) const { return mpPrev; }

  PegScreen *Screen() const { return mpScreen; }

  static void SetScreenPtr(PegScreen *ps) { mpScreen = ps; }
  Viewport *ViewportList(void) { return mpViewportList; }

  // Member variables needed for drawing and identification
  PegRect mReal;
  PegRect mClient;
  PegRect mClip;

  // protected:
  UCHAR muType;
  WORD mwStyle;
  WORD mwId;
  WORD mwStatus;
  WORD mwSignalMask;
  PegThing *mpParent = nullptr;
  PegThing *mpFirst;
  PegThing *mpNext;
  PegThing *mpPrev;
  Viewport *mpViewportList;

  static PegScreen *mpScreen;
  static PegPresentationManager *mpPresentation;
  static PegMessageQueue *mpMessageQueue;
};

#endif