/****************************************************************

    canterbury.c - Canterbury1940

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


#include "canterbury.h"

bool colorDistance(int r1, int g1, int b1, int r2, int g2, int b2, double threshold) {
    return (sqrt((r1 - r2) * (r1 - r2) +
                (g1 - g2) * (g1 - g2) +
                (b1 - b2) * (b1 - b2)) <= threshold);
}

bool isColorSimilar(RGB startColor, RGB currentColor, double threshold) {
    return colorDistance(startColor.r, startColor.g, startColor.b, currentColor.r, currentColor.g, currentColor.b, threshold);
}

bool isColorEqual(RGB color1, RGB color2) {
    return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b;
}

static int counter = 0;

void removeLine(RGB image[HEIGHT][WIDTH], RGB outv[HEIGHT][WIDTH], int startX, int startY, int dx, int dy, double threshold) {
    if (startX < 0 || startX >= WIDTH || startY < 0 || startY >= HEIGHT) {
        printf("Starting point out of bounds.\n");
        return;
    }

    RGB startColor = image[startY][startX];
    int x = startX;
    int y = startY;

    // Find direction based on surrounding startXY location surrounding images.
    // do it within -2/+2, -2/+2 for x and y getting dx, dy directions
    
    // Traversal and line detection
    while (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        if (!isColorSimilar(startColor, image[y][x], threshold)) {
            break;
        }

        // Remove the pixel from the output buffer
        outv[y][x] = image[y][x];
        image[y][x] = (RGB){0, 0, 0}; // Black as a placeholder for removed pixels

        // Move to the next pixel along the line
        
        counter++;

        x += ((dx * counter)/3);
        y += ((dy * counter)/3);
        
        if (counter == 3) {
            counter = 0;
        }
        
        // x += dx; Update this logic based on the expected line direction
        // y += dy;
    }

    printf("Line removed from output buffer.\n");
}

