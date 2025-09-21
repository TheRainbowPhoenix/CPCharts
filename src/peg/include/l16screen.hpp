#ifndef _L16SCREEN_
#define _L16SCREEN_

// Define our screen dimensions
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240 // 528
#define FRAME_BUFFER_BASE ((uint16_t *)0x8c000000)
#define FRAME_BUFFER_PITCH 320 // Pitch in pixels (0x280 / 2)

class L16Screen : public PegScreen {
public:
  L16Screen(const PegRect &);
  virtual ~L16Screen();

  // --- Implementation of PegScreen's Pure Virtual Functions ---
  void BeginDraw(PegThing *);
  void EndDraw();
  void SetPointerType(UCHAR bType);
  void SetPalette(SIGNED iFirst, SIGNED iNum, const UCHAR *pGet);

protected:
  void DrawTextView(PegPoint Where, const TCHAR *Text, PegColor &Color,
                    PegFont *pFont, SIGNED iLen, PegRect &View);
  void LineView(SIGNED xStart, SIGNED yStart, SIGNED xEnd, SIGNED yEnd,
                PegRect &View, PegColor Color, SIGNED iWidth);
  void BitmapView(const PegPoint Where, const PegBitmap *pMap,
                  const PegRect &View);
  void RectMoveView(PegThing *Caller, const PegRect &View, const SIGNED xMove,
                    const SIGNED yMove);
  void HorizontalLine(SIGNED xStart, SIGNED xEnd, SIGNED y, COLORVAL cColor,
                      SIGNED iWidth);
  void VerticalLine(SIGNED yStart, SIGNED yEnd, SIGNED x, COLORVAL cColor,
                    SIGNED iWidth);
  COLORVAL GetPixelView(SIGNED x, SIGNED y);
};

#endif