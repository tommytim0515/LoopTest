#include <iostream>
#include <algorithm>
#include <functional>
#include <cstdint>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include "GrayBMP24.hpp"
#include "functions.h"
//#include "declaration.h"
#include "imageProc.h"
#include "track_processing.hpp"
#include "loop.hpp"

using namespace std;

int main(){
	for (int i = 1; i < 24; ++i) {
		std::cout << "No. " << to_string(i) << std::endl;
		string name("pictures/" + to_string(i) + ".bmp");
		const char* Name = name.c_str();
		int counter = 0;
		std::vector<int> picture = ReadBMP(Name);
		GrayBMP tmp(Image::WIDTH, Image::HEIGHT);
		uint8_t img_arr[Image::WIDTH * Image::HEIGHT];
		for (int i = 0; i < Image::HEIGHT; i ++) {
			for (int j = 0; j < Image::WIDTH; j ++) {
				tmp.img[i*Image::WIDTH + j] = picture[Image::WIDTH*Image::HEIGHT - i*Image::WIDTH + j - 1];//picture[IMAGE_WIDTH*IMAGE_HEIGHT - i*IMAGE_WIDTH + j - 1];
				//img_arr[i*Image::WIDTH + j] = picture[Image::WIDTH*Image::HEIGHT - i*Image::WIDTH + j - 1];
			}
		}
		//Threshold(tmp);
		for (int i = 0; i < Image::HEIGHT; i ++) {
			for (int j = 0; j < Image::WIDTH; j ++) {
				//tmp.img[i*Image::WIDTH + j] = picture[Image::WIDTH*Image::HEIGHT - i*Image::WIDTH + j - 1];//picture[IMAGE_WIDTH*IMAGE_HEIGHT - i*IMAGE_WIDTH + j - 1];
				img_arr[i*Image::WIDTH + j] = picture[Image::WIDTH*Image::HEIGHT - i*Image::WIDTH + j - 1];
			}
		}
		//PRE_LOOP_R();
		//Threshold(tmp);
		//sobel(tmp);
		CAM_BUFFER = img_arr;
		if (i < 5)
			PRE_LOOP_R(tmp);
		else if (i >= 5 && i <= 8)
			INTO_LOOP_R(tmp);
		else if (i >= 9 && i <= 11)
			IN_LOOP_R(tmp);
		else if (i > 11 && i <= 13)
			OUT_LOOP_R(tmp);
		else if (i > 13 && i < 18)
			END_LOOP_R(tmp);
		else if (i == 18)
			LOOP_TO_NOR_R(tmp);
		else if (i > 18 && i < 23)
			PRE_LOOP_L(tmp);
		//makeSmallDot(tmp, Image::WIDTH / 2, Image::HEIGHT / 2);
		tmp.save("out/" + to_string(i) + ".bmp");
	}
	std::cout << "Done" << std::endl;
    return 0;
}
