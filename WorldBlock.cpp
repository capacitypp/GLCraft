#include "WorldBlock.h"
#include "BlockManager.h"

bool WorldBlock::isHit(const Vector& blockPosition, const Vector& position) const
{
	if (blockID == 0)
		return false;
	return BlockManager::getBlock(blockID)->isHit(blockPosition, position);
}

bool WorldBlock::isHitPlayer(const Vector& blockPosition, const Vector& playerPosition) const
{
	if (!_isVisible)
		return false;
	return BlockManager::getBlock(blockID)->isHitPlayer(blockPosition, playerPosition);
}

bool WorldBlock::isHitReach(const Vector& blockPosition, const Player& player) const
{
	if (!_isVisible)
		return false;
	if (blockID == 0)
		return false;
	return BlockManager::getBlock(blockID)->isHitReach(blockPosition, player);
}
