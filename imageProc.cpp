
//#include "declaration.h"
#include "GrayBMP24.hpp"
#include "imageProc.h"
#include "image.hpp"
#include "global.hpp"
#include <vector>
#include <iostream>

uint8_t getXY(GrayBMP &bmp, int x, int y) {
    if (x < 0 || y < 0 || x >= int(bmp.bmpHeader.width) || y >= int(bmp.bmpHeader.height)) {
            return 0;
        }else{
            return bmp.getPixel(x, y);
        }
}

void makeSmallDot(GrayBMP &bmp, int x, int y) {
    for (int i = x - 2; i <= x + 2; ++i) {
        for (int j = y - 2; j <= y + 2; ++j) {
            if (i >= 0 && j >= 0 && i < Image::WIDTH && j < Image::HEIGHT) {
                bmp.getPixel(i, j) = 125;
            }
        }
    }
}

void makeSmallDot(GrayBMP &bmp, ii coor) {
    for (int i = coor.second - 2; i <= coor.second + 2; i ++) {
        for (int j = coor.first - 2; j <= coor.first + 2; j ++) {
            if (i >= 0 && j >= 0 && i < Image::WIDTH && j < Image::HEIGHT) {
                bmp.getPixel(i, j) = 125;
            }
        }
    }
}

void makeBigDot(GrayBMP &bmp, int x, int y) {
    for (int i = x - 7; i <= x + 7; ++i) {
        for (int j = y - 7; j <= y + 7; ++j) {
            if (i >= 0 && j >= 0 && i < Image::WIDTH && j < Image::HEIGHT) {
                bmp.getPixel(i, j) = 125;
            }
        }
    }
}

void makeBigDot(GrayBMP &bmp, ii coor) {
    for (int i = coor.second - 7; i <= coor.second + 7; i ++) {
        for (int j = coor.first - 7; j <= coor.first + 7; ++i) {
            if (i >= 0 && j >= 0 && i < Image::WIDTH && j < Image::HEIGHT) {
                bmp.getPixel(i, j) = 125;
            }
        }
    }
}

void makeTinyDot(GrayBMP &bmp, int x, int y) {
    for (int i = x - 1; i < x + 1; ++i) {
        for (int j = y - 1; j < y + 1; ++j) {
            if (i >= 0 && j >= 0 && i < Image::WIDTH && j < Image::HEIGHT) {
                bmp.getPixel(i, j) = 125;
            }
        }
    }
}

void makeTinyDot(GrayBMP &bmp, ii coor) {
    for (int i = coor.second - 1; i < coor.second + 1; i ++) {
        for (int j = coor.first - 1; j < coor.first + 1; j ++) {
            if (i >= 0 && j >= 0 && i < Image::WIDTH && j < Image::HEIGHT) {
                bmp.getPixel(i, j) = 125;
            }
        }
    }
}
uint8_t SobelPixel(GrayBMP &original, GrayBMP &current, int CoorX, int CoorY){
	int matrixx[3][3] = {{ 1, 0,-1},
						 { 2, 0,-2},
						 { 1, 0,-1}};

	int matrixy[3][3] = {{ 1, 2, 1},
						 { 0, 0, 0},
						 {-1,-2,-1}};

	int matrixxy[3][3] = {{-2,-1, 0},
						  {-1, 0, 1},
						  { 0, 1, 2}};

	int matrixyx[3][3] = {{ 0,-1,-2},
						  { 1, 0,-1},
						  { 2, 1, 0}};
    
    int sumx = 0, sumy = 0,sumxy = 0, sumyx = 0;
    for (int j = -1; j < 2; j ++) {
        for (int i = -1; i < 2; i ++) {
            sumx += getXY(original, CoorX + i, CoorY + j)*matrixx[1 + j][i + 1];
            sumy += getXY(original, CoorX + i, CoorY + j)*matrixy[1 + j][i + 1];
            sumxy += getXY(original, CoorX + i, CoorY + j)*matrixxy[1 + j][i + 1];
            sumyx += getXY(original, CoorX + i, CoorY + j)*matrixyx[1 + j][i + 1];
        }
    }
    sumx = std::max(std::min(sumx, 255), 0);
    sumy = std::max(std::min(sumy, 255), 0);
    sumxy = std::max(std::min(sumxy, 255), 0);
    sumyx = std::max(std::min(sumyx, 255), 0);
    int sum = abs(sumx) + abs(sumy) + abs(sumxy) + abs(sumyx);
    current.getPixel(CoorX, CoorY) = uint8_t(std::min(sum, 255));
    return current.getPixel(CoorX, CoorY);
}

void showEdge(GrayBMP &bmp, vii coors) {
    std::cout << "Edge size: " << coors.size() << std::endl;
    for (int i = 0; i < coors.size(); ++i) {
        makeSmallDot(bmp, coors[i]);
    }
}

// void EdgeDetect(GrayBMP &original, GrayBMP &current) {
//     //First detect the edge on the left
//     //From the bottom of the image
//     int CoorX = IMAGE_WIDTH / 2;
//     for (; CoorX > 0 && original.getPixel(CoorX, IMAGE_HEIGHT - 5) == 255; -- CoorX);
//     recursionSearch(original, current, Up, Left, CoorX + 1, IMAGE_HEIGHT - 5);
//     //recursionSearch(original, current, NoneD, CoorX, IMAGE_HEIGHT - 2);
// }

// void recursionSearch(GrayBMP &original, GrayBMP &current, searchDir state, searchDir direction, int CoorX, int CoorY) {
//     //std::cout << "done" << std::endl;
//     if (CoorX < 0 || CoorY < 0 || CoorX >= IMAGE_WIDTH || CoorY >= IMAGE_HEIGHT) {
//         //makeDot(current, CoorX, CoorY);
//         return;
//     }
//     if (original.getPixel(CoorX, CoorY) == 0)
//     return;
    
//     // if (SobelPixel(original, current, CoorX, CoorY) != 255) {
//     //     //makeDot(current, CoorX, CoorY);
//     //     return;
//     // }
//     makeDot(current, CoorX, CoorY);
//     switch (state) {
//     case Up:
//         switch (direction) {
//         case Up:

//         }
//         break;
//     case Down:
//         recursionSearch(original, current, Down, CoorX, CoorY + 1);
//         recursionSearch(original, current, Right, CoorX + 1, CoorY);
//         recursionSearch(original, current, Left, CoorX - 1, CoorY);
//         break;
//     case Left:
//         recursionSearch(original, current, Up, CoorX, CoorY - 1);
//         recursionSearch(original, current, Down, CoorX, CoorY + 1);
//         recursionSearch(original, current, Left, CoorX - 1, CoorY);
//         break;
//     case Right:
//         recursionSearch(original, current, Up, CoorX, CoorY - 1);
//         recursionSearch(original, current, Down, CoorX, CoorY + 1);
//         recursionSearch(original, current, Right, CoorX + 1, CoorY);
//         break;
//     default:
//         recursionSearch(original, current, Up, CoorX, CoorY - 1);
//         recursionSearch(original, current, Down, CoorX, CoorY + 1);
//         recursionSearch(original, current, Left, CoorX - 1, CoorY);
//         recursionSearch(original, current, Right, CoorX + 1, CoorY);
//         break;
//     }
// }