#ifndef ___Class_BlockWire
#define ___Class_BlockWire

#include "Block.h"
#include "Image.h"

class BlockWire : public Block {
public:
	BlockWire() : Block("blockWire") {}
	virtual void registerDisplayList(void);

};

#endif
