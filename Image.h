#ifndef ___Class_Image
#define ___Class_Image

#include <string>

class Image {
	int width, height;
	unsigned char* data;
public:
	Image(void) : width(0), height(0), data(NULL) {}
	Image(const std::string& fpath);
	int getWidth(void) const { return width; }
	int getHeight(void) const { return height; }
	unsigned char* getData(void) const { return data; }
	void readImage(const std::string& fpath);
	void readBuiltInImage(const std::string& fpath);
	unsigned char* RGB2RGBA(int width, int height, const unsigned char* src) const;
};

#endif
