#ifndef ___Class_BlockCVIP
#define ___Class_BlockCVIP

#include "Block.h"
#include "Image.h"

class BlockCVIP : public Block {
	Image texture;
	GLuint textureID;
public:
	BlockCVIP() : Block("blockCVIP") {}
	virtual void readTextures(void);
	virtual void registerTextures(void);
	virtual void registerDisplayList(void);
};

#endif
