#ifndef ___Class_BlockDirt
#define ___Class_BlockDirt

#include "Block.h"
#include "Image.h"

class BlockDirt : public Block {
	Image texture1, texture2, texture3;
	GLuint textureID1, textureID2, textureID3;
public:
	BlockDirt() : Block("blockDirt") {}
	virtual void readTextures(void);
	virtual void registerTextures(void);
	virtual void registerDisplayList(void);
};

#endif
