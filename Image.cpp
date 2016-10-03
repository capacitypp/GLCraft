#include <iostream>
#include <fstream>

#include "Image.h"
#include "BuiltInImage.h"

using namespace std;

Image::Image(const std::string& fpath)
{
	ifstream file;
	file.open(fpath.c_str());
	//テクスチャファイルが存在しなければ，内蔵テクスチャを使う
	if (file.fail())
		readBuiltInImage(fpath);
	else
		readImage(fpath);
}

//テクスチャのファイルからの読み込み
void Image::readImage(const std::string& fpath)
{
	cout << "fpath : " << fpath << endl;
	ifstream file;
	file.open(fpath.c_str(), ifstream::in | ifstream::binary);
	string format;
	file >> format;
	unsigned char sharp;
	file >> sharp;
	string comment;
	getline(file, comment);
	file >> width;
	file >> height;
	int level;
	file >> level;
	data = new unsigned char[width * height * 4];
	file.get();
	unsigned char *ptr = data;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			unsigned char* ptr2 = ptr;
			for (int k = 0; k < 3; k++)
				*ptr++ = file.get();
			*ptr++ = 255;
			if (ptr2[0] == 255 && ptr2[1] == 255 && ptr2[2] == 255)
				ptr2[3] = 0;
		}
	}
}

//テクスチャのソースファイルからの読み込み
void Image::readBuiltInImage(const std::string& fpath)
{
	cout << "built-in image : " << fpath << endl;
	if (fpath == "texture/block/blockCVIP.ppm") {
		width = blockCVIPWidth;
		height = blockCVIPHeight;
		data = RGB2RGBA(width, height, blockCVIPData);
		return;
	}
	if (fpath == "texture/block/blockDirt1.ppm") {
		width = blockDirt1Width;
		height = blockDirt1Height;
		data = RGB2RGBA(width, height, blockDirt1Data);
		return;
	}
	if (fpath == "texture/block/blockDirt2.ppm") {
		width = blockDirt2Width;
		height = blockDirt2Height;
		data = RGB2RGBA(width, height, blockDirt2Data);
		return;
	}
	if (fpath == "texture/block/blockDirt3.ppm") {
		width = blockDirt3Width;
		height = blockDirt3Height;
		data = RGB2RGBA(width, height, blockDirt3Data);
		return;
	}
	if (fpath == "texture/block/blockLeaf.ppm") {
		width = blockLeafWidth;
		height = blockLeafHeight;
		data = RGB2RGBA(width, height, blockLeafData);
		return;
	}
	if (fpath == "texture/block/blockMosaic1.ppm") {
		width = blockMosaic1Width;
		height = blockMosaic1Height;
		data = RGB2RGBA(width, height, blockMosaic1Data);
		return;
	}
	if (fpath == "texture/block/blockMosaic2.ppm") {
		width = blockMosaic2Width;
		height = blockMosaic2Height;
		data = RGB2RGBA(width, height, blockMosaic2Data);
		return;
	}
	if (fpath == "texture/block/blockMosaic3.ppm") {
		width = blockMosaic3Width;
		height = blockMosaic3Height;
		data = RGB2RGBA(width, height, blockMosaic3Data);
		return;
	}
	if (fpath == "texture/block/blockMosaic4.ppm") {
		width = blockMosaic4Width;
		height = blockMosaic4Height;
		data = RGB2RGBA(width, height, blockMosaic4Data);
		return;
	}
	if (fpath == "texture/block/blockMosaic5.ppm") {
		width = blockMosaic5Width;
		height = blockMosaic5Height;
		data = RGB2RGBA(width, height, blockMosaic5Data);
		return;
	}
	if (fpath == "texture/block/blockMosaic6.ppm") {
		width = blockMosaic6Width;
		height = blockMosaic6Height;
		data = RGB2RGBA(width, height, blockMosaic6Data);
		return;
	}
	if (fpath == "texture/block/blockMosaic7.ppm") {
		width = blockMosaic7Width;
		height = blockMosaic7Height;
		data = RGB2RGBA(width, height, blockMosaic7Data);
		return;
	}
	if (fpath == "texture/block/blockMosaic8.ppm") {
		width = blockMosaic8Width;
		height = blockMosaic8Height;
		data = RGB2RGBA(width, height, blockMosaic8Data);
		return;
	}
	if (fpath == "texture/block/blockMosaic9.ppm") {
		width = blockMosaic9Width;
		height = blockMosaic9Height;
		data = RGB2RGBA(width, height, blockMosaic9Data);
		return;
	}
	if (fpath == "texture/block/blockMosaic10.ppm") {
		width = blockMosaic10Width;
		height = blockMosaic10Height;
		data = RGB2RGBA(width, height, blockMosaic10Data);
		return;
	}
	if (fpath == "texture/block/blockWood1.ppm") {
		width = blockWood1Width;
		height = blockWood1Height;
		data = RGB2RGBA(width, height, blockWood1Data);
		return;
	}
	if (fpath == "texture/block/blockWood2.ppm") {
		width = blockWood2Width;
		height = blockWood2Height;
		data = RGB2RGBA(width, height, blockWood2Data);
		return;
	}
	if (fpath == "texture/itemSlot/itemSlot1.ppm") {
		width = itemSlot1Width;
		height = itemSlot1Height;
		data = RGB2RGBA(width, height, itemSlot1Data);
		return;
	}
	if (fpath == "texture/itemSlot/itemSlot2.ppm") {
		width = itemSlot2Width;
		height = itemSlot2Height;
		data = RGB2RGBA(width, height, itemSlot2Data);
		return;
	}

}

//RGBデータをRGBAデータに変換する
unsigned char* Image::RGB2RGBA(int width, int height, const unsigned char* src) const
{
	unsigned char* data = new unsigned char[height * width * 4];
	unsigned char* ptr = data;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			unsigned char* ptr2 = ptr;
			for (int k = 0; k < 3; k++)
				*ptr++ = *src++;
			*ptr++ = 255;
			if (ptr2[0] == 255 && ptr2[1] == 255 && ptr2[2] == 255)
				ptr2[3] = 0;
		}
	}
	return data;
}
