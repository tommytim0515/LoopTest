/**
 * GrayBMP.h
 *
 * @author Daniel Cheung
 */

#ifndef __HELPER_H_
#define __HELPER_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>

#pragma pack(push, 1)
/**
 * This packed struct is the header of the BMP file, before the color table and the content of the image.
 */
struct BMPHeader {
    uint16_t magic;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t colorPlanes;
    uint16_t bitsPerPixel;
    uint32_t compressionMethod;
    uint32_t imageSize;
    int32_t horizontalResolution;
    int32_t verticalResolution;
    uint32_t numberOfColors;
    uint32_t numberOfImportantColors;
};
#pragma pack(pop)

/**
 * Reads the BMP file and converts it into a 1D vector represented in a row-by-row then column-by-column fashion. The
 * vector starts at the top left of the image, then scans horizontally, wrapping to the next row until the bottom right
 * of the image.
 *
 * @param filename      BMP filename
 * @param bmpHeader     Reference variable of bmpHeader
 * @param colorTable    Reference variable of colorTable
 */
std::vector<uint8_t> readBMP(std::string const &filename, BMPHeader& bmpHeader, std::vector<uint8_t> &colorTable);

/**
 * Writes a BMP file.
 *
 * @param filename      BMP filename
 * @param bmpHeader     Reference variable of bmpHeader
 * @param img           Reference variable of the image vector
 * @param colorTable    Reference variable of colorTable
 */
void writeBMP(std::string const &filename, BMPHeader& bmpHeader, std::vector<uint8_t>& img, std::vector<uint8_t> &colorTable);

/**
 * Retrieves pixel information of image. Top left hand corner is the origin (0, 0) and the bottom right hand corner is
 * (width-1, height-1).
 *
 * @param  img      image vector
 * @param  x        x-coordinate of desired location
 * @param  y        y-coordinate of desired location
 * @param  width    width of image
 * @return          The value of the selected pixel
 */
inline uint8_t getPixel(const std::vector<uint8_t>& img, int x, int y, int width) {
    return img[y*width+x];
}

/**
 * This container class represents an 8-bit gray scale image.
 */
class GrayBMP {
public:
    /**
     * BMP file header of the opened BMP file.
     */
    BMPHeader bmpHeader{};
    /**
     * Color table providing indexed colors. By default ranges from black to white linearly.
     */
    std::vector<uint8_t> colorTable;
    /**
     * The image stored as a 1D array. Values ranging from 0~255
     */
    std::vector<uint8_t> img;
    
    static std::vector<uint8_t> grayScaleColorTable;
    
    /**
     * Opens a BMP file with a filename.
     */
    explicit GrayBMP(std::string const &filename);
    
    /**
     * Creates a new blank BMP with given width and height.
     */
    GrayBMP(int32_t width, int32_t height, uint8_t backgroundColor = 0);
    
    /**
     * Saves the image to another BMP file.
     */
    void save(std::string const &filename);
    
    /**
     * Retrieves pixel information of image. Top left hand corner is the origin (0, 0) and the bottom right hand corner
     * is (width-1, height-1).
     * @param x x-coordinate of desired location
     * @param y y-coordinate of desired location
     * @return Reference to gray value of the pixel location
     */
    uint8_t& getPixel(int x, int y);
    
private:
    /**
     * This constructor initializes the static variables of the class.
     */
    explicit GrayBMP();
};

std::vector<int> ReadBMP(const char* filename);

#endif // __HELPER_H_
