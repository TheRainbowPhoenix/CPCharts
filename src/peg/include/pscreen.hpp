#ifndef _PEGSCREEN_
#define _PEGSCREEN_

class PegThing; // Forward reference

#ifdef PEG_FULL_CLIPPING

#define VIEWPORT_LIST_INCREMENT                                                \
  32 // This is how many viewports to add when
     // we run out. There is a global pool of
     // viewports, allocated to each PSF_VIEWPORT
     // OBJECT as needed.

struct Viewport {
  PegRect mView;
  Viewport *pNext;
};

#endif

class PegScreen {
public:
  PegScreen(const PegRect &);
  virtual ~PegScreen();

  // ---- Pure Virtual Functions we MUST implement in L16Screen ----
  virtual void BeginDraw(PegThing *) = 0;
  virtual void EndDraw() = 0;
  virtual void SetPointerType(UCHAR bType) = 0;
  virtual void SetPalette(SIGNED iFirst, SIGNED iNum, const UCHAR *pGet) = 0;

protected:
  // ---- Protected Pure Virtuals we MUST implement ----
  virtual void DrawTextView(PegPoint Where, const TCHAR *Text, PegColor &Color,
                            PegFont *pFont, SIGNED iLen, PegRect &View) = 0;
  virtual void LineView(SIGNED xStart, SIGNED yStart, SIGNED xEnd, SIGNED yEnd,
                        PegRect &View, PegColor Color, SIGNED iWidth) = 0;
  virtual void BitmapView(const PegPoint Where, const PegBitmap *pMap,
                          const PegRect &View) = 0;
  virtual void RectMoveView(PegThing *Caller, const PegRect &View,
                            const SIGNED xMove, const SIGNED yMove) = 0;
  virtual void HorizontalLine(SIGNED xStart, SIGNED xEnd, SIGNED y,
                              COLORVAL cColor, SIGNED iWidth) = 0;
  virtual void VerticalLine(SIGNED yStart, SIGNED yEnd, SIGNED x,
                            COLORVAL cColor, SIGNED iWidth) = 0;
  virtual COLORVAL GetPixelView(SIGNED x, SIGNED y) = 0;

public:
  // --- Key functions we WILL implement in the base class ---
  virtual void DrawText(PegThing *Caller, PegPoint Where, const TCHAR *Text,
                        PegColor &Color, PegFont *Font, SIGNED Count = -1);
  virtual SIGNED TextWidth(const TCHAR *Text, PegFont *Font, SIGNED iLen = -1);
  virtual SIGNED TextHeight(const TCHAR *Text, PegFont *Font);

  void Invalidate(const PegRect &Rect);

#ifdef PEG_FULL_CLIPPING
  void GenerateViewportList(PegThing *pStart);
  void FreeViewports(PegThing *pStart);
#endif

protected:
  // Helper to check if a rectangle is valid for drawing
  bool ClipRect(PegRect &Rect, PegRect &mClip) {
    Rect &= mClip;
    if (Rect.wLeft > Rect.wRight || Rect.wBottom < Rect.wTop) {
      return false;
    }
    return true;
  }

#ifdef PEG_FULL_CLIPPING

  void SplitView(PegThing *pTarget, PegRect Top, PegRect Bottom);
  void SplitView(PegThing *pTarget, PegThing *Child, PegRect Under);
  void AddViewport(PegThing *pTarget, PegRect NewRect);
  void AllocateViewportBlock();
  Viewport *GetFreeViewport(void);
  Viewport *mpFreeListStart;
  Viewport *mpFreeListEnd;

#endif

  PegRect mInvalid;
  PegRect mVirtualRect;
  SIGNED mwHRes;
  SIGNED mwVRes;
  WORD mwTotalViewports;
  WORD mwDrawNesting;
  SIGNED miInvalidCount;
  WORD **mpScanPointers; // Will point to our frame buffer
};

// This function will be defined in l16screen.cpp
PegScreen *CreatePegScreen(void);

#endif