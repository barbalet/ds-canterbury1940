/****************************************************************

    canterbury.h - Canterbury1940

 =============================================================

 Copyright 1996-2025 Tom Barbalet. All rights reserved.

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or
 sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:

 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.

 This software is a continuing work of Tom Barbalet, begun on
 13 June 1996. No apes or cats were harmed in the writing of
 this software.

 ****************************************************************/

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#ifndef canterbury_h
#define canterbury_h

#define NEW1600

#ifdef NEW1600

#define WIDTH 1600
#define HEIGHT 1600

#else

#define WIDTH 400
#define HEIGHT 400

#endif

#define TOPCOLORENTRIES 16

typedef union
{
    struct
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };
    unsigned char data[3];
} RGB;


typedef struct {
    int row;
    int col;
} Location;

void gatherCalculations(void);

void findTopColors(uint8_t *image, size_t width, size_t height, uint32_t topColors[32], uint32_t pixelCounts[32]);

void removeLine(RGB image[HEIGHT][WIDTH], RGB outv[HEIGHT][WIDTH], int startX, int startY, int dx, int dy, double threshold);

bool colorDistance(int r1, int g1, int b1, int r2, int g2, int b2, double threshold);

bool isColorSimilar(RGB startColor, RGB currentColor, double threshold);

bool isColorEqual(RGB color1, RGB color2);

#endif /* canterbury_h */
