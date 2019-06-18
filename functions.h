#include "GrayBMP24.hpp"

#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H



void brightness (GrayBMP& bmp, int adjustmentlevels);
void contrast (GrayBMP& bmp, float adjust);
void invert (GrayBMP& bmp);
void medianfilter(GrayBMP& bmp, uint8_t windowSize);
void Threshold(GrayBMP& bmp);
void sobelX(GrayBMP& bmp);
void sobel(GrayBMP& bmp);
void CornerDetect1(GrayBMP& bmp, int32_t threst);
void CornerDetect(GrayBMP& bmp,float th);
void perspectiveBMP(GrayBMP& bmp, uint8_t arr[]);
void bmp_to_arr(GrayBMP img_src, uint8_t arr[]);

#endif
