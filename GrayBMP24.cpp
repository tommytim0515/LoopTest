#include "GrayBMP24.hpp"

uint32_t getRowSize(uint16_t bitsPerPixel, int32_t width) {
    return (bitsPerPixel * width + 31)/32 * 4;
}

uint32_t getRowSize(BMPHeader& header) {
    return getRowSize(header.bitsPerPixel, header.width);
}

std::vector<uint8_t> readBMP(const std::string &filename, BMPHeader& header, std::vector<uint8_t> &colorTable) {
    std::ifstream f(filename, std::ios::binary);
    
    if (f.fail()) {
        std::cout << "The file at the location does not exist." << std::endl;
        return{};
    }
    
    f.read((char *)&header, sizeof(header));
    
    if (header.magic != 19778) {
        std::cout << "The input file is not a BMP" << std::endl;
        return{};
    }
    
    if (header.bitsPerPixel != 24) {
        std::cout << "This is not a 24 bit BMP!" << std::endl;
        std::cout << "bits: " << header.bitsPerPixel << std::endl;
        return{};
    }
    
    std::vector<uint8_t> img;
    uint8_t data[header.imageSize];
    
    colorTable = std::vector<uint8_t>(4 * header.numberOfColors);
    
    f.read((char *)&colorTable[0], 4 * header.numberOfColors);
    
    // find bits
    f.seekg(header.offset);
    
    // read data
    f.read((char *)&data[0], header.imageSize);
    
    // pixels are packed into 4-byte rows, filled bottom up, left to right, unused cells will be padded with some value
    
    int rowSize = getRowSize(header);
    int rowCount = header.imageSize / rowSize;
    
    if (header.height > 0) {
        for (int row = rowCount - 1; row >= 0 ; row-= 3) {
            for (int x = 0; x < header.width; x++) {
                img.push_back(data[row*rowSize + x]);
            }
        }
    } else {
        for (int row = 0; row < rowCount ; ++row) {
            for (int x = 0; x < header.width; x++) {
                img.push_back(data[row*rowSize + x]);
            }
        }
    }
    
    f.close();
    
    return img;
}

void writeBMP(std::string const &filename, BMPHeader &bmpHeader, std::vector<uint8_t> &img, std::vector<uint8_t> &colorTable) {
    std::ofstream f(filename, std::ios::binary);
    f.write((char *)&bmpHeader, sizeof(bmpHeader));
    
    f.seekp(bmpHeader.headerSize + 14);
    f.write((char *)&colorTable[0], colorTable.size() * sizeof(uint8_t));
    
    f.seekp(bmpHeader.offset);
    
    int rowSize = getRowSize(bmpHeader);
    int padSize = rowSize - bmpHeader.width;
    
    if (bmpHeader.height > 0) {
        for (int y = bmpHeader.height - 1; y >= 0 ; y--) {
            for (int x = 0; x < bmpHeader.width; x++) {
                f.put(getPixel(img, x, y, bmpHeader.width));
            }
            for (int i = 0; i < padSize; i++) {
                f.put(0);
            }
        }
    } else {
        for (int y = 0; y < -bmpHeader.height ; ++y) {
            for (int x = 0; x < bmpHeader.width; x++) {
                f.put(getPixel(img, x, y, bmpHeader.width));
            }
            for (int i = 0; i < padSize; i++) {
                f.put(0);
            }
        }
    }
    
    f.close();
}

/**
 * The 8-bit BMPs are indexed images. It means that there are 256 colors in its palette. The conventional 8-bit
 * gray scale images will populate its color table from black to white in a linear fashion.
 *
 * The color table consists of Red, Green, Blue, Alpha values, in the RGBA32 format. However, the A is set to 0 since
 * most OSs disallows this feature.
 *
 * @return color table section of data in bytes
 */
std::vector<uint8_t> makeColorTable() {
    std::vector<uint8_t> colorTable;
    
    for (int i = 0; i < 256; i++) {
        colorTable.push_back((uint8_t) i);
        colorTable.push_back((uint8_t) i);
        colorTable.push_back((uint8_t) i);
        colorTable.push_back(0);
    }
    
    return colorTable;
}


std::vector<uint8_t> GrayBMP::grayScaleColorTable;

GrayBMP::GrayBMP() {
    if (GrayBMP::grayScaleColorTable.empty()) {
        GrayBMP::grayScaleColorTable = makeColorTable();
    }
}

GrayBMP::GrayBMP(std::string const &filename): GrayBMP() {
    img = readBMP(filename, bmpHeader, colorTable);
}

void GrayBMP::save(std::string const &filename) {
    writeBMP(filename, bmpHeader, img, colorTable);
}

GrayBMP::GrayBMP(int32_t width, int32_t height, uint8_t backgroundColor): GrayBMP() {
    static uint32_t fileHeaderSize = 14;
    static uint32_t dibHeaderSize = 40;
    static uint32_t colorTableSize = 256 * 4;
    static uint32_t offset = fileHeaderSize + dibHeaderSize + colorTableSize;
    
    uint32_t rowSize = getRowSize(8, width);
    uint32_t imageSize = rowSize * height;
    uint32_t fileSize = imageSize + offset;
    
    colorTable = grayScaleColorTable;
    
    bmpHeader = {
        19778,
        fileSize,
        0,
        0,
        offset,
        dibHeaderSize,
        width,
        height,
        1,
        8,
        0,
        imageSize,
        2834,
        2834,
        256,
        256
    };
    
    for (int i = 0; i < width * height; i++) {
        img.push_back(backgroundColor);
    }
}

uint8_t &GrayBMP::getPixel(int x, int y) {
    return img[y*bmpHeader.width+x];
}

std::vector<int> ReadBMP(const char* filename)
{
    int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];

    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);
	std::vector<int> datum;

	for (int i = 0; i < size; i+=3) {
		datum.push_back(data[i]);
	}

    // for(i = 0; i < size; i += 3)
    // {
    //         unsigned char tmp = data[i];
    //         data[i] = data[i+2];
    //         data[i+2] = tmp;
    // }

    return datum;
}