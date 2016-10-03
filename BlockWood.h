#ifndef ___Class_BlockWood
#define ___Class_BlockWood

#include "Block.h"
#include "Image.h"

class BlockWood : public Block {
	Image texture1, texture2;
	GLuint textureID1, textureID2;
public:
	BlockWood() : Block("blockWood") {}
	virtual void readTextures(void);
	virtual void registerTextures(void);
	virtual void registerDisplayList(void);
};

#endif
