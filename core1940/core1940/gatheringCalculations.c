//
//  gatheringCalculations.c
//  canterbury-mac
//
//  Created by Thomas Barbalet on 12/21/24.
//

#include "pnglite.h"
#include "canterbury.h"


#define  MAPLOCATION "/Users/barbalet/ds-canterbury1940/ds-canterbury1940/canterbury400.png"
#define  NEWLOCATION "/Users/barbalet/ds-canterbury1940/"

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
                    
                    
//                    int loopDirection = 0;
//                    
//                    while (loopDirection < outputCount) {
//
//                        removeLine(canterbury, outputv,loopx, loopy, outputLocations[loopDirection].row - 3, outputLocations[loopDirection].col - 3, DOUBLE_THRESHOLD);
//                        
//                        loopDirection ++;
//                    }
                    
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
