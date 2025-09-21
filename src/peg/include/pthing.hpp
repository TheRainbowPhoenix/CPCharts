#ifndef _PEGTHING_
#define _PEGTHING_

// Forward declarations to break circular dependencies
class PegScreen;
class PegPresentationManager;
class PegMessageQueue;

class PegThing {
public:
  PegThing(const PegRect &Rect, WORD wId = 0)
      : mReal(Rect), mClient(Rect), mClip(Rect), mwId(wId) {}
  PegThing() {
    mReal.Set(0, 0, 0, 0);
    mClient = mClip = mReal;
    mwId = 0;
  }
  virtual ~PegThing() {}

  bool StatusIs(WORD wMask) { return (mwStatus & wMask); }
  PegThing *Parent() { return mpParent; }

  // Static pointers for the global PEG environment
  static PegScreen *mpScreen;
  static PegPresentationManager *mpPresentation;
  static PegMessageQueue *mpMessageQueue;
  static void SetScreenPtr(PegScreen *ps) { mpScreen = ps; }

  // Member variables needed for drawing and identification
  PegRect mReal;
  PegRect mClient;
  PegRect mClip;
  WORD mwId;
  WORD mwStatus = PSF_VISIBLE; // Assume visible by default for simplicity

protected:
  PegThing *mpParent = nullptr;
};

#endif