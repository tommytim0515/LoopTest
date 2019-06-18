#include <iostream>
#include <algorithm>
#include <functional>
#include <cstdint>
#include <cmath>
#include <fstream>
#include <string>
#include "functions.h"
#include "GrayBMP24.hpp"
//#include "declaration.h"
#include "COORD.hardcode"
#include "global.hpp"
#include "image.hpp"

void brightness (GrayBMP& bmp, int adjustmentlevels) {
	for (auto& pixel: bmp.img) {
		pixel = uint8_t(std::max(std::min((int)pixel + adjustmentlevels, 255), 0));
	}
}

void contrast (GrayBMP& bmp, float adjust) {
	auto half = 127;
	for (auto& pixel: bmp.img) {
		pixel = uint8_t(std::max(std::min((int)((pixel - half)*adjust + half), 255), 0));
	}
	std::cout << "Contrast Finished" << std::endl;
}

void invert (GrayBMP& bmp) {
	for (auto& pixel: bmp.img) {
		pixel = uint8_t(std::max(std::min(255 - (int)pixel, 255), 8));
	}
}

void medianfilter(GrayBMP& bmp, uint8_t windowSize) {
	GrayBMP original = bmp;
	auto getXY = [&](int x, int y)->uint8_t{
		if (x < 0 || y < 0 || x >= int(original.bmpHeader.width) || y >= int(original.bmpHeader.height)) {
			return 0;
		}else{
			return original.getPixel(x, y);
		}
	};

	for (int y = 0; y < int(original.bmpHeader.height); y++) {
		for (int x = 0; x < int(original.bmpHeader.width); x++) {
			//std::vector<uint8_t> grayLevels(windowSize*windowSize); // reserves the require length of the values in vector
			std::vector<uint8_t> grayLevels;

			for (int j = -windowSize / 2; j <= windowSize / 2; j ++) {
				for (int i = -windowSize / 2; i <= windowSize / 2; i ++) {
					grayLevels.push_back(getXY(x + i + 1, y + j + 1));
				}
			}
			std::sort(grayLevels.begin(), grayLevels.end());
			bmp.getPixel(x, y) = grayLevels[(windowSize*windowSize - 1)/2]; //changed signature of GrayBMP.getPixel() to return a reference
		}
	}
}

void Threshold(GrayBMP& bmp) {
	for (auto& pixel: bmp.img) {
		if (pixel >= 150)
			pixel = 255;
		else
			pixel = 0;
		
	}
	//std::cout << "Threshold Finished" << std::endl;
}

void sobelX(GrayBMP& bmp){
	int matrix[3][3] = {{-1, 0, 1},
						{-2, 0, 2},
						{-1, 0, 1}};
	GrayBMP original = bmp;
		auto getXY = [&](int x, int y)->uint8_t{
			if (x < 0 || y < 0 || x >= int(original.bmpHeader.width) || y >= int(original.bmpHeader.height)) {
				return 0;
			}else{
				return original.getPixel(x, y);
			}
		};
		std::vector<int> sobelfiltered;
		for (int y = 0; y < int(original.bmpHeader.height); y++) {
			for (int x = 0; x < int(original.bmpHeader.width); x++) {
				int sum = 0;
				for (int j = -1; j < 2; j ++) {
					for (int i = -1; i < 2; i ++) {
						sum += getXY(x + i, y + j)*matrix[1 + j][i + 1];
					}
				}
				sobelfiltered.push_back(sum);
			}
		}
		int i = 0;
		for (int y = 0; y < int(original.bmpHeader.height); y++) {
			for (int x = 0; x < int(original.bmpHeader.width); x++) {
				bmp.getPixel(x, y) = uint8_t(std::max(std::min(sobelfiltered[i], 255), 0));
				i ++;
			}
		}
		//std::cout << "SobelX Finished" << std::endl;
	}

void sobel(GrayBMP& bmp){
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
	GrayBMP original = bmp;
		auto getXY = [&](int x, int y)->uint8_t{
			if (x < 0 || y < 0 || x >= int(original.bmpHeader.width) || y >= int(original.bmpHeader.height)) {
				return 0;
			}else{
				return original.getPixel(x, y);
			}
		};
		std::vector<int> sobelfilteredx;
		std::vector<int> sobelfilteredy;
		std::vector<int> sobelfilteredxy;
		std::vector<int> sobelfilteredyx;
		for (int y = 0; y < (int)(original.bmpHeader.height); y++) {
			for (int x = 0; x < int(original.bmpHeader.width); x++) {
				int sumx = 0, sumy = 0,sumxy = 0, sumyx = 0;
				for (int j = -1; j < 2; j ++) {
					for (int i = -1; i < 2; i ++) {
						sumx += getXY(x + i, y + j)*matrixx[1 + j][i + 1];
						sumy += getXY(x + i, y + j)*matrixy[1 + j][i + 1];
						sumxy += getXY(x + i, y + j)*matrixxy[1 + j][i + 1];
						sumyx += getXY(x + i, y + j)*matrixyx[1 + j][i + 1];
					}
				}
				sumx = std::max(std::min(sumx, 255), 0);
				sobelfilteredx.push_back(sumx);
				sumy = std::max(std::min(sumy, 255), 0);
				sobelfilteredy.push_back(sumy);
				sumxy = std::max(std::min(sumxy, 255), 0);
				sobelfilteredxy.push_back(sumxy);
				sumyx = std::max(std::min(sumyx, 255), 0);
				sobelfilteredyx.push_back(sumyx);
			}
		}
		int i =0;
		for (int y = 0; y < (int)(original.bmpHeader.height); y++) {
			for (int x = 0; x < (int)(original.bmpHeader.width); x++) {
				int sum, sumx, sumy,sumxy, sumyx;
				sumx = sobelfilteredx[i];
				sumy = sobelfilteredy[i];
				sumxy = sobelfilteredxy[i];
				sumyx = sobelfilteredyx[i];
				sum = abs(sumx) + abs(sumy) + abs(sumxy) + abs(sumyx);
				bmp.getPixel(x, y) = uint8_t(std::min(sum, 255));
				i ++;
			}
		}
		//std::cout << "Sobel Finished" << std::endl;
	}

