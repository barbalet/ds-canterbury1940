//
//  canterbury.h
//  canterbury-mac
//
//  Created by Thomas Barbalet on 12/21/24.
//

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <stdlib.h>
#include <string.h>

#ifndef canterbury_h
#define canterbury_h

#define WIDTH 400
#define HEIGHT 400

#define DOUBLE_THRESHOLD (4)

#define ARRAY_SIZE 7
#define MAX_EDGES (4 * (ARRAY_SIZE - 1)) // Maximum edge points in a 7x7 array

#define TOPCOLORENTRIES 64

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

//void findColorInArray(RGB array[HEIGHT][WIDTH], RGB outv[HEIGHT][WIDTH], RGB target_color, int px, int py);

void arrayElements(RGB array[ARRAY_SIZE][ARRAY_SIZE], RGB centerColor, Location outputLocations[MAX_EDGES], int* outputCount);

#endif /* canterbury_h */
