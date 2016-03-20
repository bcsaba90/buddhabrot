#ifndef BMP_H
#define BMP_H
#include "pixel.h"
#include <string>
#include <limits>

const unsigned long long MAX_BMP_SIZE_IN_BYTES = std::numeric_limits<int>::max();

class Bmp {
	private:
		unsigned int width, height;
		Pixel** image;
		void saveInternal(const std::string& fileName, unsigned int startY, unsigned int endY, unsigned int startX, unsigned int endX);
		unsigned long long calculateSizeInBytes(unsigned int w, unsigned int h);
	public:
		Bmp(unsigned int width, unsigned int height);
		~Bmp();
		
		void fill(const Pixel& color);
		void setPixel(unsigned int x, unsigned int y, const Pixel& color);
		void addToPixel(unsigned int x, unsigned int y, const Pixel& color);
		void save(const std::string& fileName);
};
#endif
