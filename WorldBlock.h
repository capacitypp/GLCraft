#ifndef ___Class_WorldBlock
#define ___Class_WorldBlock

#include "Vector.h"
#include "Player.h"

class WorldBlock {
	bool _isVisible;
	int blockID;
public:
	WorldBlock(void) : _isVisible(false), blockID(0) {}
	WorldBlock(int _blockID) : _isVisible(true), blockID(_blockID) {}
	inline bool isVisible(void) const { return _isVisible; }
	inline bool& isVisible(void) { return _isVisible; }
	inline int getBlockID(void) const { return blockID; }
	bool isHit(const Vector& blockPosition, const Vector& position) const;
	bool isHitPlayer(const Vector& blockPosition, const Vector& playerPosition) const;
	bool isHitReach(const Vector& blockPosition, const Player& player) const;
};

#endif
