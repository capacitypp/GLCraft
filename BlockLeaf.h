#ifndef ___Class_BlockLeaf
#define ___Class_BlockLeaf

#include "Block.h"
#include "Image.h"

class BlockLeaf : public Block {
	Image texture;
	GLuint textureID;
public:
	BlockLeaf() : Block("blockLeaf") {}
	virtual void readTextures(void);
	virtual void registerTextures(void);
	virtual void registerDisplayList(void);
};

#endif
