/*******************************************************************************
 * Size: 16 px
 * Bpp: 4
 * Opts: 
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef FONT_EN_16
#define FONT_EN_16 1
#endif

#if FONT_EN_16

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0021 "!" */
    0xbd, 0xbd, 0xbd, 0xad, 0xac, 0x8b, 0x79, 0x68,
    0x57, 0x0, 0xbd, 0xdf,

    /* U+0022 "\"" */
    0x4d, 0xf, 0x14, 0xd0, 0xf1, 0x4d, 0xf, 0x14,
    0xd0, 0xf1, 0x15, 0x6, 0x0,

    /* U+0023 "#" */
    0x0, 0xd, 0x30, 0xc5, 0x0, 0x0, 0xf2, 0xe,
    0x30, 0x0, 0x1f, 0x0, 0xf1, 0x0, 0x3, 0xe0,
    0x1f, 0x0, 0xf, 0xff, 0xff, 0xff, 0x50, 0x29,
    0xb2, 0x7c, 0x20, 0x0, 0xa7, 0x9, 0x80, 0x7,
    0xff, 0xff, 0xff, 0xe0, 0x12, 0xf4, 0x2e, 0x52,
    0x0, 0x1f, 0x0, 0xf1, 0x0, 0x3, 0xe0, 0x1f,
    0x0, 0x0, 0x5c, 0x3, 0xe0, 0x0,

    /* U+0024 "$" */
    0x0, 0x0, 0xe0, 0x0, 0x0, 0x9, 0xef, 0xd5,
    0x0, 0xa, 0xe7, 0xe9, 0xf4, 0x1, 0xf5, 0xe,
    0x9, 0xc0, 0x3f, 0x30, 0xe0, 0x14, 0x1, 0xfa,
    0xe, 0x0, 0x0, 0x8, 0xfe, 0xf5, 0x0, 0x0,
    0x4, 0xaf, 0xfe, 0x30, 0x0, 0x0, 0xe4, 0xde,
    0x13, 0x40, 0xe, 0x2, 0xf5, 0x6e, 0x0, 0xe0,
    0x1f, 0x41, 0xf8, 0xe, 0x8, 0xf1, 0x6, 0xfe,
    0xfe, 0xf5, 0x0, 0x2, 0x7f, 0x72, 0x0, 0x0,
    0x0, 0xe0, 0x0, 0x0,

    /* U+0025 "%" */
    0x6, 0xee, 0x80, 0x0, 0x0, 0xb9, 0x0, 0x2f,
    0x21, 0xe4, 0x0, 0x7, 0xd0, 0x0, 0x6c, 0x0,
    0xa8, 0x0, 0x3f, 0x30, 0x0, 0x8b, 0x0, 0x99,
    0x0, 0xd7, 0x0, 0x0, 0x6c, 0x0, 0xa8, 0x9,
    0xb0, 0x0, 0x0, 0x2f, 0x22, 0xf2, 0x5e, 0x17,
    0xdd, 0x60, 0x6, 0xdd, 0x61, 0xe5, 0x3f, 0x23,
    0xf2, 0x0, 0x0, 0xc, 0x90, 0x8a, 0x0, 0xc6,
    0x0, 0x0, 0x7d, 0x0, 0x99, 0x0, 0xb8, 0x0,
    0x3, 0xf3, 0x0, 0x8a, 0x0, 0xc6, 0x0, 0xd,
    0x70, 0x0, 0x3e, 0x12, 0xf1, 0x0, 0xab, 0x0,
    0x0, 0x7, 0xee, 0x50,

    /* U+0026 "&" */
    0x0, 0x2c, 0xfd, 0x40, 0x0, 0x0, 0xd, 0xc5,
    0xaf, 0x10, 0x0, 0x0, 0xf5, 0x1, 0xf3, 0x0,
    0x0, 0xc, 0x90, 0x6e, 0x0, 0x0, 0x0, 0x3f,
    0x8e, 0x40, 0x0, 0x0, 0x7, 0xff, 0x60, 0x0,
    0x0, 0xa, 0xe4, 0x9e, 0x27, 0xc0, 0x3, 0xf4,
    0x0, 0xcc, 0xba, 0x0, 0x6f, 0x0, 0x1, 0xef,
    0x40, 0x5, 0xf3, 0x0, 0xa, 0xf6, 0x0, 0xe,
    0xe6, 0x5a, 0xfa, 0xf3, 0x0, 0x2b, 0xff, 0xb3,
    0xc, 0xd1,

    /* U+0027 "'" */
    0x5c, 0x5c, 0x5c, 0x5c, 0x14,

    /* U+0028 "(" */
    0x0, 0x1e, 0x10, 0xa, 0x80, 0x2, 0xf1, 0x0,
    0x8b, 0x0, 0xd, 0x80, 0x1, 0xf5, 0x0, 0x2f,
    0x30, 0x3, 0xf3, 0x0, 0x2f, 0x40, 0x0, 0xf5,
    0x0, 0xc, 0x80, 0x0, 0x8c, 0x0, 0x1, 0xf1,
    0x0, 0x9, 0x80, 0x0, 0x1e, 0x10,

    /* U+0029 ")" */
    0xd, 0x20, 0x0, 0x6b, 0x0, 0x0, 0xf3, 0x0,
    0xa, 0xa0, 0x0, 0x6e, 0x0, 0x3, 0xf3, 0x0,
    0x1f, 0x40, 0x1, 0xf5, 0x0, 0x1f, 0x40, 0x3,
    0xf3, 0x0, 0x6f, 0x0, 0x9, 0xb0, 0x0, 0xe4,
    0x0, 0x5c, 0x0, 0xd, 0x30, 0x0,

    /* U+002A "*" */
    0x0, 0xa3, 0x0, 0x56, 0xa6, 0x91, 0x17, 0xfd,
    0x50, 0x7, 0x9d, 0x10, 0x7, 0x4, 0x30,

    /* U+002B "+" */
    0x0, 0x0, 0xc5, 0x0, 0x0, 0x0, 0xc, 0x50,
    0x0, 0x0, 0x0, 0xc5, 0x0, 0x0, 0x0, 0xc,
    0x50, 0x0, 0x3f, 0xff, 0xff, 0xff, 0xd0, 0x22,
    0x2c, 0x72, 0x22, 0x0, 0x0, 0xc5, 0x0, 0x0,
    0x0, 0xc, 0x50, 0x0, 0x0, 0x0, 0xc5, 0x0,
    0x0,

    /* U+002C "," */
    0x9e, 0x1a, 0xf1, 0xc, 0x6, 0xa0, 0x30, 0x0,

    /* U+002D "-" */
    0x1, 0x11, 0x10, 0x3f, 0xff, 0xf6, 0x4, 0x44,
    0x41,

    /* U+002E "." */
    0x9e, 0x1a, 0xf1,

    /* U+002F "/" */
    0x0, 0x0, 0xb, 0x60, 0x0, 0x2, 0xf0, 0x0,
    0x0, 0x89, 0x0, 0x0, 0xe, 0x30, 0x0, 0x5,
    0xd0, 0x0, 0x0, 0xb7, 0x0, 0x0, 0x1f, 0x10,
    0x0, 0x8, 0xa0, 0x0, 0x0, 0xe4, 0x0, 0x0,
    0x4d, 0x0, 0x0, 0xb, 0x70, 0x0, 0x1, 0xf1,
    0x0, 0x0,

    /* U+0030 "0" */
    0x0, 0x7d, 0xfd, 0x60, 0x0, 0x7f, 0x95, 0xaf,
    0x50, 0xe, 0xa0, 0x0, 0xbc, 0x2, 0xf4, 0x0,
    0x6, 0xf0, 0x4f, 0x20, 0x0, 0x4f, 0x25, 0xf1,
    0x0, 0x3, 0xf3, 0x5f, 0x10, 0x0, 0x3f, 0x34,
    0xf2, 0x0, 0x4, 0xf2, 0x2f, 0x40, 0x0, 0x6f,
    0x0, 0xea, 0x0, 0xb, 0xd0, 0x7, 0xf8, 0x59,
    0xf6, 0x0, 0x7, 0xdf, 0xd6, 0x0,

    /* U+0031 "1" */
    0x0, 0x8, 0xb0, 0x17, 0xfb, 0x9f, 0xff, 0xb1,
    0x22, 0xbb, 0x0, 0xa, 0xb0, 0x0, 0xab, 0x0,
    0xa, 0xb0, 0x0, 0xab, 0x0, 0xa, 0xb0, 0x0,
    0xab, 0x0, 0xa, 0xb0, 0x0, 0xab,

    /* U+0032 "2" */
    0x0, 0x8e, 0xfc, 0x50, 0x9, 0xf7, 0x5a, 0xf5,
    0x1f, 0x70, 0x0, 0xcc, 0x4f, 0x20, 0x0, 0x9d,
    0x14, 0x0, 0x0, 0xbb, 0x0, 0x0, 0x6, 0xf4,
    0x0, 0x0, 0x8f, 0x70, 0x0, 0x3d, 0xe5, 0x0,
    0x3, 0xfa, 0x0, 0x0, 0xd, 0x90, 0x0, 0x0,
    0x5f, 0x54, 0x44, 0x43, 0x8f, 0xff, 0xff, 0xfc,

    /* U+0033 "3" */
    0x0, 0x9e, 0xfc, 0x40, 0x0, 0x9f, 0x75, 0xbf,
    0x30, 0xf, 0x60, 0x0, 0xe9, 0x2, 0xc1, 0x0,
    0xd, 0x90, 0x0, 0x0, 0x7, 0xf3, 0x0, 0x0,
    0x8f, 0xf8, 0x0, 0x0, 0x1, 0x37, 0xf9, 0x0,
    0x0, 0x0, 0x8, 0xf0, 0x6c, 0x0, 0x0, 0x5f,
    0x25, 0xf3, 0x0, 0x9, 0xf0, 0xd, 0xe6, 0x59,
    0xf7, 0x0, 0x1a, 0xef, 0xc5, 0x0,

    /* U+0034 "4" */
    0x0, 0x0, 0xb, 0xb0, 0x0, 0x0, 0x6, 0xfb,
    0x0, 0x0, 0x1, 0xeb, 0xb0, 0x0, 0x0, 0xb7,
    0x9b, 0x0, 0x0, 0x5c, 0x9, 0xb0, 0x0, 0x1e,
    0x30, 0x9b, 0x0, 0xa, 0x80, 0x9, 0xb0, 0x5,
    0xe1, 0x0, 0x9b, 0x0, 0x8f, 0xff, 0xff, 0xff,
    0x31, 0x33, 0x33, 0xac, 0x30, 0x0, 0x0, 0x9,
    0xb0, 0x0, 0x0, 0x0, 0x9b, 0x0,

    /* U+0035 "5" */
    0x2, 0xff, 0xff, 0xf8, 0x0, 0x5e, 0x44, 0x44,
    0x20, 0x8, 0xb0, 0x0, 0x0, 0x0, 0xa8, 0x0,
    0x0, 0x0, 0xd, 0xad, 0xfe, 0x60, 0x0, 0xfd,
    0x54, 0xaf, 0x70, 0x5, 0x0, 0x0, 0xbe, 0x0,
    0x0, 0x0, 0x5, 0xf1, 0x26, 0x0, 0x0, 0x6f,
    0x14, 0xf3, 0x0, 0xb, 0xd0, 0xc, 0xe7, 0x5a,
    0xf5, 0x0, 0x19, 0xef, 0xc5, 0x0,

    /* U+0036 "6" */
    0x0, 0x4c, 0xfe, 0x80, 0x0, 0x3f, 0x95, 0x7f,
    0x80, 0xb, 0xa0, 0x0, 0x7c, 0x0, 0xf3, 0x0,
    0x0, 0x0, 0x3f, 0x3c, 0xff, 0x90, 0x5, 0xfe,
    0x84, 0x7f, 0x90, 0x6f, 0x90, 0x0, 0x7f, 0x15,
    0xf4, 0x0, 0x3, 0xf3, 0x3f, 0x40, 0x0, 0x3f,
    0x30, 0xe9, 0x0, 0x8, 0xf0, 0x6, 0xf9, 0x58,
    0xf8, 0x0, 0x7, 0xdf, 0xd7, 0x0,

    /* U+0037 "7" */
    0x3f, 0xff, 0xff, 0xff, 0x21, 0x66, 0x66, 0x6a,
    0xd0, 0x0, 0x0, 0x2, 0xe2, 0x0, 0x0, 0x0,
    0xc7, 0x0, 0x0, 0x0, 0x6e, 0x0, 0x0, 0x0,
    0xe, 0x70, 0x0, 0x0, 0x6, 0xf1, 0x0, 0x0,
    0x0, 0xbb, 0x0, 0x0, 0x0, 0x1f, 0x60, 0x0,
    0x0, 0x5, 0xf2, 0x0, 0x0, 0x0, 0x8f, 0x0,
    0x0, 0x0, 0xa, 0xd0, 0x0, 0x0,

    /* U+0038 "8" */
    0x0, 0x7d, 0xfd, 0x60, 0x0, 0x7f, 0x85, 0x9f,
    0x50, 0xd, 0x90, 0x0, 0xcb, 0x0, 0xe8, 0x0,
    0xb, 0xb0, 0x7, 0xe4, 0x15, 0xf5, 0x0, 0xc,
    0xff, 0xfa, 0x0, 0xb, 0xf7, 0x47, 0xfa, 0x3,
    0xf5, 0x0, 0x7, 0xf1, 0x5f, 0x10, 0x0, 0x3f,
    0x33, 0xf5, 0x0, 0x7, 0xf1, 0xc, 0xf7, 0x58,
    0xf9, 0x0, 0x8, 0xdf, 0xd7, 0x0,

    /* U+0039 "9" */
    0x0, 0x9d, 0xfc, 0x40, 0x0, 0xce, 0x65, 0xaf,
    0x30, 0x4f, 0x40, 0x0, 0xcb, 0x6, 0xf0, 0x0,
    0x8, 0xf0, 0x4f, 0x20, 0x0, 0xaf, 0x10, 0xec,
    0x31, 0x7e, 0xf2, 0x3, 0xdf, 0xfd, 0x6f, 0x10,
    0x0, 0x22, 0x6, 0xf0, 0x16, 0x10, 0x0, 0x9e,
    0x0, 0xf6, 0x0, 0xe, 0x90, 0x9, 0xf7, 0x5c,
    0xf2, 0x0, 0x8, 0xef, 0xc3, 0x0,

    /* U+003A ":" */
    0xaf, 0x19, 0xe1, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x9, 0xe1, 0xaf, 0x10,

    /* U+003B ";" */
    0xaf, 0x19, 0xe1, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x9, 0xe1, 0xaf, 0x10, 0xc0, 0x6a,
    0x3, 0x0,

    /* U+003C "<" */
    0x0, 0x0, 0x0, 0x1, 0x70, 0x0, 0x0, 0x29,
    0xf8, 0x0, 0x2, 0xaf, 0x91, 0x0, 0x3a, 0xe8,
    0x10, 0x0, 0x3f, 0x91, 0x0, 0x0, 0x1, 0x9f,
    0x92, 0x0, 0x0, 0x0, 0x18, 0xfa, 0x20, 0x0,
    0x0, 0x1, 0x8e, 0xa3, 0x0, 0x0, 0x0, 0x7,
    0xc0, 0x0, 0x0, 0x0, 0x0,

    /* U+003D "=" */
    0x3f, 0xff, 0xff, 0xff, 0xd0, 0x22, 0x22, 0x22,
    0x22, 0x0, 0x0, 0x0, 0x0, 0x3, 0xff, 0xff,
    0xff, 0xfd, 0x2, 0x22, 0x22, 0x22, 0x20,

    /* U+003E ">" */
    0x25, 0x0, 0x0, 0x0, 0x2, 0xcd, 0x60, 0x0,
    0x0, 0x0, 0x4c, 0xe7, 0x0, 0x0, 0x0, 0x3,
    0xbe, 0x70, 0x0, 0x0, 0x0, 0x3d, 0xd0, 0x0,
    0x0, 0x5c, 0xd5, 0x0, 0x5, 0xdd, 0x50, 0x0,
    0x6d, 0xc5, 0x0, 0x0, 0x4c, 0x40, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0,

    /* U+003F "?" */
    0x0, 0x6d, 0xfd, 0x50, 0x6, 0xf8, 0x59, 0xf5,
    0xe, 0x90, 0x0, 0xcb, 0x1f, 0x40, 0x0, 0xac,
    0x0, 0x0, 0x0, 0xe8, 0x0, 0x0, 0xb, 0xd1,
    0x0, 0x0, 0x9e, 0x10, 0x0, 0x1, 0xf4, 0x0,
    0x0, 0x4, 0xf1, 0x0, 0x0, 0x0, 0x10, 0x0,
    0x0, 0x7, 0xe3, 0x0, 0x0, 0x8, 0xf4, 0x0,

    /* U+0040 "@" */
    0x0, 0x2, 0x9e, 0xfe, 0xb4, 0x0, 0x0, 0x5,
    0xe7, 0x10, 0x4, 0xd9, 0x0, 0x3, 0xe3, 0x2b,
    0xfc, 0x39, 0xc7, 0x0, 0xc6, 0x1e, 0xa3, 0x9e,
    0x84, 0xd0, 0x2f, 0x9, 0xc0, 0x0, 0xf3, 0x1f,
    0x4, 0xc0, 0xe5, 0x0, 0x2e, 0x2, 0xe0, 0x4c,
    0xf, 0x30, 0x8, 0xa0, 0x99, 0x2, 0xf0, 0xd8,
    0x5, 0xf6, 0x4d, 0x10, 0xc, 0x62, 0xce, 0x86,
    0xe9, 0x44, 0x0, 0x3e, 0x30, 0x0, 0x0, 0x2e,
    0x10, 0x0, 0x5e, 0x71, 0x1, 0x6e, 0x30, 0x0,
    0x0, 0x29, 0xdf, 0xd9, 0x10, 0x0,

    /* U+0041 "A" */
    0x0, 0x0, 0xd, 0xf3, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0xc9, 0x0, 0x0, 0x0, 0x0, 0x9b, 0x6e,
    0x0, 0x0, 0x0, 0x0, 0xe6, 0x1f, 0x50, 0x0,
    0x0, 0x5, 0xf1, 0xb, 0xb0, 0x0, 0x0, 0xb,
    0xb0, 0x6, 0xf1, 0x0, 0x0, 0x1f, 0x61, 0x12,
    0xf7, 0x0, 0x0, 0x6f, 0xff, 0xff, 0xfd, 0x0,
    0x0, 0xcb, 0x44, 0x44, 0x7f, 0x30, 0x2, 0xf4,
    0x0, 0x0, 0xf, 0x90, 0x8, 0xf0, 0x0, 0x0,
    0xb, 0xe0, 0xe, 0x90, 0x0, 0x0, 0x5, 0xf5,

    /* U+0042 "B" */
    0xcf, 0xff, 0xff, 0xd7, 0x0, 0xcd, 0x55, 0x56,
    0xbf, 0x60, 0xcc, 0x0, 0x0, 0xe, 0xb0, 0xcc,
    0x0, 0x0, 0xd, 0xa0, 0xcc, 0x11, 0x12, 0x7f,
    0x30, 0xcf, 0xff, 0xff, 0xf8, 0x0, 0xcd, 0x44,
    0x44, 0x8f, 0xa0, 0xcc, 0x0, 0x0, 0x7, 0xf1,
    0xcc, 0x0, 0x0, 0x5, 0xf3, 0xcc, 0x0, 0x0,
    0x8, 0xf0, 0xcd, 0x55, 0x55, 0x9f, 0x90, 0xcf,
    0xff, 0xff, 0xe8, 0x0,

    /* U+0043 "C" */
    0x0, 0x6, 0xcf, 0xfc, 0x60, 0x0, 0xa, 0xfb,
    0x66, 0xaf, 0xb0, 0x6, 0xf6, 0x0, 0x0, 0x5f,
    0x60, 0xec, 0x0, 0x0, 0x0, 0xa8, 0x2f, 0x60,
    0x0, 0x0, 0x0, 0x4, 0xf4, 0x0, 0x0, 0x0,
    0x0, 0x4f, 0x40, 0x0, 0x0, 0x0, 0x2, 0xf6,
    0x0, 0x0, 0x0, 0x68, 0xe, 0xc0, 0x0, 0x0,
    0xd, 0xa0, 0x7f, 0x60, 0x0, 0x7, 0xf4, 0x0,
    0xbf, 0xb6, 0x7b, 0xfa, 0x0, 0x0, 0x7d, 0xff,
    0xc6, 0x0,

    /* U+0044 "D" */
    0xcf, 0xff, 0xfe, 0x92, 0x0, 0xcd, 0x55, 0x68,
    0xdf, 0x40, 0xcc, 0x0, 0x0, 0xc, 0xe0, 0xcc,
    0x0, 0x0, 0x4, 0xf5, 0xcc, 0x0, 0x0, 0x0,
    0xf8, 0xcc, 0x0, 0x0, 0x0, 0xea, 0xcc, 0x0,
    0x0, 0x0, 0xe9, 0xcc, 0x0, 0x0, 0x0, 0xf8,
    0xcc, 0x0, 0x0, 0x5, 0xf4, 0xcc, 0x0, 0x0,
    0x1d, 0xd0, 0xcd, 0x55, 0x68, 0xef, 0x30, 0xcf,
    0xff, 0xfd, 0x92, 0x0,

    /* U+0045 "E" */
    0xcf, 0xff, 0xff, 0xff, 0x2c, 0xd5, 0x55, 0x55,
    0x50, 0xcc, 0x0, 0x0, 0x0, 0xc, 0xc0, 0x0,
    0x0, 0x0, 0xcc, 0x11, 0x11, 0x10, 0xc, 0xff,
    0xff, 0xff, 0xb0, 0xcd, 0x44, 0x44, 0x42, 0xc,
    0xc0, 0x0, 0x0, 0x0, 0xcc, 0x0, 0x0, 0x0,
    0xc, 0xc0, 0x0, 0x0, 0x0, 0xcd, 0x55, 0x55,
    0x55, 0x1c, 0xff, 0xff, 0xff, 0xf3,

    /* U+0046 "F" */
    0xcf, 0xff, 0xff, 0xfc, 0xcd, 0x55, 0x55, 0x54,
    0xcc, 0x0, 0x0, 0x0, 0xcc, 0x0, 0x0, 0x0,
    0xcc, 0x11, 0x11, 0x10, 0xcf, 0xff, 0xff, 0xf0,
    0xcd, 0x44, 0x44, 0x40, 0xcc, 0x0, 0x0, 0x0,
    0xcc, 0x0, 0x0, 0x0, 0xcc, 0x0, 0x0, 0x0,
    0xcc, 0x0, 0x0, 0x0, 0xcc, 0x0, 0x0, 0x0,

    /* U+0047 "G" */
    0x0, 0x5, 0xcf, 0xfd, 0x81, 0x0, 0x0, 0x9f,
    0xb7, 0x69, 0xee, 0x30, 0x5, 0xf7, 0x0, 0x0,
    0x1e, 0xc0, 0xd, 0xd0, 0x0, 0x0, 0x3, 0x60,
    0x1f, 0x70, 0x0, 0x11, 0x11, 0x10, 0x4f, 0x40,
    0x0, 0xdf, 0xff, 0xf2, 0x4f, 0x30, 0x0, 0x34,
    0x46, 0xf2, 0x2f, 0x60, 0x0, 0x0, 0x4, 0xf2,
    0xe, 0xc0, 0x0, 0x0, 0x9, 0xf2, 0x6, 0xf7,
    0x0, 0x0, 0x4f, 0xf2, 0x0, 0x8f, 0xb7, 0x6a,
    0xf9, 0xf2, 0x0, 0x4, 0xbe, 0xfc, 0x50, 0xe2,

    /* U+0048 "H" */
    0xcc, 0x0, 0x0, 0x3, 0xf4, 0xcc, 0x0, 0x0,
    0x3, 0xf4, 0xcc, 0x0, 0x0, 0x3, 0xf4, 0xcc,
    0x0, 0x0, 0x3, 0xf4, 0xcc, 0x11, 0x11, 0x14,
    0xf4, 0xcf, 0xff, 0xff, 0xff, 0xf4, 0xcd, 0x44,
    0x44, 0x46, 0xf4, 0xcc, 0x0, 0x0, 0x3, 0xf4,
    0xcc, 0x0, 0x0, 0x3, 0xf4, 0xcc, 0x0, 0x0,
    0x3, 0xf4, 0xcc, 0x0, 0x0, 0x3, 0xf4, 0xcc,
    0x0, 0x0, 0x3, 0xf4,

    /* U+0049 "I" */
    0xbd, 0xbd, 0xbd, 0xbd, 0xbd, 0xbd, 0xbd, 0xbd,
    0xbd, 0xbd, 0xbd, 0xbd,

    /* U+004A "J" */
    0x0, 0x0, 0x7, 0xf1, 0x0, 0x0, 0x7, 0xf1,
    0x0, 0x0, 0x7, 0xf1, 0x0, 0x0, 0x7, 0xf1,
    0x0, 0x0, 0x7, 0xf1, 0x0, 0x0, 0x7, 0xf1,
    0x0, 0x0, 0x7, 0xf1, 0x12, 0x0, 0x7, 0xf1,
    0xae, 0x0, 0x7, 0xf0, 0x8f, 0x10, 0xa, 0xf0,
    0x2f, 0xc6, 0x8f, 0x90, 0x4, 0xcf, 0xe9, 0x0,

    /* U+004B "K" */
    0xcc, 0x0, 0x0, 0x1d, 0xe2, 0xcc, 0x0, 0x1,
    0xde, 0x20, 0xcc, 0x0, 0xc, 0xe2, 0x0, 0xcc,
    0x0, 0xce, 0x20, 0x0, 0xcc, 0xc, 0xe2, 0x0,
    0x0, 0xcc, 0xbf, 0xf3, 0x0, 0x0, 0xcf, 0xe3,
    0xed, 0x0, 0x0, 0xce, 0x20, 0x4f, 0x90, 0x0,
    0xcc, 0x0, 0x9, 0xf4, 0x0, 0xcc, 0x0, 0x0,
    0xde, 0x10, 0xcc, 0x0, 0x0, 0x3f, 0xa0, 0xcc,
    0x0, 0x0, 0x8, 0xf5,

    /* U+004C "L" */
    0xcc, 0x0, 0x0, 0x0, 0xcc, 0x0, 0x0, 0x0,
    0xcc, 0x0, 0x0, 0x0, 0xcc, 0x0, 0x0, 0x0,
    0xcc, 0x0, 0x0, 0x0, 0xcc, 0x0, 0x0, 0x0,
    0xcc, 0x0, 0x0, 0x0, 0xcc, 0x0, 0x0, 0x0,
    0xcc, 0x0, 0x0, 0x0, 0xcc, 0x0, 0x0, 0x0,
    0xcd, 0x55, 0x55, 0x54, 0xcf, 0xff, 0xff, 0xfd,

    /* U+004D "M" */
    0xbf, 0x80, 0x0, 0x0, 0x9, 0xfa, 0xbf, 0xe0,
    0x0, 0x0, 0xe, 0xfa, 0xbc, 0xf3, 0x0, 0x0,
    0x5f, 0xda, 0xbb, 0xb9, 0x0, 0x0, 0xaa, 0xca,
    0xbb, 0x5e, 0x0, 0x1, 0xf4, 0xca, 0xbb, 0xf,
    0x50, 0x6, 0xe0, 0xca, 0xbb, 0xa, 0xa0, 0xc,
    0x90, 0xca, 0xbb, 0x4, 0xf0, 0x2f, 0x30, 0xca,
    0xbb, 0x0, 0xe6, 0x7d, 0x0, 0xca, 0xbb, 0x0,
    0x9b, 0xd8, 0x0, 0xca, 0xbb, 0x0, 0x3f, 0xf2,
    0x0, 0xca, 0xbb, 0x0, 0xd, 0xc0, 0x0, 0xca,

    /* U+004E "N" */
    0xcf, 0x20, 0x0, 0x1, 0xf5, 0xcf, 0xc0, 0x0,
    0x1, 0xf5, 0xcd, 0xf6, 0x0, 0x1, 0xf5, 0xca,
    0x9e, 0x10, 0x1, 0xf5, 0xca, 0x1e, 0x90, 0x1,
    0xf5, 0xca, 0x5, 0xf3, 0x1, 0xf5, 0xca, 0x0,
    0xbd, 0x1, 0xf5, 0xca, 0x0, 0x2f, 0x71, 0xf5,
    0xca, 0x0, 0x8, 0xf3, 0xf5, 0xca, 0x0, 0x0,
    0xdc, 0xf5, 0xca, 0x0, 0x0, 0x4f, 0xf5, 0xca,
    0x0, 0x0, 0xa, 0xf5,

    /* U+004F "O" */
    0x0, 0x5, 0xce, 0xfc, 0x60, 0x0, 0x0, 0xaf,
    0xb7, 0x6a, 0xfc, 0x0, 0x7, 0xf6, 0x0, 0x0,
    0x4f, 0x90, 0xe, 0xa0, 0x0, 0x0, 0x8, 0xf1,
    0x3f, 0x50, 0x0, 0x0, 0x2, 0xf6, 0x5f, 0x20,
    0x0, 0x0, 0x0, 0xf7, 0x5f, 0x20, 0x0, 0x0,
    0x0, 0xf7, 0x3f, 0x50, 0x0, 0x0, 0x2, 0xf6,
    0xe, 0xa0, 0x0, 0x0, 0x8, 0xf1, 0x7, 0xf6,
    0x0, 0x0, 0x4f, 0x90, 0x0, 0xaf, 0xb7, 0x6a,
    0xfc, 0x0, 0x0, 0x5, 0xce, 0xfc, 0x60, 0x0,

    /* U+0050 "P" */
    0xcf, 0xff, 0xff, 0xc4, 0xc, 0xd5, 0x55, 0x7d,
    0xf3, 0xcc, 0x0, 0x0, 0x1f, 0x9c, 0xc0, 0x0,
    0x0, 0xcb, 0xcc, 0x0, 0x0, 0xe, 0xac, 0xc1,
    0x11, 0x2a, 0xf5, 0xcf, 0xff, 0xff, 0xf8, 0xc,
    0xd4, 0x44, 0x31, 0x0, 0xcc, 0x0, 0x0, 0x0,
    0xc, 0xc0, 0x0, 0x0, 0x0, 0xcc, 0x0, 0x0,
    0x0, 0xc, 0xc0, 0x0, 0x0, 0x0,

    /* U+0051 "Q" */
    0x0, 0x5, 0xce, 0xfc, 0x60, 0x0, 0x0, 0xaf,
    0xb7, 0x6a, 0xfc, 0x0, 0x7, 0xf6, 0x0, 0x0,
    0x4f, 0x90, 0xe, 0xa0, 0x0, 0x0, 0x8, 0xf1,
    0x3f, 0x50, 0x0, 0x0, 0x2, 0xf6, 0x5f, 0x20,
    0x0, 0x0, 0x0, 0xf8, 0x5f, 0x20, 0x0, 0x0,
    0x0, 0xf7, 0x3f, 0x50, 0x0, 0x0, 0x2, 0xf5,
    0xe, 0xa0, 0x0, 0x5, 0x8, 0xf1, 0x7, 0xf6,
    0x0, 0x4f, 0xaf, 0x90, 0x0, 0xaf, 0xb7, 0x6c,
    0xff, 0x10, 0x0, 0x5, 0xce, 0xfc, 0x6c, 0xd1,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x70,

    /* U+0052 "R" */
    0xcf, 0xff, 0xff, 0xd7, 0x0, 0xcd, 0x55, 0x56,
    0x9f, 0x80, 0xcc, 0x0, 0x0, 0xa, 0xf0, 0xcc,
    0x0, 0x0, 0x7, 0xf0, 0xcc, 0x0, 0x0, 0x9,
    0xe0, 0xcc, 0x11, 0x12, 0x6f, 0x70, 0xcf, 0xff,
    0xff, 0xf9, 0x0, 0xcd, 0x44, 0x44, 0x9f, 0x50,
    0xcc, 0x0, 0x0, 0xd, 0xb0, 0xcc, 0x0, 0x0,
    0x9, 0xd0, 0xcc, 0x0, 0x0, 0x8, 0xf0, 0xcc,
    0x0, 0x0, 0x5, 0xf3,

    /* U+0053 "S" */
    0x0, 0x4b, 0xef, 0xc6, 0x0, 0x7, 0xfb, 0x66,
    0xbf, 0x90, 0xe, 0xb0, 0x0, 0x9, 0xf1, 0xf,
    0x80, 0x0, 0x2, 0xa2, 0xb, 0xf6, 0x0, 0x0,
    0x0, 0x1, 0xaf, 0xfc, 0x83, 0x0, 0x0, 0x1,
    0x48, 0xcf, 0xc0, 0x1, 0x0, 0x0, 0x4, 0xf8,
    0x5f, 0x20, 0x0, 0x0, 0xcb, 0x2f, 0x90, 0x0,
    0x1, 0xf9, 0x8, 0xfc, 0x76, 0x8e, 0xe2, 0x0,
    0x5b, 0xff, 0xe9, 0x10,

    /* U+0054 "T" */
    0xff, 0xff, 0xff, 0xff, 0xf2, 0x55, 0x57, 0xf9,
    0x55, 0x50, 0x0, 0x2, 0xf5, 0x0, 0x0, 0x0,
    0x2, 0xf5, 0x0, 0x0, 0x0, 0x2, 0xf5, 0x0,
    0x0, 0x0, 0x2, 0xf5, 0x0, 0x0, 0x0, 0x2,
    0xf5, 0x0, 0x0, 0x0, 0x2, 0xf5, 0x0, 0x0,
    0x0, 0x2, 0xf5, 0x0, 0x0, 0x0, 0x2, 0xf5,
    0x0, 0x0, 0x0, 0x2, 0xf5, 0x0, 0x0, 0x0,
    0x2, 0xf5, 0x0, 0x0,

    /* U+0055 "U" */
    0xdb, 0x0, 0x0, 0x2, 0xf6, 0xdb, 0x0, 0x0,
    0x2, 0xf6, 0xdb, 0x0, 0x0, 0x2, 0xf6, 0xdb,
    0x0, 0x0, 0x2, 0xf6, 0xdb, 0x0, 0x0, 0x2,
    0xf6, 0xdb, 0x0, 0x0, 0x2, 0xf6, 0xdb, 0x0,
    0x0, 0x2, 0xf6, 0xcb, 0x0, 0x0, 0x2, 0xf5,
    0xbd, 0x0, 0x0, 0x5, 0xf4, 0x7f, 0x50, 0x0,
    0xb, 0xf1, 0xd, 0xfa, 0x67, 0xcf, 0x70, 0x1,
    0x8d, 0xff, 0xc5, 0x0,

    /* U+0056 "V" */
    0xd, 0xc0, 0x0, 0x0, 0xe, 0xa0, 0x8f, 0x10,
    0x0, 0x4, 0xf4, 0x2, 0xf6, 0x0, 0x0, 0x9f,
    0x0, 0xd, 0xb0, 0x0, 0xe, 0xa0, 0x0, 0x8f,
    0x0, 0x3, 0xf4, 0x0, 0x2, 0xf5, 0x0, 0x8e,
    0x0, 0x0, 0xd, 0xa0, 0xd, 0x90, 0x0, 0x0,
    0x7f, 0x2, 0xf4, 0x0, 0x0, 0x2, 0xf4, 0x7e,
    0x0, 0x0, 0x0, 0xd, 0x9c, 0x90, 0x0, 0x0,
    0x0, 0x7f, 0xf4, 0x0, 0x0, 0x0, 0x2, 0xfe,
    0x0, 0x0,

    /* U+0057 "W" */
    0xbd, 0x0, 0x0, 0x8f, 0x50, 0x0, 0xf, 0x77,
    0xf1, 0x0, 0xc, 0xf9, 0x0, 0x3, 0xf3, 0x3f,
    0x40, 0x1, 0xfa, 0xd0, 0x0, 0x7f, 0x0, 0xf8,
    0x0, 0x5e, 0x3f, 0x10, 0xb, 0xb0, 0xb, 0xc0,
    0x9, 0xa0, 0xf5, 0x0, 0xe7, 0x0, 0x7f, 0x0,
    0xd6, 0xb, 0x90, 0x2f, 0x30, 0x3, 0xf3, 0x1f,
    0x20, 0x7d, 0x6, 0xf0, 0x0, 0xf, 0x65, 0xe0,
    0x2, 0xf1, 0xab, 0x0, 0x0, 0xba, 0x99, 0x0,
    0xe, 0x5e, 0x70, 0x0, 0x7, 0xed, 0x50, 0x0,
    0xab, 0xf3, 0x0, 0x0, 0x3f, 0xf1, 0x0, 0x6,
    0xfe, 0x0, 0x0, 0x0, 0xfd, 0x0, 0x0, 0x2f,
    0xb0, 0x0,

    /* U+0058 "X" */
    0x7, 0xf5, 0x0, 0x0, 0x7f, 0x30, 0xd, 0xe0,
    0x0, 0x2f, 0x90, 0x0, 0x3f, 0x80, 0xb, 0xe0,
    0x0, 0x0, 0x8f, 0x25, 0xf4, 0x0, 0x0, 0x0,
    0xdc, 0xea, 0x0, 0x0, 0x0, 0x4, 0xfe, 0x10,
    0x0, 0x0, 0x0, 0x7f, 0xf3, 0x0, 0x0, 0x0,
    0x2f, 0x9d, 0xd0, 0x0, 0x0, 0xc, 0xe0, 0x3f,
    0x80, 0x0, 0x6, 0xf4, 0x0, 0x9f, 0x20, 0x1,
    0xfa, 0x0, 0x1, 0xec, 0x0, 0xbe, 0x10, 0x0,
    0x6, 0xf7,

    /* U+0059 "Y" */
    0xaf, 0x20, 0x0, 0x0, 0xaf, 0x11, 0xfb, 0x0,
    0x0, 0x4f, 0x60, 0x6, 0xf5, 0x0, 0xd, 0xc0,
    0x0, 0xc, 0xe0, 0x7, 0xf3, 0x0, 0x0, 0x2f,
    0x81, 0xf9, 0x0, 0x0, 0x0, 0x8f, 0xbe, 0x0,
    0x0, 0x0, 0x0, 0xef, 0x50, 0x0, 0x0, 0x0,
    0x8, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x8f, 0x0,
    0x0, 0x0, 0x0, 0x8, 0xf0, 0x0, 0x0, 0x0,
    0x0, 0x8f, 0x0, 0x0, 0x0, 0x0, 0x8, 0xf0,
    0x0, 0x0,

    /* U+005A "Z" */
    0x1f, 0xff, 0xff, 0xff, 0xf4, 0x5, 0x55, 0x55,
    0x5d, 0xf2, 0x0, 0x0, 0x0, 0x6f, 0x60, 0x0,
    0x0, 0x3, 0xf9, 0x0, 0x0, 0x0, 0x1e, 0xd0,
    0x0, 0x0, 0x0, 0xbf, 0x20, 0x0, 0x0, 0x7,
    0xf5, 0x0, 0x0, 0x0, 0x4f, 0x80, 0x0, 0x0,
    0x1, 0xec, 0x0, 0x0, 0x0, 0xc, 0xe1, 0x0,
    0x0, 0x0, 0x7f, 0x95, 0x55, 0x55, 0x52, 0xaf,
    0xff, 0xff, 0xff, 0xf7,

    /* U+005B "[" */
    0xdf, 0xf2, 0xd8, 0x20, 0xd7, 0x0, 0xd7, 0x0,
    0xd7, 0x0, 0xd7, 0x0, 0xd7, 0x0, 0xd7, 0x0,
    0xd7, 0x0, 0xd7, 0x0, 0xd7, 0x0, 0xd7, 0x0,
    0xd7, 0x0, 0xd8, 0x20, 0xdf, 0xf2,

    /* U+005C "\\" */
    0x1f, 0x10, 0x0, 0x0, 0xb7, 0x0, 0x0, 0x4,
    0xd0, 0x0, 0x0, 0xe, 0x40, 0x0, 0x0, 0x8a,
    0x0, 0x0, 0x1, 0xf1, 0x0, 0x0, 0xb, 0x70,
    0x0, 0x0, 0x5d, 0x0, 0x0, 0x0, 0xe3, 0x0,
    0x0, 0x8, 0x90, 0x0, 0x0, 0x2f, 0x0, 0x0,
    0x0, 0xb6,

    /* U+005D "]" */
    0xff, 0xf2, 0x6f, 0x4, 0xf0, 0x4f, 0x4, 0xf0,
    0x4f, 0x4, 0xf0, 0x4f, 0x4, 0xf0, 0x4f, 0x4,
    0xf0, 0x4f, 0x4, 0xf2, 0x6f, 0xff, 0xf0,

    /* U+005E "^" */
    0x0, 0x0, 0x63, 0x0, 0x0, 0x0, 0x3f, 0xc0,
    0x0, 0x0, 0xa, 0x7d, 0x30, 0x0, 0x2, 0xe0,
    0x6b, 0x0, 0x0, 0x98, 0x0, 0xe2, 0x0, 0x1e,
    0x10, 0x7, 0xa0, 0x8, 0x90, 0x0, 0x1e, 0x10,
    0xe2, 0x0, 0x0, 0x89,

    /* U+005F "_" */
    0xdd, 0xdd, 0xdd, 0xdd,

    /* U+0060 "`" */
    0x17, 0x40, 0x8, 0xe0, 0x0, 0xa7,

    /* U+0061 "a" */
    0x1, 0x9e, 0xfd, 0x70, 0x0, 0xbe, 0x65, 0x9f,
    0x40, 0xe, 0x50, 0x0, 0xe7, 0x0, 0x0, 0x13,
    0x7f, 0x80, 0x5, 0xcf, 0xda, 0xe8, 0x2, 0xf8,
    0x0, 0xd, 0x80, 0x6f, 0x10, 0x1, 0xf8, 0x3,
    0xfa, 0x56, 0xdf, 0xb1, 0x7, 0xef, 0xc4, 0x8f,
    0x40,

    /* U+0062 "b" */
    0xe7, 0x0, 0x0, 0x0, 0xe7, 0x0, 0x0, 0x0,
    0xe7, 0x0, 0x0, 0x0, 0xe7, 0xae, 0xfb, 0x20,
    0xee, 0xc5, 0x6d, 0xe1, 0xed, 0x0, 0x1, 0xf8,
    0xe8, 0x0, 0x0, 0xac, 0xe6, 0x0, 0x0, 0x9d,
    0xe8, 0x0, 0x0, 0xac, 0xee, 0x0, 0x1, 0xf8,
    0xee, 0xc6, 0x6d, 0xf1, 0xe7, 0x9e, 0xfc, 0x30,

    /* U+0063 "c" */
    0x0, 0x7d, 0xfd, 0x70, 0x0, 0x8f, 0x85, 0x7f,
    0x80, 0x1f, 0x80, 0x0, 0x8e, 0x4, 0xf2, 0x0,
    0x0, 0x0, 0x6f, 0x0, 0x0, 0x0, 0x4, 0xf2,
    0x0, 0x1, 0x50, 0x1f, 0x70, 0x0, 0x9d, 0x0,
    0x9f, 0x85, 0x8f, 0x60, 0x0, 0x8e, 0xfd, 0x60,
    0x0,

    /* U+0064 "d" */
    0x0, 0x0, 0x0, 0xf, 0x60, 0x0, 0x0, 0x0,
    0xf6, 0x0, 0x0, 0x0, 0xf, 0x60, 0x8, 0xef,
    0xc3, 0xf6, 0x8, 0xf8, 0x58, 0xef, 0x61, 0xf8,
    0x0, 0x7, 0xf6, 0x4f, 0x20, 0x0, 0xf, 0x66,
    0xf0, 0x0, 0x0, 0xe6, 0x4f, 0x10, 0x0, 0xf,
    0x61, 0xf7, 0x0, 0x5, 0xf6, 0x8, 0xf6, 0x24,
    0xef, 0x60, 0x8, 0xef, 0xd5, 0xf6,

    /* U+0065 "e" */
    0x0, 0x6d, 0xfd, 0x40, 0x0, 0x7f, 0x85, 0xaf,
    0x30, 0x1f, 0x70, 0x0, 0xbb, 0x5, 0xf2, 0x0,
    0x5, 0xf0, 0x6f, 0xff, 0xff, 0xff, 0x24, 0xf4,
    0x33, 0x33, 0x30, 0x1f, 0x50, 0x0, 0x5b, 0x0,
    0x7f, 0x85, 0x7e, 0x80, 0x0, 0x7d, 0xfd, 0x70,
    0x0,

    /* U+0066 "f" */
    0x0, 0xaf, 0xb0, 0x5f, 0x63, 0x6, 0xf0, 0xd,
    0xff, 0xf8, 0x39, 0xf4, 0x20, 0x6f, 0x0, 0x6,
    0xf0, 0x0, 0x6f, 0x0, 0x6, 0xf0, 0x0, 0x6f,
    0x0, 0x6, 0xf0, 0x0, 0x6f, 0x0,

    /* U+0067 "g" */
    0x0, 0x7e, 0xfc, 0x4f, 0x20, 0x8f, 0x85, 0x9e,
    0xf2, 0x1f, 0x70, 0x0, 0xaf, 0x24, 0xf2, 0x0,
    0x4, 0xf2, 0x6f, 0x0, 0x0, 0x3f, 0x24, 0xf2,
    0x0, 0x5, 0xf2, 0x1f, 0x70, 0x0, 0xbf, 0x20,
    0x8f, 0x75, 0xad, 0xf2, 0x0, 0x8e, 0xfb, 0x4f,
    0x10, 0x41, 0x0, 0x7, 0xe0, 0xd, 0xc4, 0x36,
    0xf8, 0x0, 0x2a, 0xef, 0xd7, 0x0,

    /* U+0068 "h" */
    0xf6, 0x0, 0x0, 0xf, 0x60, 0x0, 0x0, 0xf6,
    0x0, 0x0, 0xf, 0x69, 0xee, 0xa0, 0xfe, 0xb5,
    0x7f, 0x8f, 0xc0, 0x0, 0x9c, 0xf7, 0x0, 0x7,
    0xdf, 0x60, 0x0, 0x7e, 0xf6, 0x0, 0x7, 0xef,
    0x60, 0x0, 0x7e, 0xf6, 0x0, 0x7, 0xef, 0x60,
    0x0, 0x7e,

    /* U+0069 "i" */
    0xe7, 0xa5, 0x0, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7,
    0xe7, 0xe7, 0xe7, 0xe7,

    /* U+006A "j" */
    0x0, 0xe7, 0x0, 0xa5, 0x0, 0x0, 0x0, 0xe7,
    0x0, 0xe7, 0x0, 0xe7, 0x0, 0xe7, 0x0, 0xe7,
    0x0, 0xe7, 0x0, 0xe7, 0x0, 0xe7, 0x0, 0xe7,
    0x0, 0xe7, 0x5, 0xf5, 0x3f, 0xb0,

    /* U+006B "k" */
    0xe7, 0x0, 0x0, 0x0, 0xe7, 0x0, 0x0, 0x0,
    0xe7, 0x0, 0x0, 0x0, 0xe7, 0x0, 0x4f, 0x80,
    0xe7, 0x3, 0xf8, 0x0, 0xe7, 0x3f, 0x80, 0x0,
    0xea, 0xee, 0x0, 0x0, 0xef, 0xaf, 0x80, 0x0,
    0xea, 0x8, 0xf2, 0x0, 0xe7, 0x0, 0xec, 0x0,
    0xe7, 0x0, 0x5f, 0x60, 0xe7, 0x0, 0xb, 0xe1,

    /* U+006C "l" */
    0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7, 0xe7,
    0xe7, 0xe7, 0xe7, 0xe7,

    /* U+006D "m" */
    0xf5, 0xaf, 0xf9, 0x1a, 0xfe, 0x90, 0xfe, 0x72,
    0x6f, 0xe6, 0x25, 0xf6, 0xfa, 0x0, 0xe, 0xa0,
    0x0, 0xc9, 0xf6, 0x0, 0xd, 0x80, 0x0, 0xba,
    0xf6, 0x0, 0xd, 0x80, 0x0, 0xba, 0xf6, 0x0,
    0xd, 0x80, 0x0, 0xba, 0xf6, 0x0, 0xd, 0x80,
    0x0, 0xba, 0xf6, 0x0, 0xd, 0x80, 0x0, 0xba,
    0xf6, 0x0, 0xd, 0x80, 0x0, 0xba,

    /* U+006E "n" */
    0xf5, 0xaf, 0xea, 0xf, 0xe7, 0x24, 0xe8, 0xfa,
    0x0, 0x8, 0xcf, 0x70, 0x0, 0x7d, 0xf6, 0x0,
    0x7, 0xef, 0x60, 0x0, 0x7e, 0xf6, 0x0, 0x7,
    0xef, 0x60, 0x0, 0x7e, 0xf6, 0x0, 0x7, 0xe0,

    /* U+006F "o" */
    0x0, 0x7d, 0xfe, 0x80, 0x0, 0x8f, 0x95, 0x8f,
    0xb0, 0x1f, 0x90, 0x0, 0x6f, 0x35, 0xf2, 0x0,
    0x0, 0xf8, 0x6f, 0x0, 0x0, 0xd, 0x95, 0xf2,
    0x0, 0x0, 0xf8, 0x1f, 0x80, 0x0, 0x6f, 0x30,
    0x8f, 0x95, 0x8f, 0xb0, 0x0, 0x7d, 0xfe, 0x80,
    0x0,

    /* U+0070 "p" */
    0xe8, 0xaf, 0xfb, 0x20, 0xef, 0x92, 0x3b, 0xe1,
    0xec, 0x0, 0x0, 0xe8, 0xe7, 0x0, 0x0, 0xac,
    0xe6, 0x0, 0x0, 0x9d, 0xe8, 0x0, 0x0, 0xac,
    0xee, 0x0, 0x1, 0xf8, 0xee, 0xc6, 0x6d, 0xf1,
    0xe7, 0x9e, 0xfc, 0x30, 0xe7, 0x0, 0x0, 0x0,
    0xe7, 0x0, 0x0, 0x0, 0xe7, 0x0, 0x0, 0x0,

    /* U+0071 "q" */
    0x0, 0x8e, 0xfc, 0x3f, 0x60, 0x8f, 0x85, 0x8e,
    0xf6, 0x1f, 0x80, 0x0, 0x6f, 0x64, 0xf2, 0x0,
    0x0, 0xf6, 0x6f, 0x0, 0x0, 0xe, 0x64, 0xf2,
    0x0, 0x0, 0xf6, 0x1f, 0x80, 0x0, 0x6f, 0x60,
    0x8f, 0x95, 0x8e, 0xf6, 0x0, 0x8e, 0xfd, 0x4f,
    0x60, 0x0, 0x0, 0x0, 0xf6, 0x0, 0x0, 0x0,
    0xf, 0x60, 0x0, 0x0, 0x0, 0xf6,

    /* U+0072 "r" */
    0xf, 0x47, 0xe5, 0xf, 0xbd, 0x71, 0xf, 0xe0,
    0x0, 0xf, 0x80, 0x0, 0xf, 0x60, 0x0, 0xf,
    0x50, 0x0, 0xf, 0x50, 0x0, 0xf, 0x50, 0x0,
    0xf, 0x50, 0x0,

    /* U+0073 "s" */
    0x4, 0xcf, 0xea, 0x20, 0x1f, 0xb5, 0x6d, 0xc0,
    0x4f, 0x20, 0x3, 0xb0, 0x1f, 0xd7, 0x30, 0x0,
    0x2, 0x9e, 0xfe, 0x70, 0x0, 0x0, 0x29, 0xf4,
    0x5e, 0x0, 0x0, 0xf7, 0x2f, 0xb5, 0x5a, 0xf3,
    0x3, 0xbf, 0xfc, 0x40,

    /* U+0074 "t" */
    0x3, 0x70, 0x0, 0x7e, 0x0, 0x7, 0xe0, 0xd,
    0xff, 0xf9, 0x39, 0xe4, 0x20, 0x7e, 0x0, 0x7,
    0xe0, 0x0, 0x7e, 0x0, 0x7, 0xe0, 0x0, 0x7e,
    0x0, 0x6, 0xf5, 0x20, 0x1d, 0xf9,

    /* U+0075 "u" */
    0xf6, 0x0, 0x7, 0xef, 0x60, 0x0, 0x7e, 0xf6,
    0x0, 0x7, 0xef, 0x60, 0x0, 0x7e, 0xf6, 0x0,
    0x7, 0xef, 0x60, 0x0, 0x8e, 0xe7, 0x0, 0xc,
    0xea, 0xd3, 0x29, 0xee, 0x1b, 0xfe, 0x97, 0xe0,

    /* U+0076 "v" */
    0x9e, 0x0, 0x0, 0xc9, 0x4f, 0x30, 0x1, 0xf4,
    0xe, 0x80, 0x6, 0xe0, 0x9, 0xd0, 0xb, 0x90,
    0x3, 0xf2, 0xf, 0x40, 0x0, 0xe7, 0x4e, 0x0,
    0x0, 0x8c, 0x99, 0x0, 0x0, 0x3f, 0xe4, 0x0,
    0x0, 0xd, 0xe0, 0x0,

    /* U+0077 "w" */
    0x9e, 0x0, 0xc, 0xe0, 0x0, 0xab, 0x4f, 0x20,
    0xf, 0xf2, 0x0, 0xe6, 0xf, 0x60, 0x3e, 0xc6,
    0x2, 0xf1, 0xb, 0xa0, 0x7a, 0x8a, 0x6, 0xd0,
    0x6, 0xe0, 0xa7, 0x4e, 0xa, 0x80, 0x1, 0xf2,
    0xe3, 0xf, 0x2e, 0x30, 0x0, 0xd8, 0xf0, 0xc,
    0x8e, 0x0, 0x0, 0x8f, 0xc0, 0x9, 0xfa, 0x0,
    0x0, 0x3f, 0x80, 0x5, 0xf5, 0x0,

    /* U+0078 "x" */
    0x4f, 0x60, 0x3, 0xf7, 0x0, 0x9f, 0x10, 0xcc,
    0x0, 0x0, 0xea, 0x7f, 0x10, 0x0, 0x4, 0xff,
    0x60, 0x0, 0x0, 0xe, 0xf3, 0x0, 0x0, 0x9,
    0xec, 0xc0, 0x0, 0x3, 0xf5, 0x2f, 0x70, 0x0,
    0xdb, 0x0, 0x8f, 0x20, 0x8f, 0x20, 0x0, 0xdc,
    0x0,

    /* U+0079 "y" */
    0xad, 0x0, 0x0, 0xbb, 0x4f, 0x20, 0x1, 0xf5,
    0xe, 0x80, 0x6, 0xe0, 0x8, 0xd0, 0xb, 0x90,
    0x2, 0xf3, 0x1f, 0x30, 0x0, 0xc8, 0x6d, 0x0,
    0x0, 0x5e, 0xb7, 0x0, 0x0, 0xf, 0xf1, 0x0,
    0x0, 0xb, 0xb0, 0x0, 0x0, 0xf, 0x50, 0x0,
    0x5, 0xae, 0x0, 0x0, 0x1e, 0xe3, 0x0, 0x0,

    /* U+007A "z" */
    0x5f, 0xff, 0xff, 0xf1, 0x14, 0x44, 0x4e, 0xb0,
    0x0, 0x0, 0x9e, 0x10, 0x0, 0x5, 0xf3, 0x0,
    0x0, 0x2f, 0x70, 0x0, 0x0, 0xcb, 0x0, 0x0,
    0x9, 0xe1, 0x0, 0x0, 0x5f, 0x84, 0x44, 0x41,
    0xaf, 0xff, 0xff, 0xf5,

    /* U+007B "{" */
    0x0, 0x4e, 0xf0, 0xc, 0x90, 0x0, 0xe6, 0x0,
    0xe, 0x60, 0x0, 0xe6, 0x0, 0xf, 0x50, 0x6,
    0xf2, 0x3, 0xf6, 0x0, 0x8, 0xf1, 0x0, 0xf,
    0x50, 0x0, 0xe6, 0x0, 0xe, 0x60, 0x0, 0xe6,
    0x0, 0xc, 0x90, 0x0, 0x4e, 0xf0,

    /* U+007C "|" */
    0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5,
    0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5, 0xc5,
    0xc5,

    /* U+007D "}" */
    0x9f, 0x80, 0x0, 0x4f, 0x10, 0x0, 0xf3, 0x0,
    0xf, 0x30, 0x0, 0xf3, 0x0, 0xf, 0x40, 0x0,
    0xcb, 0x0, 0x2, 0xe8, 0x0, 0xbc, 0x20, 0xf,
    0x50, 0x0, 0xf3, 0x0, 0xf, 0x30, 0x0, 0xf3,
    0x0, 0x4f, 0x20, 0x9f, 0x80, 0x0,

    /* U+007E "~" */
    0x3c, 0xd9, 0x30, 0x63, 0xa4, 0x27, 0xdf, 0xc1,
    0x0, 0x0, 0x0, 0x0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 71, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 66, .box_w = 2, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 12, .adv_w = 109, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 25, .adv_w = 142, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 79, .adv_w = 142, .box_w = 9, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 147, .adv_w = 256, .box_w = 14, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 231, .adv_w = 161, .box_w = 11, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 297, .adv_w = 71, .box_w = 2, .box_h = 5, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 302, .adv_w = 66, .box_w = 5, .box_h = 15, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 340, .adv_w = 66, .box_w = 5, .box_h = 15, .ofs_x = -1, .ofs_y = -3},
    {.bitmap_index = 378, .adv_w = 90, .box_w = 6, .box_h = 5, .ofs_x = 0, .ofs_y = 7},
    {.bitmap_index = 393, .adv_w = 154, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 434, .adv_w = 71, .box_w = 3, .box_h = 5, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 442, .adv_w = 100, .box_w = 6, .box_h = 3, .ofs_x = 0, .ofs_y = 3},
    {.bitmap_index = 451, .adv_w = 71, .box_w = 3, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 454, .adv_w = 85, .box_w = 7, .box_h = 12, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 496, .adv_w = 142, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 550, .adv_w = 142, .box_w = 5, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 580, .adv_w = 142, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 628, .adv_w = 142, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 682, .adv_w = 142, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 736, .adv_w = 142, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 790, .adv_w = 142, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 844, .adv_w = 142, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 898, .adv_w = 142, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 952, .adv_w = 142, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1006, .adv_w = 71, .box_w = 3, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1020, .adv_w = 71, .box_w = 3, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 1038, .adv_w = 154, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1083, .adv_w = 154, .box_w = 9, .box_h = 5, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 1106, .adv_w = 154, .box_w = 9, .box_h = 10, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 1151, .adv_w = 142, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1199, .adv_w = 205, .box_w = 13, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1277, .adv_w = 166, .box_w = 12, .box_h = 12, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 1349, .adv_w = 175, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1409, .adv_w = 185, .box_w = 11, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1475, .adv_w = 180, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1535, .adv_w = 156, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1589, .adv_w = 147, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1637, .adv_w = 194, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1709, .adv_w = 185, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1769, .adv_w = 66, .box_w = 2, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1781, .adv_w = 133, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1829, .adv_w = 171, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1889, .adv_w = 142, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1937, .adv_w = 223, .box_w = 12, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2009, .adv_w = 185, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2069, .adv_w = 195, .box_w = 12, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2141, .adv_w = 166, .box_w = 9, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2195, .adv_w = 195, .box_w = 12, .box_h = 13, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 2273, .adv_w = 175, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2333, .adv_w = 166, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2393, .adv_w = 147, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2453, .adv_w = 185, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2513, .adv_w = 156, .box_w = 11, .box_h = 12, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 2579, .adv_w = 237, .box_w = 15, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2669, .adv_w = 156, .box_w = 11, .box_h = 12, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 2735, .adv_w = 166, .box_w = 11, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2801, .adv_w = 156, .box_w = 10, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 2861, .adv_w = 66, .box_w = 4, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 2891, .adv_w = 85, .box_w = 7, .box_h = 12, .ofs_x = -1, .ofs_y = 0},
    {.bitmap_index = 2933, .adv_w = 66, .box_w = 3, .box_h = 15, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 2956, .adv_w = 154, .box_w = 9, .box_h = 8, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 2992, .adv_w = 128, .box_w = 8, .box_h = 1, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 2996, .adv_w = 57, .box_w = 4, .box_h = 3, .ofs_x = -1, .ofs_y = 10},
    {.bitmap_index = 3002, .adv_w = 137, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 3043, .adv_w = 152, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3091, .adv_w = 137, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 3132, .adv_w = 152, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 3186, .adv_w = 137, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 3227, .adv_w = 76, .box_w = 5, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 3257, .adv_w = 147, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 3311, .adv_w = 142, .box_w = 7, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3353, .adv_w = 57, .box_w = 2, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3365, .adv_w = 57, .box_w = 4, .box_h = 15, .ofs_x = -1, .ofs_y = -3},
    {.bitmap_index = 3395, .adv_w = 133, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3443, .adv_w = 57, .box_w = 2, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3455, .adv_w = 218, .box_w = 12, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3509, .adv_w = 142, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3541, .adv_w = 147, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 3582, .adv_w = 152, .box_w = 8, .box_h = 12, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 3630, .adv_w = 152, .box_w = 9, .box_h = 12, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 3684, .adv_w = 85, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 3711, .adv_w = 128, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 3747, .adv_w = 81, .box_w = 5, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 3777, .adv_w = 142, .box_w = 7, .box_h = 9, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 3809, .adv_w = 128, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 3845, .adv_w = 194, .box_w = 12, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 3899, .adv_w = 133, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 3940, .adv_w = 128, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 3988, .adv_w = 123, .box_w = 8, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 4024, .adv_w = 85, .box_w = 5, .box_h = 15, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 4062, .adv_w = 57, .box_w = 2, .box_h = 17, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 4079, .adv_w = 85, .box_w = 5, .box_h = 15, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 4117, .adv_w = 154, .box_w = 8, .box_h = 3, .ofs_x = 1, .ofs_y = 2}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Pair left and right glyphs for kerning*/
