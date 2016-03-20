#include "bmp.h"
#include "vector.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>

const int BPP=32;

struct bmpfile_magic {
  unsigned char magic[2];
};
 
struct bmpfile_header {
  int filesz;
  short creator1;
  short creator2;
  int bmp_offset;
};

typedef struct {
  int header_sz;
  int width;
  int height;
  short nplanes;
  short bitspp;
  int compress_type;
  int bmp_bytesz;
  int hres;
  int vres;
  int ncolors;
  int nimpcolors;
} BITMAPINFOHEADER;


Bmp::Bmp(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
	image = new Pixel*[height];
	for (unsigned int i = 0; i < height; ++i) {
		image[i] = new Pixel[width];
	}
}

Bmp::~Bmp() {
	for (unsigned int i = 0; i < height; ++i) {
		delete[] image[i];
	}
	delete[] image;
}

void Bmp::fill(const Pixel& color) {
	for (unsigned int i = 0; i < height; ++i) {
		for (unsigned int j = 0; j < width; ++j) {
			image[i][j] = color;
		}
	}
}

void Bmp::setPixel(unsigned int x, unsigned int y, const Pixel& color) {
	if (x < 0 || y < 0 || x >= width || y >= height) {
		return;
	}
	image[y][x] = color;
}

void Bmp::addToPixel(unsigned int x, unsigned int y, const Pixel& color) {
	if (x < 0 || y < 0 || x >= width || y >= height) {
		return;
	}
	image[y][x].addWithClamp(color);
}

void Bmp::save(const std::string& fileName) {
	unsigned long long size = calculateSizeInBytes(width, height);
	if (size < MAX_BMP_SIZE_IN_BYTES) {
		saveInternal(fileName + ".bmp", 0, height, 0, width);
	} else {
		unsigned int numberOfPixels = static_cast<unsigned int>(sqrt((MAX_BMP_SIZE_IN_BYTES - 54L) * 8.0 / BPP));
		unsigned int numberOfHorizontalTiles = static_cast<unsigned int>(ceil(static_cast<double>(width) / numberOfPixels));
		unsigned int numberOfVerticalTiles = static_cast<unsigned int>(ceil(static_cast<double>(height) / numberOfPixels));
		std::cout << "PixelCount: " << numberOfHorizontalTiles << ' ' << numberOfVerticalTiles << std::endl;
		for (unsigned int i = 0; i < numberOfVerticalTiles; ++i) {
			for (unsigned int j = 0; j < numberOfHorizontalTiles; ++j) {
				unsigned int startY = i * numberOfPixels;
				unsigned int startX = j * numberOfPixels;
				unsigned int endX = (startX + numberOfPixels < width ? startX + numberOfPixels : width);
				unsigned int endY = (startY + numberOfPixels < height ? startY + numberOfPixels : height); 
				std::stringstream ss;
				ss << fileName << "_part_" << i << "_" << j << ".bmp";
				saveInternal(ss.str(), startY, endY, startX, endX);
			}
		}
	}
}

void Bmp::saveInternal(const std::string& fileName, unsigned int startY, unsigned int endY, unsigned int startX, unsigned int endX) {
	int tileHeight = endY - startY;
	int tileWidth = endX - startX;
	std::ofstream out(fileName.c_str(),std::ios::binary);
	bmpfile_magic bm={'B','M'};
	bmpfile_header bh={static_cast<int>(calculateSizeInBytes(tileWidth, tileHeight)), 0, 0, 54};
	BITMAPINFOHEADER bhi={40, tileWidth, tileHeight, 1, BPP, 0, ((BPP*tileWidth)/8)*tileHeight, 2750, 2750, 0, 0};
	out.write((char*)&bm,sizeof(bm));
	out.write((char*)&bh,sizeof(bh));
	out.write((char*)&bhi,sizeof(bhi));
	for(unsigned int i = startY; i < endY; i++) {
		for(unsigned int j = startX; j < endX; j++)
		{
				out.put(image[i][j].b);
				out.put(image[i][j].g);
				out.put(image[i][j].r);
				out.put((char)255);
		}
	}
	out.close();
}

unsigned long long Bmp::calculateSizeInBytes(unsigned int w, unsigned int h) {
	return 54L + ((BPP * w) / 8) * h;
}