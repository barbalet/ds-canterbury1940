#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define MAX_LINES 100000
#define DISTANCE_THRESHOLD 10.0 // Threshold for considering lines "close by"
#define GRADIENT_THRESHOLD 0.1  // Threshold for considering gradients "similar"

typedef struct {
    int r, g, b;
} RGB;

typedef struct {
    int startX, startY; // Integer coordinates
    int endX, endY;
    RGB color;
    double gradient; // Gradient of the line
} LineInfo;

// Function to calculate the Euclidean distance between two points.
double distance(int x1, int y1, int x2, int y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Function to check if two colors are equal.
bool isColorEqual(RGB color1, RGB color2) {
    return color1.r == color2.r && color1.g == color2.g && color1.b == color2.b;
}

// Function to calculate the gradient of a line.
double calculateGradient(LineInfo line) {
    if (line.endX == line.startX) {
        return INFINITY; // Vertical line
    }
    return (double)(line.endY - line.startY) / (double)(line.endX - line.startX);
}

// Function to check if two lines are close to each other and have similar gradients.
bool areLinesClose(LineInfo line1, LineInfo line2) {
    double startDistance = distance(line1.startX, line1.startY, line2.startX, line2.startY);
    double endDistance = distance(line1.endX, line1.endY, line2.endX, line2.endY);

    // Check if gradients are similar
    bool gradientsSimilar = fabs(line1.gradient - line2.gradient) < GRADIENT_THRESHOLD;

    return startDistance < DISTANCE_THRESHOLD && endDistance < DISTANCE_THRESHOLD && gradientsSimilar;
}

// Function to read lines from a JSON file.
int readLinesFromJSON(const char *filename, LineInfo *lines) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open JSON file for reading.\n");
        return 0;
    }

    char buffer[1024];
    int lineCount = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (strstr(buffer, "\"startX\":")) {
            sscanf(buffer, "    \"startX\": %d,", &lines[lineCount].startX);
            fgets(buffer, sizeof(buffer), file); // Read next line
            sscanf(buffer, "    \"startY\": %d,", &lines[lineCount].startY);
            fgets(buffer, sizeof(buffer), file); // Read next line
            sscanf(buffer, "    \"endX\": %d,", &lines[lineCount].endX);
            fgets(buffer, sizeof(buffer), file); // Read next line
            sscanf(buffer, "    \"endY\": %d,", &lines[lineCount].endY);
            fgets(buffer, sizeof(buffer), file); // Read next line
            sscanf(buffer, "    \"color\": {\"r\": %d, \"g\": %d, \"b\": %d}", &lines[lineCount].color.r, &lines[lineCount].color.g, &lines[lineCount].color.b);

            // Calculate gradient for the line
            lines[lineCount].gradient = calculateGradient(lines[lineCount]);
            lineCount++;
        }
    }

    fclose(file);
    return lineCount;
}

// Function to write every fifth line to a JSON file.
void writeEveryFifthLineToJSON(const char *filename, LineInfo *lines, int lineCount) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        fprintf(stderr, "Failed to open JSON file for writing.\n");
        return;
    }

    fprintf(file, "[\n");
    for (int i = 0; i < lineCount; i++) {
        if ((i + 1) % 5 == 0) { // Select every fifth line note (5th, 10th, 15th, etc.)
            fprintf(file, "  {\n");
            fprintf(file, "    \"startX\": %d,\n", lines[i].startX);
            fprintf(file, "    \"startY\": %d,\n", lines[i].startY);
            fprintf(file, "    \"endX\": %d,\n", lines[i].endX);
            fprintf(file, "    \"endY\": %d,\n", lines[i].endY);
            fprintf(file, "    \"color\": {\"r\": %d, \"g\": %d, \"b\": %d}\n", lines[i].color.r, lines[i].color.g, lines[i].color.b);
            fprintf(file, "  }%s\n", (i < lineCount - 1) ? "," : "");
        }
    }
    fprintf(file, "]\n");

    fclose(file);
}

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_json> <output_json>\n", argv[0]);
        return 1;
    }

    const char *inputFile = argv[1];
    const char *outputFile = argv[2];

    LineInfo lines[MAX_LINES];
    int lineCount = readLinesFromJSON(inputFile, lines);

    printf("Read %d lines from JSON.\n", lineCount);

    // Write every fifth line to the output file
    writeEveryFifthLineToJSON(outputFile, lines, lineCount);

    printf("Every fifth line note has been written to %s.\n", outputFile);

    return 0;
}
