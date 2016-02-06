#ifndef BMP_H
#define BMP_H
#include "pixel.h"
#include <string>

class Bmp {
	private:
		int width, height;
		Pixel** image;
	public:
		Bmp(int width, int height);
		~Bmp();
		
		void fill(const Pixel& color);
		void setPixel(int x, int y, const Pixel& color);
		void addToPixel(int x, int y, const Pixel& color);
		void save(std::string fileName);
};
#endif
