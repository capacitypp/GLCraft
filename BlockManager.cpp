#include "BlockManager.h"
#include "BlockAir.h"
#include "BlockWire.h"
#include "BlockDirt.h"
#include "BlockWood.h"
#include "BlockLeaf.h"
#include "BlockCVIP.h"
#include "BlockMosaic.h"

//ブロックのインスタンス化
Block& BlockManager::blockAir = BlockAir();
Block& BlockManager::blockWire = BlockWire();
Block& BlockManager::blockDirt = BlockDirt();
Block& BlockManager::blockWood = BlockWood();
Block& BlockManager::blockLeaf = BlockLeaf();
Block& BlockManager::blockCVIP = BlockCVIP();
Block& BlockManager::blockMosaic = BlockMosaic();

void BlockManager::init(void)
{
	blockAir.registerDisplayList();
	blockWire.registerDisplayList();
	blockDirt.registerDisplayList();
	blockWood.registerDisplayList();
	blockLeaf.registerDisplayList();
	blockCVIP.registerDisplayList();
	blockMosaic.registerDisplayList();
}
