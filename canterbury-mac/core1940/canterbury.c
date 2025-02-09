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
#include <unistd.h>
#include "pnglite.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// Define a structure to store RGB color frequencies.
typedef struct {
    uint32_t color; // 32-bit representation of the RGB color.
    uint32_t count; // Frequency of the color.
} ColorFreq;

// Comparator function for sorting colors by frequency in descending order.
int compareColorFreq(const void *a, const void *b) {
    ColorFreq *cf1 = (ColorFreq *)a;
    ColorFreq *cf2 = (ColorFreq *)b;
    return (cf1->count == cf2->count) ? (cf1->color - cf2->color) : (cf2->count - cf1->count);
}

// Find the top colors in an image and their pixel counts.
void findTopColors(uint8_t *image, size_t width, size_t height, uint32_t topColors[TOPCOLORENTRIES], uint32_t pixelCounts[TOPCOLORENTRIES]) {
    #define MAX_COLORS 16777216 // Maximum possible unique RGB colors (24-bit).
    uint32_t *colorFrequency = calloc(MAX_COLORS, sizeof(uint32_t));
    if (!colorFrequency) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Count the frequency of each color in the image.
    for (size_t i = 0; i < width * height * 3; i += 3) {
        uint32_t color = (image[i] << 16) | (image[i + 1] << 8) | image[i + 2];
        colorFrequency[color]++;
    }

    // Transfer non-zero color frequencies to an array for sorting.
    ColorFreq *colorFreqArray = malloc(MAX_COLORS * sizeof(ColorFreq));
    size_t colorCount = 0;
    for (uint32_t color = 0; color < MAX_COLORS; ++color) {
        if (colorFrequency[color] > 0) {
            colorFreqArray[colorCount++] = (ColorFreq){color, colorFrequency[color]};
        }
    }

    free(colorFrequency); // Free the hash table.

    // Sort colors by frequency.
    qsort(colorFreqArray, colorCount, sizeof(ColorFreq), compareColorFreq);

    // Extract the top TOPCOLORENTRIES colors.
    for (size_t i = 0; i < TOPCOLORENTRIES && i < colorCount; ++i) {
        topColors[i] = colorFreqArray[i].color;
        pixelCounts[i] = colorFreqArray[i].count;
    }

    // Fill remaining entries with 0 if there are fewer than TOPCOLORENTRIES colors.
    for (size_t i = colorCount; i < TOPCOLORENTRIES; ++i) {
        topColors[i] = 0;
        pixelCounts[i] = 0;
    }

    free(colorFreqArray); // Free the sorted array.
}

#define MAPLOCATION "/Users/barbalet/github/ds-canterbury1940/canterbury400.png"
#define NEWLOCATION "/Users/barbalet/github/ds-canterbury1940/"

extern unsigned char *read_png_file(char *filename, png_t *ptr);
extern int write_png_file(char *filename, int width, int height, unsigned char *buffer);

// Map raw image data to an RGB array.
void mapToOutput(RGB output[WIDTH][HEIGHT], unsigned char *map) {
    int index = 0;
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            output[x][y].r = map[index++];
            output[x][y].g = map[index++];
            output[x][y].b = map[index++];
        }
    }
}

static int fileCount = 0;

// Write the image to a PNG file with an incrementing counter.
void pngWriteWithCounter(unsigned char *canterbury) {
    char outfileName[200];
    sprintf(outfileName, "%soutput%d.png", NEWLOCATION, fileCount++);
    write_png_file(outfileName, WIDTH, HEIGHT, canterbury);
}

