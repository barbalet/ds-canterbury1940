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


#if 1

#define TOLERANCE_VALUE (20)

#include "canterbury.h"
#include <unistd.h>
#include "pnglite.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Define a structure to store RGB color frequencies.
typedef struct {
    uint32_t color; // 32-bit representation of the RGB color.
    uint32_t count; // Frequency of the color.
    float luminance; // Luminance of the color (darker colors have lower luminance).
} ColorFreq;

// Comparator function for sorting colors by frequency in descending order and luminance in ascending order.
int compareColorFreq(const void *a, const void *b) {
    ColorFreq *cf1 = (ColorFreq *)a;
    ColorFreq *cf2 = (ColorFreq *)b;
    if (cf1->count == cf2->count) {
        return (cf1->luminance < cf2->luminance) ? -1 : 1;
    }
    return (cf2->count - cf1->count);
}

// Calculate the luminance of a color.
float calculateLuminance(uint32_t color) {
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    return 0.2126 * r + 0.7152 * g + 0.0722 * b; // Standard luminance formula.
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
    size_t imageSize = width * height * 3;
    for (size_t i = 0; i < imageSize; i += 3) {
        uint32_t color = (image[i] << 16) | (image[i + 1] << 8) | image[i + 2];
        colorFrequency[color]++;
    }

    // Transfer non-zero color frequencies to an array for sorting.
    ColorFreq *colorFreqArray = malloc(MAX_COLORS * sizeof(ColorFreq));
    if (!colorFreqArray) {
        fprintf(stderr, "Memory allocation failed\n");
        free(colorFrequency);
        return;
    }

    size_t colorCount = 0;
    for (uint32_t color = 0; color < MAX_COLORS; ++color) {
        if (colorFrequency[color] > 0) {
            colorFreqArray[colorCount++] = (ColorFreq){color, colorFrequency[color], calculateLuminance(color)};
        }
    }

    free(colorFrequency); // Free the hash table.

    // Sort colors by frequency and luminance.
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

#ifdef NEW1600
#define MAPLOCATION "/Users/barbalet/github/ds-canterbury1940/canterbury1600.png"
#else
#define MAPLOCATION "/Users/barbalet/github/ds-canterbury1940/canterbury400.png"
#endif

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
    snprintf(outfileName, sizeof(outfileName), "%soutput%d.png", NEWLOCATION, fileCount++);
    write_png_file(outfileName, WIDTH, HEIGHT, canterbury);
}

// Structure to store line information.
typedef struct {
    int startX, startY;
    int endX, endY;
    RGB color;
} LineInfo;

// Check if two RGB colors are similar within a tolerance.
bool isColorSimilar(RGB color1, RGB color2, int tolerance) {
    return abs((int)color1.r - (int)color2.r) <= tolerance &&
           abs((int)color1.g - (int)color2.g) <= tolerance &&
           abs((int)color1.b - (int)color2.b) <= tolerance;
}

// Function to draw a line with a real-number gradient.
void drawLine(RGB image[WIDTH][HEIGHT], int startX, int startY, int endX, int endY, RGB color) {
    int dx = abs(endX - startX);
    int dy = abs(endY - startY);
    int sx = (startX < endX) ? 1 : -1;
    int sy = (startY < endY) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        image[startX][startY] = color;

        if (startX == endX && startY == endY) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            startX += sx;
        }
        if (e2 < dx) {
            err += dx;
            startY += sy;
        }
    }
}

