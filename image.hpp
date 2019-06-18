#pragma once

#include <bitset>
#include <cstring>
#include <limits>

namespace Image {
	const int WIDTH = 184;
	const int HEIGHT = 120;
	//const int WIDTH = 376;
	//const int HEIGHT = 240;
	//const int WIDTH = 720;
	//const int HEIGHT = 480;
	const int SIZE = WIDTH * HEIGHT;
	const int DISPLAY_WIDTH = WIDTH;
	const int DISPLAY_HEIGHT = HEIGHT;
	const int DISPLAY_SIZE = DISPLAY_WIDTH * DISPLAY_HEIGHT;
	
	extern uint8_t displayImage[DISPLAY_HEIGHT][DISPLAY_WIDTH];
	extern uint8_t fullResTempImage[HEIGHT][WIDTH];

	const int DELTA_THRES = 26;//30;//46;//50;
	
	uint8_t getPixel(const uint8_t image[HEIGHT][WIDTH], int y, int x);
	uint8_t getPixel(const uint8_t image[HEIGHT][WIDTH], std::pair<int, int> yx);
	uint8_t getPixel(const uint8_t image[SIZE], int y, int x);
	uint8_t getPixel(const uint8_t image[SIZE], std::pair<int, int> yx);
	int getDelta(const uint8_t image[HEIGHT][WIDTH], int y, int x, std::pair<int, int> dir);
	int getDelta(const uint8_t image[HEIGHT][WIDTH], std::pair<int, int> yx, std::pair<int, int> dir);
	int getDelta(const uint8_t image[SIZE], int y, int x, std::pair<int, int> dir);
	int getDelta(const uint8_t image[SIZE], std::pair<int, int> yx, std::pair<int, int> dir);
	bool isEdge(const uint8_t image[HEIGHT][WIDTH], int y, int x);
	bool isEdge(const uint8_t image[HEIGHT][WIDTH], std::pair<int, int> yx);
	bool isEdge(const uint8_t image[SIZE], int y, int x);
	bool isEdge(const uint8_t image[SIZE], std::pair<int, int> yx);
	
}