void CornerDetect1(GrayBMP& bmp, int32_t threst) {
	int adjust3 = 4;
	int matrixx[9][9] = {  {1, 1, 1, 1, 1, 1, 1, 1, 1},
						   {1, 1, 1, 1, 1, 1, 1, 1, 1},
						   {1, 1, 2, 2, 2, 2, 2, 1, 1},
						   {1, 1, 2, 4, 4, 4, 2, 1, 1},
						   {1, 1, 2, 4, 8, 4, 2, 1, 1},
						   {1, 1, 2, 4, 4, 4, 2, 1, 1},
						   {1, 1, 2, 2, 2, 2, 2, 1, 1},
						   {1, 1, 1, 1, 1, 1, 1, 1, 1},
						   {1, 1, 1, 1, 1, 1, 1, 1, 1}};
	GrayBMP original = bmp;
		auto getXY = [&](int32_t x, int32_t y)->uint8_t{
			if (x < 0 || y < 0 || x >= int(original.bmpHeader.width) || y >= int(original.bmpHeader.height)) {
				return 0;
			}else{
				return original.getPixel(x, y);
			}
		};
		for (int y = 5; y < (int)(original.bmpHeader.height - 5); y+=9) {
			for (int x = 5; x < int(original.bmpHeader.width - 5); x+=9) {
				if (getXY(x, y) == 255) {
				int32_t sum1 = 0, sum2 = 0, sum3 = 0,sum4 = 0, sum5 = 0, sum6 = 0, sum7 = 0, sum8 = 0, sum9 = 0,
						sum10 = 0, sum11 = 0, sum12 = 0, sum13 = 0, sum14 = 0, sum15 = 0, sum16 = 0, sum17 = 0;
				for (int j = -adjust3 ; j < adjust3 + 1; j ++) {
					for (int i = -adjust3; i < adjust3 + 1; i ++) {
						int adjust1 = 6, adjust2 = 2;
						sum1 += getXY(x + i, y + j)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum2 += getXY(x + i + adjust1, y + j)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum3 += getXY(x + i - adjust1, y + j)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum4 += getXY(x + i, y + j + adjust1)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum5 += getXY(x + i, y + j - adjust1)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum6 += getXY(x + i + adjust1, y + j + adjust1)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum7 += getXY(x + i - adjust1, y + j - adjust1)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum8 += getXY(x + i + adjust1, y + j - adjust1)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum9 += getXY(x + i - adjust1, y + j + adjust1)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum10 += getXY(x + i + adjust1, y + j + adjust2)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum11 += getXY(x + i + adjust1, y + j - adjust2)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum12 += getXY(x + i - adjust1, y + j + adjust2)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum13 += getXY(x + i - adjust1, y + j - adjust2)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum14 += getXY(x + i + adjust2, y + j + adjust1)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum15 += getXY(x + i - adjust2, y + j + adjust1)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum16 += getXY(x + i + adjust2, y + j - adjust1)*matrixx[adjust3 + j][i + adjust3] / 10;
						sum17 += getXY(x + i - adjust2, y + j - adjust1)*matrixx[adjust3 + j][i + adjust3] / 10;
					}
				}
				//std::cout << sum1 << std::endl;
				if (abs(sum1 - sum2) >= threst && abs(sum1 - sum3) >= threst && abs(sum1 - sum4) >= threst &&
						abs(sum1 - sum5) >= threst && abs(sum1 - sum6) >= threst && abs(sum1 - sum7) >= threst &&
						abs(sum1 - sum8) >= threst && abs(sum1 - sum9) >= threst && abs(sum1 - sum10) >= threst &&
						abs(sum1 - sum11) >= threst && abs(sum1 - sum12) >= threst && abs(sum1 - sum13) >= threst &&
						abs(sum1 - sum14) >= threst && abs(sum1 - sum15) >= threst && abs(sum1 - sum16) >= threst &&
						abs(sum1 - sum17) >= threst) {
						bmp.getPixel(x - 1, y - 1) = 127;
						bmp.getPixel(x - 1, y) = 127;
						bmp.getPixel(x, y - 1) = 127;
						bmp.getPixel(x + 1, y + 1) = 127;
						bmp.getPixel(x + 1, y) = 127;
						bmp.getPixel(x, y + 1) = 127;
						bmp.getPixel(x - 1, y + 1) = 127;
						bmp.getPixel(x + 1, y - 1) = 127;
						bmp.getPixel(x, y) = 127;
				}
			}
		}
	}
}

