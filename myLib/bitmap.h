#ifndef _BIT_MAP_HELPER_
#define _BIT_MAP_HELPER_

#include <stdio.h>
#include <stdlib.h>

void saveImage(char* imageName, int **img, int imageWidth, int imageHeight);
void saveColoredImage(char* imageName, int **r, int **g, int **b, int imageWidth, int imageHeight);

#endif
