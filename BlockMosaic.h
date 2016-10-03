#ifndef ___Class_BlockMosaic
#define ___Class_BlockMosaic

#include "Block.h"
#include "Image.h"

#define BLOCK_MOSAIC_TEXTURE_NUM	10

class BlockMosaic : public Block {
	Image texture[BLOCK_MOSAIC_TEXTURE_NUM];
	GLuint textureID[BLOCK_MOSAIC_TEXTURE_NUM];
	int displayListIDs[BLOCK_MOSAIC_TEXTURE_NUM];
public:
	BlockMosaic() : Block("blockMosaic") {}
	virtual void readTextures(void);
	virtual void registerTextures(void);
	virtual void registerDisplayList(void);
	virtual void draw(int cnt) const;
};

#endif
