#include "GrayBMP24.hpp"
#include "global.hpp"
#include <vector>

#ifndef _IMAGEPROC_
#define _IMAGEPROC_

typedef enum searchDir {
    Up,
    Down,
    Left,
    Right,
    UpLeft,
    UpRight,
    DownLeft,
    Downright,
    NoneD
} searchDir;
void makeSmallDot(GrayBMP &bmp, int x, int y);
void makeSmallDot(GrayBMP &bmp, ii coor);
void makeBigDot(GrayBMP &bmp, int x, int y);
void makeBigDot(GrayBMP &bmp, ii coor);
void makeTinyDot(GrayBMP &bmp, int x, int y);
void makeTinyDot(GrayBMP &bmp, ii coor);
uint8_t SobelPixel(GrayBMP &original, GrayBMP &current, int x, int y);
void showEdge(GrayBMP &bmp, vii coors);
// void showEdge(std::vector<ii> edge, GrayBMP input);
//void EdgeDetect(GrayBMP &original, GrayBMP &current);
//void recursionSearch(GrayBMP &original, GrayBMP &current, searchDir state, searchDir direction, int CoorX, int CoorY);

#endif