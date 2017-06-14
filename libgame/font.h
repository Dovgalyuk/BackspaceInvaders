#ifndef FONT_H
#define FONT_H

#include "libgame.h"
#include "binary.h"

#define FONT_WIDTH 5
#define FONT_HEIGHT 7

// windows-1251 encoded font
static const unsigned char font_data[] DATA = {
    // 0x00 (N/A)
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,

    // 0x01 (N/A)
    B01110,
    B11111,
    B10101,
    B11111,
    B11011,
    B10001,
    B01110,

    // 0x02 (N/A)
    B01110,
    B11111,
    B10101,
    B11111,
    B10001,
    B11011,
    B01110,

    // 0x03 (N/A)
    B00000,
    B01010,
    B11111,
    B11111,
    B11111,
    B01110,
    B00100,

    // 0x04 (N/A)
    B00000,
    B00100,
    B01110,
    B11111,
    B11111,
    B01110,
    B00100,

    // 0x05 (N/A)
    B01110,
    B01010,
    B11111,
    B10101,
    B11111,
    B00100,
    B01110,

    // 0x06 (N/A)
    B00100,
    B01110,
    B11111,
    B11111,
    B11111,
    B00100,
    B01110,

    // 0x07 (N/A)
    B00000,
    B00000,
    B00100,
    B01110,
    B01110,
    B00100,
    B00000,

    // 0x08 (N/A)
    B11111,
    B11111,
    B11011,
    B10001,
    B10001,
    B11011,
    B11111,

    // 0x09 (N/A)
    B00000,
    B00000,
    B00100,
    B01010,
    B01010,
    B00100,
    B00000,

    // 0x0a (N/A)
    B11111,
    B11111,
    B11011,
    B10101,
    B10101,
    B11011,
    B11111,

    // 0x0b (N/A)
    B00000,
    B00111,
    B00011,
    B01101,
    B10100,
    B10100,
    B01000,

    // 0x0c (N/A)
    B01110,
    B10001,
    B10001,
    B01110,
    B00100,
    B11111,
    B00100,

    // 0x0d (N/A)
    B01111,
    B01001,
    B01111,
    B01000,
    B01000,
    B01000,
    B11000,

    // 0x0e (N/A)
    B01111,
    B01001,
    B01111,
    B01001,
    B01001,
    B01011,
    B11000,

    // 0x0f (N/A)
    B00100,
    B10101,
    B01110,
    B11011,
    B11011,
    B01110,
    B10101,

    // 0x10 (N/A)
    B10000,
    B11000,
    B11110,
    B11111,
    B11110,
    B11000,
    B10000,

    // 0x11 (N/A)
    B00001,
    B00011,
    B01111,
    B11111,
    B01111,
    B00011,
    B00001,

    // 0x12 (N/A)
    B00100,
    B01110,
    B10101,
    B00100,
    B10101,
    B01110,
    B00100,

    // 0x13 (N/A)
    B11011,
    B11011,
    B11011,
    B11011,
    B11011,
    B00000,
    B11011,

    // 0x14 (N/A)
    B01111,
    B10101,
    B10101,
    B01101,
    B00101,
    B00101,
    B00101,

    // 0x15 (N/A)
    B00110,
    B01001,
    B01010,
    B00101,
    B00010,
    B01001,
    B01001,

    // 0x16 (N/A)
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B11111,

    // 0x17 (N/A)
    B00100,
    B01110,
    B10101,
    B00100,
    B10101,
    B01110,
    B00100,

    // 0x18 (N/A)
    B00000,
    B00100,
    B01110,
    B10101,
    B00100,
    B00100,
    B00100,

    // 0x19 (N/A)
    B00000,
    B00100,
    B00100,
    B00100,
    B10101,
    B01110,
    B00100,

    // 0x1a (N/A)
    B00000,
    B00100,
    B00010,
    B11111,
    B00010,
    B00100,
    B00000,

    // 0x1b (N/A)
    B00000,
    B00100,
    B01000,
    B11111,
    B01000,
    B00100,
    B00000,

    // 0x1c (N/A)
    B00000,
    B10000,
    B10000,
    B10000,
    B11111,
    B00000,
    B00000,

    // 0x1d (N/A)
    B00000,
    B01010,
    B11111,
    B11111,
    B01010,
    B00000,
    B00000,

    // 0x1e (N/A)
    B00000,
    B00100,
    B00100,
    B01110,
    B11111,
    B11111,
    B00000,

    // 0x1f (N/A)
    B00000,
    B11111,
    B11111,
    B01110,
    B00100,
    B00100,
    B00000,

    // 0x20 ( )
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,

    // 0x21 (!)
    B00100,
    B00100,
    B00100,
    B00100,
    B00100,
    B00000,
    B00100,

    // 0x22 (")
    B01010,
    B01010,
    B01010,
    B00000,
    B00000,
    B00000,
    B00000,

    // 0x23 (#)
    B01010,
    B01010,
    B11111,
    B01010,
    B11111,
    B01010,
    B01010,

    // 0x24 ($)
    B00100,
    B01111,
    B10100,
    B01110,
    B00101,
    B11110,
    B00100,

    // 0x25 (%)
    B11000,
    B11001,
    B00010,
    B00100,
    B01000,
    B10011,
    B00011,

    // 0x26 (&)
    B01000,
    B10100,
    B10100,
    B01000,
    B10101,
    B10010,
    B01101,

    // 0x27 (')
    B00110,
    B00110,
    B00100,
    B01000,
    B00000,
    B00000,
    B00000,

    // 0x28 (()
    B00010,
    B00100,
    B01000,
    B01000,
    B01000,
    B00100,
    B00010,

    // 0x29 ())
    B01000,
    B00100,
    B00010,
    B00010,
    B00010,
    B00100,
    B01000,

    // 0x2a (*)
    B00100,
    B10101,
    B01110,
    B11111,
    B01110,
    B10101,
    B00100,

    // 0x2b (+)
    B00000,
    B00100,
    B00100,
    B11111,
    B00100,
    B00100,
    B00000,

    // 0x2c (,)
    B00000,
    B00000,
    B00000,
    B00000,
    B00110,
    B00110,
    B00100,

    // 0x2d (-)
    B00000,
    B00000,
    B00000,
    B11111,
    B00000,
    B00000,
    B00000,

    // 0x2e (.)
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00110,
    B00110,

    // 0x2f (/)
    B00000,
    B00001,
    B00010,
    B00100,
    B01000,
    B10000,
    B00000,

    // 0x30 (0)
    B01110,
    B10001,
    B10011,
    B10101,
    B11001,
    B10001,
    B01110,

    // 0x31 (1)
    B00100,
    B01100,
    B00100,
    B00100,
    B00100,
    B00100,
    B01110,

    // 0x32 (2)
    B01110,
    B10001,
    B00001,
    B01110,
    B10000,
    B10000,
    B11111,

    // 0x33 (3)
    B11111,
    B00001,
    B00010,
    B00110,
    B00001,
    B10001,
    B01110,

    // 0x34 (4)
    B00010,
    B00110,
    B01010,
    B10010,
    B11111,
    B00010,
    B00010,

    // 0x35 (5)
    B11111,
    B10000,
    B11110,
    B00001,
    B00001,
    B10001,
    B01110,

    // 0x36 (6)
    B00111,
    B01000,
    B10000,
    B11110,
    B10001,
    B10001,
    B01110,

    // 0x37 (7)
    B11111,
    B00001,
    B00001,
    B00010,
    B00100,
    B01000,
    B10000,

    // 0x38 (8)
    B01110,
    B10001,
    B10001,
    B01110,
    B10001,
    B10001,
    B01110,

    // 0x39 (9)
    B01110,
    B10001,
    B10001,
    B01111,
    B00001,
    B00010,
    B11100,

    // 0x3a (:)
    B00000,
    B00000,
    B00100,
    B00000,
    B00100,
    B00000,
    B00000,

    // 0x3b (;)
    B00000,
    B00000,
    B00100,
    B00000,
    B00100,
    B00100,
    B01000,

    // 0x3c (<)
    B00001,
    B00010,
    B00100,
    B01000,
    B00100,
    B00010,
    B00001,

    // 0x3d (=)
    B00000,
    B00000,
    B11111,
    B00000,
    B11111,
    B00000,
    B00000,

    // 0x3e (>)
    B01000,
    B00100,
    B00010,
    B00001,
    B00010,
    B00100,
    B01000,

    // 0x3f (?)
    B01110,
    B10001,
    B00001,
    B00110,
    B00100,
    B00000,
    B00100,

    // 0x40 (@)
    B01110,
    B10001,
    B10101,
    B10111,
    B10110,
    B10000,
    B01111,

    // 0x41 (A)
    B00100,
    B01010,
    B10001,
    B10001,
    B11111,
    B10001,
    B10001,

    // 0x42 (B)
    B11110,
    B10001,
    B10001,
    B11110,
    B10001,
    B10001,
    B11110,

    // 0x43 (C)
    B01110,
    B10001,
    B10000,
    B10000,
    B10000,
    B10001,
    B01110,

    // 0x44 (D)
    B11110,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B11110,

    // 0x45 (E)
    B11111,
    B10000,
    B10000,
    B11110,
    B10000,
    B10000,
    B11111,

    // 0x46 (F)
    B11111,
    B10000,
    B10000,
    B11110,
    B10000,
    B10000,
    B10000,

    // 0x47 (G)
    B01111,
    B10001,
    B10000,
    B10000,
    B10011,
    B10001,
    B01111,

    // 0x48 (H)
    B10001,
    B10001,
    B10001,
    B11111,
    B10001,
    B10001,
    B10001,

    // 0x49 (I)
    B01110,
    B00100,
    B00100,
    B00100,
    B00100,
    B00100,
    B01110,

    // 0x4a (J)
    B00111,
    B00010,
    B00010,
    B00010,
    B00010,
    B10010,
    B01100,

    // 0x4b (K)
    B10001,
    B10010,
    B10100,
    B11000,
    B10100,
    B10010,
    B10001,

    // 0x4c (L)
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B11111,

    // 0x4d (M)
    B10001,
    B11011,
    B10101,
    B10101,
    B10101,
    B10001,
    B10001,

    // 0x4e (N)
    B10001,
    B10001,
    B11001,
    B10101,
    B10011,
    B10001,
    B10001,

    // 0x4f (O)
    B01110,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B01110,

    // 0x50 (P)
    B11110,
    B10001,
    B10001,
    B11110,
    B10000,
    B10000,
    B10000,

    // 0x51 (Q)
    B01110,
    B10001,
    B10001,
    B10001,
    B10101,
    B10010,
    B01101,

    // 0x52 (R)
    B11110,
    B10001,
    B10001,
    B11110,
    B10100,
    B10010,
    B10001,

    // 0x53 (S)
    B01110,
    B10001,
    B10000,
    B01110,
    B00001,
    B10001,
    B01110,

    // 0x54 (T)
    B11111,
    B10101,
    B00100,
    B00100,
    B00100,
    B00100,
    B00100,

    // 0x55 (U)
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B01110,

    // 0x56 (V)
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B01010,
    B00100,

    // 0x57 (W)
    B10001,
    B10001,
    B10001,
    B10101,
    B10101,
    B10101,
    B01010,

    // 0x58 (X)
    B10001,
    B10001,
    B01010,
    B00100,
    B01010,
    B10001,
    B10001,

    // 0x59 (Y)
    B10001,
    B10001,
    B01010,
    B00100,
    B00100,
    B00100,
    B00100,

    // 0x5a (Z)
    B11111,
    B00001,
    B00010,
    B01110,
    B01000,
    B10000,
    B11111,

    // 0x5b ([)
    B01111,
    B01000,
    B01000,
    B01000,
    B01000,
    B01000,
    B01111,

    // 0x5c (\)
    B00000,
    B10000,
    B01000,
    B00100,
    B00010,
    B00001,
    B00000,

    // 0x5d (])
    B01111,
    B00001,
    B00001,
    B00001,
    B00001,
    B00001,
    B01111,

    // 0x5e (^)
    B00100,
    B01010,
    B10001,
    B00000,
    B00000,
    B00000,
    B00000,

    // 0x5f (_)
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,

    // 0x60 (`)
    B01100,
    B01100,
    B00100,
    B00010,
    B00000,
    B00000,
    B00000,

    // 0x61 (a)
    B00000,
    B00000,
    B01100,
    B00010,
    B01110,
    B10010,
    B01111,

    // 0x62 (b)
    B10000,
    B10000,
    B10110,
    B11001,
    B10001,
    B11001,
    B10110,

    // 0x63 (c)
    B00000,
    B00000,
    B01110,
    B10001,
    B10000,
    B10001,
    B01110,

    // 0x64 (d)
    B00001,
    B00001,
    B01101,
    B10011,
    B10001,
    B10011,
    B01101,

    // 0x65 (e)
    B00000,
    B00000,
    B01110,
    B10001,
    B11111,
    B10000,
    B01110,

    // 0x66 (f)
    B00010,
    B00101,
    B00100,
    B01110,
    B00100,
    B00100,
    B00100,

    // 0x67 (g)
    B01111,
    B10001,
    B10011,
    B01101,
    B00001,
    B00001,
    B01110,

    // 0x68 (h)
    B10000,
    B10000,
    B10110,
    B11001,
    B10001,
    B10001,
    B10001,

    // 0x69 (i)
    B00100,
    B00000,
    B01100,
    B00100,
    B00100,
    B00100,
    B01110,

    // 0x6a (j)
    B00010,
    B00000,
    B00010,
    B00010,
    B00010,
    B10010,
    B01100,

    // 0x6b (k)
    B10000,
    B10000,
    B10010,
    B10100,
    B11000,
    B10100,
    B10010,

    // 0x6c (l)
    B01100,
    B00100,
    B00100,
    B00100,
    B00100,
    B00100,
    B01110,

    // 0x6d (m)
    B00000,
    B00000,
    B11010,
    B10101,
    B10101,
    B10101,
    B10101,

    // 0x6e (n)
    B00000,
    B00000,
    B10110,
    B11001,
    B10001,
    B10001,
    B10001,

    // 0x6f (o)
    B00000,
    B00000,
    B01110,
    B10001,
    B10001,
    B10001,
    B01110,

    // 0x70 (p)
    B00000,
    B00000,
    B10110,
    B11001,
    B11001,
    B10110,
    B10000,

    // 0x71 (q)
    B00000,
    B00000,
    B01101,
    B10011,
    B10011,
    B01101,
    B00001,

    // 0x72 (r)
    B00000,
    B00000,
    B10110,
    B11001,
    B10000,
    B10000,
    B10000,

    // 0x73 (s)
    B00000,
    B00000,
    B01111,
    B10000,
    B01110,
    B00001,
    B11110,

    // 0x74 (t)
    B00100,
    B00100,
    B11111,
    B00100,
    B00100,
    B00101,
    B00010,

    // 0x75 (u)
    B00000,
    B00000,
    B10001,
    B10001,
    B10001,
    B10011,
    B01101,

    // 0x76 (v)
    B00000,
    B00000,
    B10001,
    B10001,
    B10001,
    B01010,
    B00100,

    // 0x77 (w)
    B00000,
    B00000,
    B10001,
    B10001,
    B10101,
    B10101,
    B01010,

    // 0x78 (x)
    B00000,
    B00000,
    B10001,
    B01010,
    B00100,
    B01010,
    B10001,

    // 0x79 (y)
    B00000,
    B00000,
    B10001,
    B10001,
    B01010,
    B00100,
    B11000,

    // 0x7a (z)
    B00000,
    B00000,
    B11111,
    B00010,
    B00100,
    B01000,
    B11111,

    // 0x7b ({)
    B00010,
    B00100,
    B00100,
    B01000,
    B00100,
    B00100,
    B00010,

    // 0x7c (|)
    B00100,
    B00100,
    B00100,
    B00000,
    B00100,
    B00100,
    B00100,

    // 0x7d (})
    B01000,
    B00100,
    B00100,
    B00010,
    B00100,
    B00100,
    B01000,

    // 0x7e (~)
    B01000,
    B10101,
    B00010,
    B00000,
    B00000,
    B00000,
    B00000,

    // 0x7f ()
    B00100,
    B01110,
    B11011,
    B10001,
    B10001,
    B11111,
    B00000,

    // 0x80 (Ђ)
    B11111,
    B01000,
    B01000,
    B01110,
    B01001,
    B01001,
    B01010,

    // 0x81 (Ѓ)
    B00010,
    B00100,
    B11111,
    B10000,
    B10000,
    B10000,
    B10000,

    // 0x82 (‚)
    B00000,
    B00000,
    B00000,
    B00000,
    B11000,
    B01000,
    B10000,

    // 0x83 (ѓ)
    B00010,
    B00100,
    B11111,
    B10000,
    B10000,
    B10000,
    B10000,

    // 0x84 („)
    B00000,
    B00000,
    B00000,
    B00000,
    B11011,
    B01001,
    B10010,

    // 0x85 (…)
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B10101,

    // 0x86 (†)
    B01000,
    B01000,
    B11100,
    B01000,
    B01000,
    B01000,
    B01000,

    // 0x87 (‡)
    B00100,
    B00100,
    B11111,
    B00100,
    B11111,
    B00100,
    B00100,

    // 0x88 (€)
    B00110,
    B01001,
    B11100,
    B01000,
    B11100,
    B01001,
    B00110,

    // 0x89 (‰)
    B01110,
    B10001,
    B00001,
    B00010,
    B00100,
    B00000,
    B00100,

    // 0x8a (Љ)
    B11100,
    B10100,
    B10100,
    B10110,
    B10101,
    B10101,
    B10110,

    // 0x8b (‹)
    B00000,
    B00000,
    B00100,
    B01000,
    B10000,
    B01000,
    B00100,

    // 0x8c (Њ)
    B10100,
    B10100,
    B10100,
    B11110,
    B10101,
    B10101,
    B10110,

    // 0x8d (Ќ)
    B00100,
    B01000,
    B10010,
    B10100,
    B11000,
    B10100,
    B10010,

    // 0x8e (Ћ)
    B11111,
    B00100,
    B00100,
    B00110,
    B00101,
    B00101,
    B00101,

    // 0x8f (Џ)
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111,
    B00100,

    // 0x90 (ђ)
    B01000,
    B11100,
    B01000,
    B01010,
    B01101,
    B01001,
    B00010,

    // 0x91 (‘)
    B01000,
    B10000,
    B11000,
    B00000,
    B00000,
    B00000,
    B00000,

    // 0x92 (’)
    B11000,
    B01000,
    B10000,
    B00000,
    B00000,
    B00000,
    B00000,

    // 0x93 (“)
    B01001,
    B10010,
    B11011,
    B00000,
    B00000,
    B00000,
    B00000,

    // 0x94 (”)
    B11011,
    B01001,
    B10010,
    B00000,
    B00000,
    B00000,
    B00000,

    // 0x95 (•)
    B00000,
    B00000,
    B00000,
    B00000,
    B11000,
    B11000,
    B00000,

    // 0x96 (–)
    B00000,
    B00000,
    B00000,
    B00000,
    B11100,
    B00000,
    B00000,

    // 0x97 (—)
    B00000,
    B00000,
    B00000,
    B11111,
    B00000,
    B00000,
    B00000,

    // 0x98 (N/A)
    B01110,
    B10001,
    B00001,
    B00010,
    B00100,
    B00000,
    B00100,

    // 0x99 (™)
    B01110,
    B10001,
    B00001,
    B00010,
    B00100,
    B00000,
    B00100,

    // 0x9a (љ)
    B00000,
    B00000,
    B11100,
    B10100,
    B10110,
    B10101,
    B10110,

    // 0x9b (›)
    B00000,
    B00000,
    B10000,
    B01000,
    B00100,
    B01000,
    B10000,

    // 0x9c (њ)
    B00000,
    B00000,
    B10100,
    B10100,
    B11110,
    B10101,
    B10110,

    // 0x9d (ќ)
    B00100,
    B01000,
    B10010,
    B10100,
    B11000,
    B10100,
    B10010,

    // 0x9e (ћ)
    B00000,
    B01000,
    B11100,
    B01000,
    B01110,
    B01001,
    B01001,

    // 0x9f (џ)
    B00000,
    B00000,
    B10001,
    B10001,
    B10001,
    B11111,
    B00100,

    // 0xa0 ( )
    B01110,
    B10001,
    B00001,
    B00010,
    B00100,
    B00000,
    B00100,

    // 0xa1 (Ў)
    B01010,
    B00100,
    B10001,
    B01010,
    B00100,
    B00100,
    B11000,

    // 0xa2 (ў)
    B01010,
    B00100,
    B10001,
    B10001,
    B01010,
    B00100,
    B11000,

    // 0xa3 (Ј)
    B00111,
    B00010,
    B00010,
    B00010,
    B00010,
    B10010,
    B01100,

    // 0xa4 (¤)
    B00000,
    B10001,
    B01110,
    B01010,
    B01110,
    B10001,
    B00000,

    // 0xa5 (Ґ)
    B01110,
    B10001,
    B00001,
    B00010,
    B00100,
    B00000,
    B00100,

    // 0xa6 (¦)
    B10000,
    B10000,
    B10000,
    B00000,
    B10000,
    B10000,
    B10000,

    // 0xa7 (§)
    B01111,
    B10000,
    B01110,
    B10001,
    B01110,
    B00001,
    B11110,

    // 0xa8 (Ё)
    B01010,
    B00000,
    B11111,
    B10000,
    B11100,
    B10000,
    B11111,

    // 0xa9 (©)
    B00000,
    B00000,
    B01100,
    B10000,
    B10000,
    B01100,
    B00000,

    // 0xaa (Є)
    B01110,
    B10000,
    B10000,
    B11100,
    B10000,
    B10000,
    B01110,

    // 0xab («)
    B00000,
    B00000,
    B01001,
    B10010,
    B01001,
    B00000,
    B00000,

    // 0xac (¬)
    B00000,
    B00000,
    B00000,
    B11111,
    B00001,
    B00001,
    B00000,

    // 0xad (­)
    B00000,
    B00000,
    B00000,
    B11111,
    B00000,
    B00000,
    B00000,

    // 0xae (®)
    B01110,
    B10001,
    B00001,
    B00010,
    B00100,
    B00000,
    B00100,

    // 0xaf (Ї)
    B01010,
    B00000,
    B11111,
    B00100,
    B00100,
    B00100,
    B11111,

    // 0xb0 (°)
    B11100,
    B10100,
    B11100,
    B00000,
    B00000,
    B00000,
    B00000,

    // 0xb1 (±)
    B00100,
    B00100,
    B11111,
    B00100,
    B00100,
    B00000,
    B11111,

    // 0xb2 (І)
    B11111,
    B00100,
    B00100,
    B00100,
    B00100,
    B00100,
    B11111,

    // 0xb3 (і)
    B01000,
    B00000,
    B11000,
    B01000,
    B01000,
    B01000,
    B11100,

    // 0xb4 (ґ)
    B01110,
    B10001,
    B00001,
    B00010,
    B00100,
    B00000,
    B00100,

    // 0xb5 (µ)
    B00000,
    B00000,
    B10010,
    B10010,
    B11101,
    B10000,
    B10000,

    // 0xb6 (¶)
    B01111,
    B11101,
    B01101,
    B00101,
    B00101,
    B00101,
    B00101,

    // 0xb7 (·)
    B00000,
    B00000,
    B00000,
    B10000,
    B00000,
    B00000,
    B00000,

    // 0xb8 (ё)
    B01010,
    B00000,
    B01110,
    B10001,
    B11110,
    B10000,
    B01110,

    // 0xb9 (№)
    B01110,
    B10001,
    B00001,
    B00010,
    B00100,
    B00000,
    B00100,

    // 0xba (є)
    B00000,
    B00000,
    B01110,
    B10001,
    B11100,
    B10001,
    B01110,

    // 0xbb (»)
    B00000,
    B00000,
    B10010,
    B01001,
    B10010,
    B00000,
    B00000,

    // 0xbc (ј)
    B00010,
    B00000,
    B00110,
    B00010,
    B00010,
    B10010,
    B01100,

    // 0xbd (Ѕ)
    B01110,
    B10000,
    B10000,
    B01110,
    B00001,
    B00001,
    B01110,

    // 0xbe (ѕ)
    B00000,
    B00000,
    B01110,
    B10000,
    B01110,
    B00001,
    B01110,

    // 0xbf (ї)
    B10100,
    B00000,
    B11000,
    B01000,
    B01000,
    B01000,
    B11100,

    // 0xc0 (А)
    B01110,
    B10001,
    B10001,
    B11111,
    B10001,
    B10001,
    B10001,

    // 0xc1 (Б)
    B11111,
    B10000,
    B10000,
    B11110,
    B10001,
    B10001,
    B11110,

    // 0xc2 (В)
    B11110,
    B10001,
    B10001,
    B11110,
    B10001,
    B10001,
    B11110,

    // 0xc3 (Г)
    B11111,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,

    // 0xc4 (Д)
    B01110,
    B01010,
    B01010,
    B01010,
    B01010,
    B11111,
    B10001,

    // 0xc5 (Е)
    B11111,
    B10000,
    B10000,
    B11110,
    B10000,
    B10000,
    B11111,

    // 0xc6 (Ж)
    B10101,
    B10101,
    B10101,
    B01110,
    B10101,
    B10101,
    B10101,

    // 0xc7 (З)
    B11100,
    B00010,
    B00010,
    B01100,
    B00010,
    B00010,
    B11100,

    // 0xc8 (И)
    B10001,
    B10001,
    B10011,
    B10101,
    B11001,
    B10001,
    B10001,

    // 0xc9 (Й)
    B01010,
    B00100,
    B10001,
    B10011,
    B10101,
    B11001,
    B10001,

    // 0xca (К)
    B10001,
    B10010,
    B10100,
    B11000,
    B10100,
    B10010,
    B10001,

    // 0xcb (Л)
    B01111,
    B01001,
    B01001,
    B01001,
    B01001,
    B01001,
    B10001,

    // 0xcc (М)
    B10001,
    B11011,
    B10101,
    B10001,
    B10001,
    B10001,
    B10001,

    // 0xcd (Н)
    B10001,
    B10001,
    B10001,
    B11111,
    B10001,
    B10001,
    B10001,

    // 0xce (О)
    B01110,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B01110,

    // 0xcf (П)
    B11111,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,

    // 0xd0 (Р)
    B11110,
    B10001,
    B10001,
    B11110,
    B10000,
    B10000,
    B10000,

    // 0xd1 (С)
    B01110,
    B10001,
    B10000,
    B10000,
    B10000,
    B10001,
    B01110,

    // 0xd2 (Т)
    B11111,
    B00100,
    B00100,
    B00100,
    B00100,
    B00100,
    B00100,

    // 0xd3 (У)
    B10001,
    B10001,
    B10001,
    B01010,
    B00100,
    B00100,
    B11000,

    // 0xd4 (Ф)
    B00100,
    B01110,
    B10101,
    B10101,
    B01110,
    B00100,
    B00100,

    // 0xd5 (Х)
    B10001,
    B10001,
    B01010,
    B00100,
    B01010,
    B10001,
    B10001,

    // 0xd6 (Ц)
    B10010,
    B10010,
    B10010,
    B10010,
    B10010,
    B11111,
    B00001,

    // 0xd7 (Ч)
    B10001,
    B10001,
    B10001,
    B01111,
    B00001,
    B00001,
    B00001,

    // 0xd8 (Ш)
    B10101,
    B10101,
    B10101,
    B10101,
    B10101,
    B10101,
    B11111,

    // 0xd9 (Щ)
    B10101,
    B10101,
    B10101,
    B10101,
    B10101,
    B11111,
    B00001,

    // 0xda (Ъ)
    B11000,
    B01000,
    B01000,
    B01110,
    B01001,
    B01001,
    B01110,

    // 0xdb (Ы)
    B10001,
    B10001,
    B10001,
    B11001,
    B10101,
    B10101,
    B11001,

    // 0xdc (Ь)
    B10000,
    B10000,
    B10000,
    B11100,
    B10010,
    B10010,
    B11100,

    // 0xdd (Э)
    B01100,
    B10010,
    B00001,
    B01111,
    B00001,
    B10010,
    B01100,

    // 0xde (Ю)
    B10010,
    B10101,
    B10101,
    B11101,
    B10101,
    B10101,
    B10010,

    // 0xdf (Я)
    B01111,
    B10001,
    B10001,
    B01111,
    B00101,
    B01001,
    B10001,

    // 0xe0 (а)
    B00000,
    B00000,
    B01110,
    B00001,
    B01111,
    B10001,
    B01111,

    // 0xe1 (б)
    B00000,
    B01110,
    B10000,
    B11110,
    B10001,
    B10001,
    B01110,

    // 0xe2 (в)
    B00000,
    B00000,
    B11110,
    B10001,
    B11110,
    B10001,
    B11110,

    // 0xe3 (г)
    B00000,
    B00000,
    B11111,
    B10000,
    B10000,
    B10000,
    B10000,

    // 0xe4 (д)
    B00000,
    B00000,
    B01110,
    B01010,
    B01010,
    B01111,
    B10001,

    // 0xe5 (е)
    B00000,
    B00000,
    B01110,
    B10001,
    B11111,
    B10000,
    B01110,

    // 0xe6 (ж)
    B00000,
    B00000,
    B10101,
    B10101,
    B01110,
    B10101,
    B10101,

    // 0xe7 (з)
    B00000,
    B00000,
    B01110,
    B10001,
    B00110,
    B10001,
    B01110,

    // 0xe8 (и)
    B00000,
    B00000,
    B10001,
    B10011,
    B10101,
    B11001,
    B10001,

    // 0xe9 (й)
    B01010,
    B00100,
    B10001,
    B10011,
    B10101,
    B11001,
    B10001,

    // 0xea (к)
    B00000,
    B00000,
    B10010,
    B10100,
    B11000,
    B10100,
    B10010,

    // 0xeb (л)
    B00000,
    B00000,
    B01111,
    B01001,
    B01001,
    B01001,
    B10001,

    // 0xec (м)
    B00000,
    B00000,
    B10001,
    B11011,
    B10101,
    B10001,
    B10001,

    // 0xed (н)
    B00000,
    B00000,
    B10001,
    B10001,
    B11111,
    B10001,
    B10001,

    // 0xee (о)
    B00000,
    B00000,
    B01110,
    B10001,
    B10001,
    B10001,
    B01110,

    // 0xef (п)
    B00000,
    B00000,
    B11111,
    B10001,
    B10001,
    B10001,
    B10001,

    // 0xf0 (р)
    B00000,
    B00000,
    B11110,
    B10001,
    B11110,
    B10000,
    B10000,

    // 0xf1 (с)
    B00000,
    B00000,
    B01110,
    B10000,
    B10000,
    B10001,
    B01110,

    // 0xf2 (т)
    B00000,
    B00000,
    B11111,
    B00100,
    B00100,
    B00100,
    B00100,

    // 0xf3 (у)
    B00000,
    B00000,
    B10001,
    B10001,
    B01010,
    B00100,
    B11000,

    // 0xf4 (ф)
    B00000,
    B00100,
    B01110,
    B10101,
    B10101,
    B01110,
    B00100,

    // 0xf5 (х)
    B00000,
    B00000,
    B10001,
    B01010,
    B00100,
    B01010,
    B10001,

    // 0xf6 (ц)
    B00000,
    B00000,
    B10010,
    B10010,
    B10010,
    B11111,
    B00001,

    // 0xf7 (ч)
    B00000,
    B00000,
    B10001,
    B10001,
    B01111,
    B00001,
    B00001,

    // 0xf8 (ш)
    B00000,
    B00000,
    B10101,
    B10101,
    B10101,
    B10101,
    B11111,

    // 0xf9 (щ)
    B00000,
    B00000,
    B10101,
    B10101,
    B10101,
    B11111,
    B00001,

    // 0xfa (ъ)
    B00000,
    B00000,
    B11000,
    B01000,
    B01100,
    B01010,
    B01100,

    // 0xfb (ы)
    B00000,
    B00000,
    B10001,
    B10001,
    B11001,
    B10101,
    B11001,

    // 0xfc (ь)
    B00000,
    B00000,
    B10000,
    B10000,
    B11000,
    B10100,
    B11000,

    // 0xfd (э)
    B00000,
    B00000,
    B01110,
    B10001,
    B00111,
    B10001,
    B01110,

    // 0xfe (ю)
    B00000,
    B00000,
    B10010,
    B10101,
    B11101,
    B10101,
    B10010,

    // 0xff (я)
    B00000,
    B00000,
    B01111,
    B10001,
    B01111,
    B01001,
    B10001
};

#endif
