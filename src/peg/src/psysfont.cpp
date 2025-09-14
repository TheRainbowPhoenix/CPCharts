#include "../include/peg.hpp"

const UCHAR Latin_Font_9_Data[] = {
    // Char 'A' (65) - 5 pixels wide
    0x10,
    0x28,
    0x28,
    0x44,
    0x44,
    0x7C,
    0x44,
    0x44,
    0x00,
    // Char 'B' (66) - 5 pixels wide
    0x78,
    0x44,
    0x44,
    0x78,
    0x44,
    0x44,
    0x78,
    0x00,
    0x00,
    // Char 'C' (67) - 5 pixels wide
    0x38,
    0x44,
    0x40,
    0x40,
    0x40,
    0x44,
    0x38,
    0x00,
    0x00,
};

// =========================================================================
// THIS IS THE DEFINITION. It should ONLY be here.
// =========================================================================
PegFont Latin_Font_9 = {
    0,                         // uType: 0 for fixed-width.
    7,                         // uAscent
    2,                         // uDescent
    9,                         // uHeight
    1,                         // wBytesPerLine
    'A',                       // wFirstChar
    'C',                       // wLastChar
    NULL,                      // pOffsets (NULL for fixed-width)
    NULL,                      // pNext
    (UCHAR *)Latin_Font_9_Data // pData
};