// Main function to gather calculations and process the image.
void gatherCalculations(void) {
    png_t snip;
    unsigned char *canterburyByte = read_png_file(MAPLOCATION, &snip);
    RGB canterbury[WIDTH][HEIGHT] = {0};

    mapToOutput(canterbury, canterburyByte);

    uint32_t topColors[TOPCOLORENTRIES];
    uint32_t pixelCounts[TOPCOLORENTRIES];
    findTopColors(canterburyByte, WIDTH, HEIGHT, topColors, pixelCounts);
    printf("findTopColors\n");
    free(canterburyByte);

    RGB colorBuffer[7][7];
    Location outputLocations[MAX_EDGES];
    int outputCount = 0;

    for (int i = 0; i < TOPCOLORENTRIES; ++i) {
        printf("Top color %d: 0x%08X, Pixel Count: %u\n", i + 1, topColors[i], pixelCounts[i]);

        RGB color = {
            .r = (topColors[i] >> 16) & 255,
            .g = (topColors[i] >> 8) & 255,
            .b = topColors[i] & 255
        };

        for (int x = 3; x < WIDTH - 3; x++) {
            for (int y = 3; y < HEIGHT - 3; y++) {
                if (isColorEqual(color, canterbury[x][y])) {
                    for (int dx = -3; dx <= 3; dx++) {
                        for (int dy = -3; dy <= 3; dy++) {
                            colorBuffer[dx + 3][dy + 3] = canterbury[x + dx][y + dy];
                        }
                    }
                    arrayElements(colorBuffer, color, outputLocations, &outputCount);
                    canterbury[x][y] = (RGB){255, 255, 255}; // Mark the pixel as processed.
                }
            }
        }
        pngWriteWithCounter((unsigned char *)canterbury);
    }
}

#define COLOR_THRESHOLD 50.0 // Threshold for RGB closeness

// Check if two colors are within a given distance.
bool colorDistance(int r1, int g1, int b1, int r2, int g2, int b2, double threshold) {
    return sqrt((r1 - r2) * (r1 - r2) + (g1 - g2) * (g1 - g2) + (b1 - b2) * (b1 - b2)) <= threshold;
}

// Check if two RGB colors are equal.
bool isColorEqual(RGB color1, RGB color2) {
    return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b;
}

// Find edge colors close to the center color.
void findCloseEdgeColors(RGB array[ARRAY_SIZE][ARRAY_SIZE], Location outputLocations[MAX_EDGES], int *outputCount) {
    RGB centerColor = array[3][3];
    *outputCount = 0;

    for (int col = 0; col < ARRAY_SIZE; col++) {
        if (colorDistance(array[0][col].r, array[0][col].g, array[0][col].b, centerColor.r, centerColor.g, centerColor.b, COLOR_THRESHOLD)) {
            outputLocations[(*outputCount)++] = (Location){0, col};
        }
        if (colorDistance(array[ARRAY_SIZE - 1][col].r, array[ARRAY_SIZE - 1][col].g, array[ARRAY_SIZE - 1][col].b, centerColor.r, centerColor.g, centerColor.b, COLOR_THRESHOLD)) {
            outputLocations[(*outputCount)++] = (Location){ARRAY_SIZE - 1, col};
        }
    }

    for (int row = 1; row < ARRAY_SIZE - 1; row++) {
        if (colorDistance(array[row][0].r, array[row][0].g, array[row][0].b, centerColor.r, centerColor.g, centerColor.b, COLOR_THRESHOLD)) {
            outputLocations[(*outputCount)++] = (Location){row, 0};
        }
        if (colorDistance(array[row][ARRAY_SIZE - 1].r, array[row][ARRAY_SIZE - 1].g, array[row][ARRAY_SIZE - 1].b, centerColor.r, centerColor.g, centerColor.b, COLOR_THRESHOLD)) {
            outputLocations[(*outputCount)++] = (Location){row, ARRAY_SIZE - 1};
        }
    }
}

// Wrapper function for finding close edge colors.
void arrayElements(RGB array[ARRAY_SIZE][ARRAY_SIZE], RGB centerColor, Location outputLocations[MAX_EDGES], int *outputCount) {
    findCloseEdgeColors(array, outputLocations, outputCount);
}

int main(int argc, const char *argv[]) {
    gatherCalculations();
    return 0;
}
