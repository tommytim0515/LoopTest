#include "image.hpp"
#include "global.hpp"

namespace Image {
	
	__attribute__((section(".m_data2"))) uint8_t displayImage[DISPLAY_HEIGHT][DISPLAY_WIDTH] __attribute__((aligned(64)));
	__attribute__((section(".m_data2"))) uint8_t fullResTempImage[HEIGHT][WIDTH] __attribute__((aligned(64)));
	
	inline uint8_t getPixelAlt(const uint8_t image[HEIGHT][WIDTH], int y, int x) {
		if (y < 0 || y >= HEIGHT || x < 0 || x >= WIDTH) return 0;
		return image[y][x];
	}
	inline uint8_t getPixel(const uint8_t image[HEIGHT][WIDTH], int y, int x) {
		return getPixelAlt(image, y, x);
		/*int sum = 0;
		for (int i = y-1; i <= y+1; i++)
			for (int j = x-1; j <= x+1; j++)
				sum += getPixelAlt(image, i, j);
		return sum / 9;*/
	}
	inline uint8_t getPixel(const uint8_t image[HEIGHT][WIDTH], std::pair<int, int> yx) { return getPixel(image, yx.first, yx.second); }
	inline uint8_t getPixel(const uint8_t image[SIZE], int y, int x) { return getPixel((const uint8_t (*)[WIDTH])image, y, x); }
	inline uint8_t getPixel(const uint8_t image[SIZE], std::pair<int, int> yx) { return getPixel((const uint8_t (*)[WIDTH])image, yx.first, yx.second); }
	
	int getDelta(const uint8_t image[HEIGHT][WIDTH], int y, int x, std::pair<int, int> dir) {
		int ret = -4;
		if (dir == mp(0, -1)) {
			ret = ((int)getPixel(image, y-1, x-1) + 2*(int)getPixel(image, y, x-1) + (int)getPixel(image, y+1, x-1)) -
				((int)getPixel(image, y-1, x+1) + 2*(int)getPixel(image, y, x+1) + (int)getPixel(image, y+1, x+1));
		} else if (dir == mp(0, 1)) {
			return -getDelta(image, y, x, mp(0, -1));
		} else if (dir == mp(-1, 0)) {
			ret = ((int)getPixel(image, y-1, x-1) + 2*(int)getPixel(image, y-1, x) + (int)getPixel(image, y-1, x+1)) -
				((int)getPixel(image, y+1, x-1) + 2*(int)getPixel(image, y+1, x) + (int)getPixel(image, y+1, x+1));
		} else if (dir == mp(1, 0)) {
			return -getDelta(image, y, x, mp(-1, 0));
		} else {
			ERROR("invld delta dir");
		}
		return ret / 4;
	}
	int getDelta(const uint8_t image[HEIGHT][WIDTH], std::pair<int, int> yx, std::pair<int, int> dir) { return getDelta(image, yx.first, yx.second, dir); }
	int getDelta(const uint8_t image[SIZE], int y, int x, std::pair<int, int> dir) { return getDelta((const uint8_t (*)[WIDTH])image, y, x, dir); }
	int getDelta(const uint8_t image[SIZE], std::pair<int, int> yx, std::pair<int, int> dir) { return getDelta((const uint8_t (*)[WIDTH])image, yx.first, yx.second, dir); }
	
	
	bool isEdge(const uint8_t image[HEIGHT][WIDTH], int y, int x) {
		if (y < 0 || y >= HEIGHT || x < 0 || x >= WIDTH) return true;
		int yDelta = getDelta(image, y, x, std::make_pair(1, 0));
		int xDelta = getDelta(image, y, x, std::make_pair(0, 1));
		return ( abs(yDelta) + abs(xDelta) ) / 2 > DELTA_THRES;
	}
	bool isEdge(const uint8_t image[HEIGHT][WIDTH], std::pair<int, int> yx) { return isEdge(image, yx.first, yx.second); }
	bool isEdge(const uint8_t image[SIZE], int y, int x) { return isEdge((const uint8_t (*)[WIDTH])image, y, x); }
	bool isEdge(const uint8_t image[SIZE], std::pair<int, int> yx) { return isEdge((const uint8_t (*)[WIDTH])image, yx.first, yx.second); }
	
}