void CornerDetect(GrayBMP& bmp,float th) {
	int Gauss[5][5] = { {1, 4, 7, 4,1},
			            {4,16,26,16,4},
						{7,26,41,26,7},
						{4,16,26,16,4},
						{1, 4, 7, 4,1} };

	int matrixx[3][3] = {{ 1, 0,-1},
						 { 2, 0,-2},
						 { 1, 0,-1}};

	int matrixy[3][3] = {{ 1, 2, 1},
						 { 0, 0, 0},
						 {-1,-2,-1}};
	GrayBMP original = bmp;
	auto getXY = [&](int x, int y)->uint8_t{
		if (x < 0 || y < 0 || x >= int(original.bmpHeader.width) || y >= int(original.bmpHeader.height)) {
			return 0;
		}else{
			return original.getPixel(x, y);
		}
	};
	std::vector<int> sobelfilteredx;
	std::vector<int> sobelfilteredy;
	std::vector<int> sobelfilteredxy;
	std::vector<int> R;
	for (int y = 0; y < (int)(original.bmpHeader.height); y++) {
		for (int x = 0; x < int(original.bmpHeader.width); x++) {
			int sumx = 0, sumy = 0;
			for (int j = -1; j < 2; j ++) {
				for (int i = -1; i < 2; i ++) {
					sumx += getXY(x + i, y + j)*matrixx[1 + j][i + 1];
					sumy += getXY(x + i, y + j)*matrixy[1 + j][i + 1];
				}
			}
			sobelfilteredx.push_back(sumx*sumx);
			sobelfilteredy.push_back(sumy*sumy);
			sobelfilteredxy.push_back(sumx*sumy);
		}
	}
	int r_max = 0;
	for (int y = 2; y < int(original.bmpHeader.height - 2); y++) {
		for (int x = 2; x < int(original.bmpHeader.width - 2); x++) {
			int64_t sumx = 0, sumy = 0,sumxy = 0;
			for (int j = -2; j < 3; j ++) {
				for (int i = -2; i < 3; i ++) {
					sumx += sobelfilteredx[x + i + (y + j)*(bmp.bmpHeader.width)]*Gauss[2 + j][i + 2];
					sumy += sobelfilteredy[x + i + (y + j)*(bmp.bmpHeader.width)]*Gauss[2 + j][i + 2];
					sumxy += sobelfilteredxy[x + i + (y + j)*(bmp.bmpHeader.width)]*Gauss[2 + j][i + 2];
				}
			}
			sumx = sumx / 273;
			sumy = sumy / 273;
			sumxy = sumxy / 273;
			//std::cout << sumx << std::endl;
			int det = sumx*sumy - sumxy*sumxy;
			int tracel;
			if ((sumx + sumy) != 0) {
				tracel = sumx + sumy;
			}else {
				tracel = 1;
			}
			int r = det / tracel;
			R.push_back(r);
			if (r > r_max) {
				r_max = r;
			}
		}
	}
	int r_th = th*r_max;
	for (int y = 2; y < int(original.bmpHeader.height - 2); y++) {
		for (int x = 2; x < int(original.bmpHeader.width - 2); x++) {
			if (R[x - 2 + (y - 2)*(bmp.bmpHeader.width - 4)] >= r_th) {
				bmp.getPixel(x - 1, y - 1) = 127;
				bmp.getPixel(x - 1, y) = 127;
				bmp.getPixel(x, y - 1) = 127;
				bmp.getPixel(x + 1, y + 1) = 127;
				bmp.getPixel(x + 1, y) = 127;
				bmp.getPixel(x, y + 1) = 127;
				bmp.getPixel(x - 1, y + 1) = 127;
				bmp.getPixel(x + 1, y - 1) = 127;
				bmp.getPixel(x, y) = 127;
			}
		}
	}
}


// void perspectiveBMP(GrayBMP& bmp, uint8_t arr[]) {
// 	for(int i = 0; i < Image::HEIGHT; ++i) {
// 		for(int j = 0; j < IMImage::WIDTH; ++j) {
// 			if(std::isnan(remapping[j][i][0]) || std::isnan(remapping[j][i][1])) continue;
// 			int x = COORD[i][j][1]; int y = COORD[i][j][0];
// 			bmp.img[y * PERS_X + x] = arr[i * Image::WIDTH + j];
// 		}
// 	}
// }

void bmp_to_arr(GrayBMP img_src, uint8_t arr[]) {
	for(int i = 0; i < Image::HEIGHT; ++i)
		for(int j = 0; j < Image::WIDTH; ++j)
			arr[i * Image::WIDTH + j] = img_src.img[i * Image::WIDTH + j];
}
