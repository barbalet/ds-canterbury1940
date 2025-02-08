//
//  top32.c
//  canterbury-mac
//
//  Created by Thomas Barbalet on 12/21/24.
//

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "canterbury.h"

// Define a structure to store RGB color frequencies.
typedef struct {
    uint32_t color; // 32-bit representation of the RGB color.
    uint32_t count; // Frequency of the color.
} ColorFreq;

// Comparator function for sorting colors by frequency in descending order.
int compareColorFreq(const void *a, const void *b) {
    ColorFreq *cf1 = (ColorFreq *)a;
    ColorFreq *cf2 = (ColorFreq *)b;
    if (cf1->count == cf2->count) {
        return cf1->color - cf2->color; // Secondary sort by color value.
    }
    return cf2->count - cf1->count;
}

void findTopColors(uint8_t *image, size_t width, size_t height, uint32_t topColors[32], uint32_t pixelCounts[TOPCOLORENTRIES]) {
    // Hash table for storing color frequencies.
    #define MAX_COLORS 16777216 // Maximum possible unique RGB colors (24-bit).
    uint32_t *colorFrequency = calloc(MAX_COLORS, sizeof(uint32_t));
    if (!colorFrequency) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Iterate through the image and count the frequency of each color.
    for (size_t i = 0; i < width * height * 3; i += 3) {
        uint32_t color = (image[i] << 16) | (image[i + 1] << 8) | image[i + 2];
        colorFrequency[color]++;
    }

    // Transfer the color frequencies to an array of ColorFreq structs.
    ColorFreq *colorFreqArray = malloc(MAX_COLORS * sizeof(ColorFreq));
    size_t colorCount = 0;
    for (uint32_t color = 0; color < MAX_COLORS; ++color) {
        if (colorFrequency[color] > 0) {
            colorFreqArray[colorCount].color = color;
            colorFreqArray[colorCount].count = colorFrequency[color];
            colorCount++;
        }
    }

    // Free the hash table memory as it's no longer needed.
    free(colorFrequency);

    // Sort the color frequencies in descending order.
    qsort(colorFreqArray, colorCount, sizeof(ColorFreq), compareColorFreq);

    // Extract the top TOPCOLORENTRIES colors and their pixel counts.
    for (size_t i = 0; i < TOPCOLORENTRIES && i < colorCount; ++i) {
        topColors[i] = colorFreqArray[i].color;
        pixelCounts[i] = colorFreqArray[i].count;
    }

    // If there are less than 32 colors, fill the remaining entries with 0.
    for (size_t i = colorCount; i < TOPCOLORENTRIES; ++i) {
        topColors[i] = 0;
        pixelCounts[i] = 0;
    }

    // Free the color frequency array.
    free(colorFreqArray);
}


