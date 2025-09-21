#ifndef _PEGTHING_
#define _PEGTHING_

// Forward declarations to break circular dependencies
class PegScreen;
class PegPresentationManager;
class PegMessageQueue;

class PegThing {
public:
  // A minimal constructor
  PegThing(const PegRect &Rect) : mClip(Rect) {}

  // Dummy functions that PegScreen needs to compile
  bool StatusIs(WORD wMask) {
    return (wMask & PSF_VISIBLE);
  } // Assume always visible for now
  PegThing *Parent() { return nullptr; }

  // The clipping rectangle is the only member we need for now.
  PegRect mClip;

  // Dummy static pointers. We will set the screen pointer.
  static PegScreen *mpScreen;
  static PegPresentationManager *mpPresentation;
  static PegMessageQueue *mpMessageQueue;

  static void SetScreenPtr(PegScreen *ps) { mpScreen = ps; }
};

#endif