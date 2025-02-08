//
//  arrayElements.c
//  canterbury-mac
//
//  Created by Thomas Barbalet on 12/21/24.
//

#include "canterbury.h"
#include <unistd.h>


#define COLOR_THRESHOLD 50.0 // Threshold for RGB closeness

void findCloseEdgeColors(
    RGB array[ARRAY_SIZE][ARRAY_SIZE],

    Location outputLocations[MAX_EDGES],
    int *outputCount) {

    RGB centerColor = array[3][3];
    
    *outputCount = 0;

    // Check top and bottom edges
    for (int col = 0; col < ARRAY_SIZE; col++) {
        // Top edge
        if (colorDistance(array[0][col].r, array[0][col].g, array[0][col].b,
                          centerColor.r, centerColor.g, centerColor.b, COLOR_THRESHOLD)) {
            outputLocations[*outputCount].row = 0;
            outputLocations[*outputCount].col = col;
            (*outputCount)++;
        }

        // Bottom edge
        if (colorDistance(array[ARRAY_SIZE - 1][col].r, array[ARRAY_SIZE - 1][col].g, array[ARRAY_SIZE - 1][col].b,
                          centerColor.r, centerColor.g, centerColor.b, COLOR_THRESHOLD)) {
            outputLocations[*outputCount].row = ARRAY_SIZE - 1;
            outputLocations[*outputCount].col = col;
            (*outputCount)++;
        }
    }

    // Check left and right edges
    for (int row = 1; row < ARRAY_SIZE - 1; row++) { // Exclude corners already checked
        // Left edge
        if (colorDistance(array[row][0].r, array[row][0].g, array[row][0].b,
                          centerColor.r, centerColor.g, centerColor.b, COLOR_THRESHOLD)) {
            outputLocations[*outputCount].row = row;
            outputLocations[*outputCount].col = 0;
            (*outputCount)++;
        }

        // Right edge
        if (colorDistance(array[row][ARRAY_SIZE - 1].r, array[row][ARRAY_SIZE - 1].g, array[row][ARRAY_SIZE - 1].b,
                          centerColor.r, centerColor.g, centerColor.b, COLOR_THRESHOLD)) {
            outputLocations[*outputCount].row = row;
            outputLocations[*outputCount].col = ARRAY_SIZE - 1;
            (*outputCount)++;
        }
    }
}

void arrayElements(RGB array[ARRAY_SIZE][ARRAY_SIZE], RGB centerColor, Location outputLocations[MAX_EDGES], int* outputCount) {

    sleep(1);
    
    findCloseEdgeColors(array, outputLocations, outputCount);

//    printf("Number of close edge colors: %d\n", *outputCount);
//    for (int i = 0; i < *outputCount; i++) {
//        printf("Location: (%d, %d)\n", outputLocations[i].row, outputLocations[i].col);
//    }
}
