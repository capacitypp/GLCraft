#include <iostream>

#include "TextureManager.h"

int TextureManager::cnt = 0;
GLuint TextureManager::IDs[MAX_TEXTURE];

using namespace std;

void TextureManager::init(void)
{
	glGenTextures(MAX_TEXTURE, IDs);
}

GLuint TextureManager::getNewID(void)
{
	if (cnt >= MAX_TEXTURE) {
		cerr << "at TextureManager::getNewID()" << endl;
		cerr << "テクスチャIDが不足しています" << endl;
		exit(-1);
	}
	return IDs[cnt++];
}
