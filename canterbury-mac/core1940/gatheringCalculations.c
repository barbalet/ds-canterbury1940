/****************************************************************

    gatheringCalculations.c - Canterbury1940

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


#include "pnglite.h"
#include "canterbury.h"


#define  MAPLOCATION "/Users/barbalet/github/ds-canterbury1940/canterbury400.png"
#define  NEWLOCATION "/Users/barbalet/github/ds-canterbury1940/"

extern unsigned char * read_png_file(char * filename, png_t * ptr);

extern int write_png_file(char* filename, int width, int height, unsigned char *buffer);

void mapToOutput(RGB output[WIDTH][HEIGHT], unsigned char* map){
    int incr = 0;
    int loopx = 0;
    while (loopx < WIDTH) {
        int loopy = 0;
        while (loopy < HEIGHT) {
            output[loopx][loopy].r = map[incr++];
            output[loopx][loopy].g = map[incr++];
            output[loopx][loopy].b = map[incr++];
            loopy++;
        }
        loopx++;
    }
}


static int fileCount = 0;

void pngWriteWithCounter(unsigned char* canterbury) {
    char outfileName[200] = {0};
    sprintf(outfileName, "%soutput%d.png", NEWLOCATION ,fileCount++);
    
    write_png_file(outfileName, WIDTH, HEIGHT, (unsigned char*)canterbury);
}


void gatherCalculations(void) {
    png_t snip;
    unsigned char* canterburyByte = read_png_file(MAPLOCATION, &snip);
    
    RGB  canterbury[WIDTH][HEIGHT] = {0};

    mapToOutput(canterbury, canterburyByte);
    
    uint32_t topColors[TOPCOLORENTRIES];
    uint32_t pixelCounts[TOPCOLORENTRIES];
    
    findTopColors(canterburyByte, WIDTH, HEIGHT, topColors, pixelCounts);
    printf("findTopColors\n");
    free(canterburyByte);
    RGB color;
    
    RGB colorBuffer[7][7];
    
    Location outputLocations[MAX_EDGES];
    int outputCount = 0;
    
    for (int i = 0; i < TOPCOLORENTRIES; ++i) {
        printf("Top color %d: 0x%08X, Pixel Count: %u\n", i + 1, topColors[i], pixelCounts[i]);
        
        color.r = (topColors[i] >> 16) & 255;
        color.g = (topColors[i] >> 8) & 255;
        color.b = (topColors[i] >> 0) & 255;
        
        int loopx = 0 + 3 ;
        while (loopx < (WIDTH - 3)) {
            int loopy = 0 + 3;
            while (loopy < (HEIGHT - 3)) {
                
                if (isColorEqual(color, canterbury[loopx][loopy])){
                    int cbx = -3;
                    while (cbx < 4) {
                        int cby = -3;
                        while (cby < 4){
                            colorBuffer[cbx+3][cby+3] = canterbury[loopx+cbx][loopy+cby];
                            cby++;
                        }
                        cbx++;
                        
                        printf("From main, loopx %d loopy %d\n", loopx, loopy);
                        arrayElements(colorBuffer, color, outputLocations, &outputCount);
                    }
                    
                    canterbury[loopx][loopy].r = 255;
                    canterbury[loopx][loopy].g = 255;
                    canterbury[loopx][loopy].b = 255;
                } else {
                    printf("Not equal\n");
                }
                loopy++;
            }
            loopx++;
            pngWriteWithCounter((unsigned char*)canterbury);
        }
    }    
}