// Function to detect and remove lines of the same color in all four quadrants.
void removeLines(RGB image[WIDTH][HEIGHT], FILE *jsonFile, uint32_t topColors[TOPCOLORENTRIES]) {
    fprintf(jsonFile, "[\n"); // Start of JSON array.

    int firstLine = 1; // Flag to handle commas between JSON objects.

    // Array to keep track of processed colors.
    bool *processedColors = calloc(TOPCOLORENTRIES, sizeof(bool));
    if (!processedColors) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Loop until all colors are processed.
    while (1) {
        bool allColorsProcessed = true;
        bool lineFound = false;

        // Iterate through all four quadrants of the image.
        for (int quadrant = 0; quadrant < 4; quadrant++) {
            int startX = 0, startY = 0, endX = WIDTH, endY = HEIGHT;

            // Define the bounds for each quadrant.
            switch (quadrant) {
                case 0: // Top-left quadrant
                    startX = 0;
                    startY = 0;
                    endX = WIDTH / 2;
                    endY = HEIGHT / 2;
                    break;
                case 1: // Top-right quadrant
                    startX = WIDTH / 2;
                    startY = 0;
                    endX = WIDTH;
                    endY = HEIGHT / 2;
                    break;
                case 2: // Bottom-left quadrant
                    startX = 0;
                    startY = HEIGHT / 2;
                    endX = WIDTH / 2;
                    endY = HEIGHT;
                    break;
                case 3: // Bottom-right quadrant
                    startX = WIDTH / 2;
                    startY = HEIGHT / 2;
                    endX = WIDTH;
                    endY = HEIGHT;
                    break;
            }

            // Iterate through the current quadrant.
            for (int x = startX; x < endX; x++) {
                for (int y = startY; y < endY; y++) {
                    if (image[x][y].r != 255 || image[x][y].g != 255 || image[x][y].b != 255) { // Skip white pixels.
                        RGB color = image[x][y];
                        uint32_t colorKey = (color.r << 16) | (color.g << 8) | color.b;

                        // Check if the color has been processed.
                        bool colorProcessed = false;
                        for (int i = 0; i < TOPCOLORENTRIES; i++) {
                            if (processedColors[i] && topColors[i] == colorKey) {
                                colorProcessed = true;
                                break;
                            }
                        }

                        if (!colorProcessed) {
                            allColorsProcessed = false;

                            // Check lines in all directions using real-number gradients.
                            for (int dx = -1; dx <= 1; dx++) {
                                for (int dy = -1; dy <= 1; dy++) {
                                    if (dx == 0 && dy == 0) continue; // Skip the current pixel.

                                    int endXLine = x, endYLine = y;

                                    // Move in the direction until the color changes or the image boundary is reached.
                                    while (1) {
                                        int nextX = endXLine + dx;
                                        int nextY = endYLine + dy;

                                        if (nextX < 0 || nextX >= WIDTH || nextY < 0 || nextY >= HEIGHT) {
                                            break; // Out of bounds.
                                        }

                                        if (!isColorSimilar(image[nextX][nextY], color, TOLERANCE_VALUE)) { // Tolerance for color similarity.
                                            break; // Color change.
                                        }

                                        endXLine = nextX;
                                        endYLine = nextY;
                                    }

                                    // If a line is detected, write it to the JSON file.
                                    if (endXLine != x || endYLine != y) {
                                        if (!firstLine) {
                                            fprintf(jsonFile, ",\n"); // Add comma between JSON objects.
                                        }
                                        firstLine = 0;

                                        fprintf(jsonFile, "  {\n");
                                        fprintf(jsonFile, "    \"startX\": %d,\n", x);
                                        fprintf(jsonFile, "    \"startY\": %d,\n", y);
                                        fprintf(jsonFile, "    \"endX\": %d,\n", endXLine);
                                        fprintf(jsonFile, "    \"endY\": %d,\n", endYLine);
                                        fprintf(jsonFile, "    \"color\": {\"r\": %d, \"g\": %d, \"b\": %d}\n", color.r, color.g, color.b);
                                        fprintf(jsonFile, "  }");

                                        // Remove the line from the image.
                                        drawLine(image, x, y, endXLine, endYLine, (RGB){255, 255, 255});
                                        lineFound = true;
                                    }
                                }
                            }

                            // Mark the color as processed.
                            for (int i = 0; i < TOPCOLORENTRIES; i++) {
                                if (topColors[i] == colorKey) {
                                    processedColors[i] = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        // If no lines were found in this iteration, break the loop.
        if (!lineFound) {
            break;
        }

        // If all colors have been processed, break the loop.
        if (allColorsProcessed) {
            break;
        }
    }

    fprintf(jsonFile, "\n]\n"); // End of JSON array.
    free(processedColors); // Free the processed colors array.
}

// Main function to gather calculations and process the image.
void gatherCalculations(void) {
    png_t snip;
    unsigned char *canterburyByte = read_png_file(MAPLOCATION, &snip);
    if (!canterburyByte) {
        fprintf(stderr, "Failed to read PNG file.\n");
        return;
    }

    RGB canterbury[WIDTH][HEIGHT] = {0};
    mapToOutput(canterbury, canterburyByte);

    uint32_t topColors[TOPCOLORENTRIES];
    uint32_t pixelCounts[TOPCOLORENTRIES];
    findTopColors(canterburyByte, WIDTH, HEIGHT, topColors, pixelCounts);
    printf("findTopColors\n");
    free(canterburyByte);

    // Open a JSON file to write line information.
    char jsonFileName[200];
    snprintf(jsonFileName, sizeof(jsonFileName), "%slines.json", NEWLOCATION);
    FILE *jsonFile = fopen(jsonFileName, "w");
    if (!jsonFile) {
        fprintf(stderr, "Failed to open JSON file for writing.\n");
        return;
    }

    // Remove lines and write them to the JSON file.
    removeLines(canterbury, jsonFile, topColors);

    fclose(jsonFile); // Close the JSON file.

    // Write the modified image to a PNG file.
    pngWriteWithCounter((unsigned char *)canterbury);
}

int main(int argc, const char *argv[]) {
    gatherCalculations();
    return 0;
}

#else

#include "canterbury.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define WIDTH 400  // Replace with the actual width of the image.
//#define HEIGHT 400 // Replace with the actual height of the image.

// Structure to represent an RGB color.
//typedef struct {
//    unsigned char r, g, b;
//} RGB;

// Structure to represent a line.
typedef struct {
    int startX, startY;
    int endX, endY;
    RGB color;
} LineInfo;

// Function to draw a line on the image array.
void drawLine(RGB image[WIDTH][HEIGHT], LineInfo line) {
    int dx = abs(line.endX - line.startX);
    int dy = abs(line.endY - line.startY);
    int sx = (line.startX < line.endX) ? 1 : -1;
    int sy = (line.startY < line.endY) ? 1 : -1;
    int err = dx - dy;

    int x = line.startX;
    int y = line.startY;

    while (1) {
        // Set the pixel color.
        image[x][y] = line.color;

        if (x == line.endX && y == line.endY) {
            break; // Line drawing complete.
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

// Function to parse the JSON file and extract line information.
void parseJSONFile(const char *filename, RGB image[WIDTH][HEIGHT]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open JSON file.\n");
        return;
    }

    char line[256];
    LineInfo currentLine;
    int readingLine = 0;

    while (fgets(line, sizeof(line), file)) {
        // Remove newline characters.
        line[strcspn(line, "\n")] = 0;

        if (strstr(line, "\"startX\":")) {
            sscanf(line, "    \"startX\": %d,", &currentLine.startX);
        } else if (strstr(line, "\"startY\":")) {
            sscanf(line, "    \"startY\": %d,", &currentLine.startY);
        } else if (strstr(line, "\"endX\":")) {
            sscanf(line, "    \"endX\": %d,", &currentLine.endX);
        } else if (strstr(line, "\"endY\":")) {
            sscanf(line, "    \"endY\": %d,", &currentLine.endY);
        } else if (strstr(line, "\"r\":")) {
            sscanf(line, "    \"color\": {\"r\": %hhu, \"g\": %hhu, \"b\": %hhu}", &currentLine.color.r, &currentLine.color.g, &currentLine.color.b);
            readingLine = 1; // All line data has been read.
        }

        // If we have read a complete line, draw it on the image.
        if (readingLine) {
            // Swap x and y coordinates if necessary.
            int temp = currentLine.startX;
            currentLine.startX = currentLine.startY;
            currentLine.startY = temp;

            temp = currentLine.endX;
            currentLine.endX = currentLine.endY;
            currentLine.endY = temp;

            drawLine(image, currentLine);
            readingLine = 0; // Reset for the next line.
        }
    }

    fclose(file);
}

// Function to save the image array as a PNG file.
void saveImageAsPNG(const char *filename, RGB image[WIDTH][HEIGHT]) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Failed to open output PNG file.\n");
        return;
    }

    // Write the PPM header (for simplicity, we'll use PPM format).
    fprintf(file, "P6\n%d %d\n255\n", WIDTH, HEIGHT);

    // Write the pixel data.
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            fwrite(&image[x][y], sizeof(RGB), 1, file);
        }
    }

    fclose(file);
}

int main(void) {
    const char *jsonFilename = "/Users/barbalet/github/ds-canterbury1940/lines.json";
    const char *outputImageFilename = "/Users/barbalet/github/ds-canterbury1940/reconstructed_image.ppm";

    // Initialize the image array with white pixels.
    RGB image[WIDTH][HEIGHT];
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            image[x][y] = (RGB){255, 255, 255}; // White background.
        }
    }

    // Parse the JSON file and draw the lines on the image.
    parseJSONFile(jsonFilename, image);

    // Save the reconstructed image as a PNG file.
    saveImageAsPNG(outputImageFilename, image);

    printf("Image reconstructed and saved to %s\n", outputImageFilename);
    return 0;
}

#endif
