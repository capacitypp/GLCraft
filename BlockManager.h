#ifndef ___Class_BlockManager
#define ___Class_BlockManager

#include <iostream>

#include "Block.h"

class BlockManager {
public:
	static Block& blockAir;
	static Block& blockWire;
	static Block& blockDirt;
	static Block& blockWood;
	static Block& blockLeaf;
	static Block& blockCVIP;
	static Block& blockMosaic;
public:
	static void init(void);
	inline static Block& getBlock(int id) {
		switch (id) {
		case 0:
			return blockAir;
		case 1:
			return blockWire;
		case 2:
			return blockDirt;
		case 3:
			return blockWood;
		case 4:
			return blockLeaf;
		case 5:
			return blockCVIP;
		case 6:
			return blockMosaic;
		default:
			std::cerr << "at BlockManager::getBlock()" << std::endl;
			std::cerr << "無効なID : " << id << std::endl;
			exit(-1);
		}
		return blockWire;
	}
};

#endif