static const uint8_t kern_pair_glyph_ids[] =
{
    34, 55,
    34, 87,
    39, 13,
    39, 15,
    39, 34,
    45, 55,
    49, 13,
    49, 15,
    49, 34,
    51, 55,
    53, 13,
    53, 14,
    53, 15,
    53, 27,
    53, 28,
    55, 13,
    55, 14,
    55, 15,
    55, 27,
    55, 28,
    55, 34,
    55, 66,
    55, 70,
    55, 74,
    55, 80,
    55, 83,
    55, 86,
    55, 90,
    56, 13,
    56, 15,
    56, 27,
    56, 28,
    58, 13,
    58, 14,
    58, 15,
    58, 27,
    58, 28,
    58, 81,
    58, 82,
    58, 87,
    71, 71,
    83, 13,
    83, 14,
    83, 15,
    83, 82,
    87, 13,
    87, 15,
    88, 13,
    88, 15,
    90, 13,
    90, 15
};

/* Kerning between the respective left and right glyphs
 * 4.4 format which needs to scaled with `kern_scale`*/
static const int8_t kern_pair_values[] =
{
    -12, -5, -40, -40, -14, -24, -46, -46,
    -19, -5, -28, -33, -28, -28, -28, -33,
    -14, -33, -12, -12, -12, -14, -14, -5,
    -14, -9, -9, -5, -19, -19, -5, -5,
    -36, -28, -36, -24, -24, -19, -24, -14,
    -5, -24, -14, -24, -5, -19, -19, -14,
    -14, -19, -19
};

/*Collect the kern pair's data in one place*/
static const lv_font_fmt_txt_kern_pair_t kern_pairs =
{
    .glyph_ids = kern_pair_glyph_ids,
    .values = kern_pair_values,
    .pair_cnt = 51,
    .glyph_ids_size = 0
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_pairs,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t font_en_16 = {
#else
lv_font_t font_en_16 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 17,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if FONT_EN_16*/

