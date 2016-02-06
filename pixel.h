#ifndef PIXEL_H
#define PIXEL_H
#include "vector.h"

struct Pixel {
	unsigned char r, g, b;

	
private:
	template<class T>
	unsigned char clampToRange(const T& value) {
		if (value > 255) {
			return 255;
		}
		if (value < 0) {
			return 0;
		}
		return static_cast<unsigned char>(value);
	}
	
public:
	Pixel() {
		r = g = b = 0;
	}
	
	Pixel(unsigned char r, unsigned char g, unsigned char b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
	
	Pixel(Vector vector) {
		r = clampToRange(vector.getX());
		g = clampToRange(vector.getY());
		b = clampToRange(vector.getZ());
	}
	
	Pixel& addWithClamp(const Pixel& pixel) {
		r = clampToRange((int)r + (int)pixel.r);
		g = clampToRange((int)g + (int)pixel.g);
		b = clampToRange((int)b + (int)pixel.b);
		return *this;
	}

};

#endif
