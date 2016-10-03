#include "BlockManager.h"
#include "BlockAir.h"
#include "BlockWire.h"
#include "BlockDirt.h"
#include "BlockWood.h"
#include "BlockLeaf.h"
#include "BlockCVIP.h"
#include "BlockMosaic.h"

//ブロックのインスタンス化
Block* BlockManager::blockAir = new BlockAir();
Block* BlockManager::blockWire = new BlockWire();
Block* BlockManager::blockDirt = new BlockDirt();
Block* BlockManager::blockWood = new BlockWood();
Block* BlockManager::blockLeaf = new BlockLeaf();
Block* BlockManager::blockCVIP = new BlockCVIP();
Block* BlockManager::blockMosaic = new BlockMosaic();

void BlockManager::init(void)
{
	blockAir->registerDisplayList();
	blockWire->registerDisplayList();
	blockDirt->registerDisplayList();
	blockWood->registerDisplayList();
	blockLeaf->registerDisplayList();
	blockCVIP->registerDisplayList();
	blockMosaic->registerDisplayList();
}
