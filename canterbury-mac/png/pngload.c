#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include "pnglite.h"

/// Reads a PNG file and decodes its pixel data.
/// - Parameter filename: The path to the PNG file.
/// - Parameter ptr: A pointer to the `png_t` structure to store the PNG file information.
/// - Returns: A pointer to the decoded pixel data (RGB format). Returns `NULL` on failure.
unsigned char *read_png_file(char *filename, png_t *ptr) {
    int retval;
    unsigned char *buffer = NULL; // Buffer to store the raw pixel data
    unsigned char *buffer2 = NULL; // Buffer to store RGB data (if conversion is needed)

    // Initialize the PNG library
    png_init(0, 0);

    // Attempt to open the PNG file
    retval = png_open_file(ptr, filename);
    if (retval != PNG_NO_ERROR) {
        printf("Failed to open file %s\n", filename);

        // Handle specific PNG errors
        switch (retval) {
            case PNG_FILE_ERROR:    printf("File error\n"); break;
            case PNG_HEADER_ERROR:  printf("Header error\n"); break;
            case PNG_IO_ERROR:      printf("IO error\n"); break;
            case PNG_EOF_ERROR:     printf("EOF error\n"); break;
            case PNG_CRC_ERROR:     printf("CRC error\n"); break;
            case PNG_MEMORY_ERROR:  printf("Memory error\n"); break;
            case PNG_ZLIB_ERROR:    printf("Zlib error\n"); break;
            case PNG_UNKNOWN_FILTER:printf("Unknown filter\n"); break;
            case PNG_NOT_SUPPORTED: printf("PNG format not supported\n"); break;
            case PNG_WRONG_ARGUMENTS: printf("Wrong arguments\n"); break;
        }
        return NULL; // Return NULL on failure
    }

    // Ensure the PNG has at least 3 bytes per pixel (RGB)
    if (ptr->bpp < 3) {
        printf("Not enough bytes per pixel\n");
        return NULL;
    }

    // Allocate memory for the raw pixel data
    buffer = (unsigned char *)malloc(ptr->width * ptr->height * ptr->bpp);
    if (!buffer) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    // Decode the PNG data into the buffer
    png_get_data(ptr, buffer);

    // If the PNG has more than 3 bytes per pixel (e.g., RGBA), convert it to RGB
    if (ptr->bpp > 3) {
        int i, j = 0;
        buffer2 = (unsigned char *)malloc(ptr->width * ptr->height * 3); // Allocate memory for RGB data
        if (!buffer2) {
            printf("Memory allocation failed\n");
            free(buffer);
            return NULL;
        }

        // Convert RGBA (or other formats) to RGB
        for (i = 0; i < ptr->width * ptr->height * ptr->bpp; i += ptr->bpp, j += 3) {
            buffer2[j] = buffer[i];     // Red
            buffer2[j + 1] = buffer[i + 1]; // Green
            buffer2[j + 2] = buffer[i + 2]; // Blue
        }

        free(buffer); // Free the original buffer
        buffer = buffer2; // Use the RGB buffer
    }

    return buffer; // Return the decoded pixel data
}

/// Writes a PNG file from pixel data.
/// - Parameter filename: The name of the output PNG file.
/// - Parameter width: The width of the image.
/// - Parameter height: The height of the image.
/// - Parameter buffer: The pixel data buffer (RGB format).
/// - Returns: 0 on success, 1 on failure.
int write_png_file(char *filename, int width, int height, unsigned char *buffer) {
    png_t png;
    FILE *fp = fopen(filename, "wb");

    // Check if the file was opened successfully
    if (fp == NULL) {
        fprintf(stderr, "Could not open file %s for writing\n", filename);
        return 1;
    }
    fclose(fp); // Close the file as `pnglite` will handle writing

    // Initialize the PNG library
    png_init(0, 0);

    // Open the file for writing
    png_open_file_write(&png, filename);

    // Write the pixel data to the PNG file
    png_set_data(&png, width, height, 8, PNG_TRUECOLOR, buffer);

    // Close the PNG file
    png_close_file(&png);

    return 0; // Success
}
