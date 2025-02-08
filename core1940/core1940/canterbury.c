//
//  canterbury.c
//  canterbury-mac
//
//  Created by Thomas Barbalet on 12/21/24.
//

#include "canterbury.h"

bool colorDistance(int r1, int g1, int b1, int r2, int g2, int b2, double threshold) {
    return (sqrt((r1 - r2) * (r1 - r2) +
                (g1 - g2) * (g1 - g2) +
                (b1 - b2) * (b1 - b2)) <= threshold);
}

bool isColorSimilar(RGB startColor, RGB currentColor, double threshold) {
//    double mse = (pow(startColor.r - currentColor.r, 2) +
//                  pow(startColor.g - currentColor.g, 2) +
//                  pow(startColor.b - currentColor.b, 2)) / 3.0;
//    return mse <= threshold;
    
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

//void findColorInArray(RGB array[HEIGHT][WIDTH], RGB outv[HEIGHT][WIDTH], RGB target_color, int px, int py) {
//    for (int y = 0; y < HEIGHT; y++) {
//        for (int x = 0; x < WIDTH; x++) {
//            if (isColorEqual(array[y][x], target_color)) {
//                printf("Color found at coordinates: (%d, %d)\n", x, y);
//                {
//                    removeLine(array, outv, x, y, DOUBLE_THRESHOLD);
//                }
//            }
//        }
//    }
//}

