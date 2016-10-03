#ifndef ___Class_BlockAir
#define ___Class_BlockAir

#include "Block.h"

class BlockAir : public Block {
public:
	BlockAir() : Block("blockAir") {}
	virtual void registerDisplayList(void);
	virtual void draw(void) const { }
	virtual bool isHit(const Vector& blockPosition, const Vector& playerPosition) { return false; }
};

#endif
