#include "ItemStack.h"
#include "BlockManager.h"
#include "StringManager.h"

void ItemStack::draw(void) const
{
	if (blockID == 0)
		return;
	BlockManager::getBlock(blockID).draw();
	StringManager::draw(0, -0.1, num);
}
