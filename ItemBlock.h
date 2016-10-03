#ifndef ___Class_ItemBlock
#define ___Class_ItemBlock

#include "Vector.h"

class ItemBlock {
	bool flag;
	int blockID;
	double angle;
	Vector position;
	Vector velocity;
public:
	ItemBlock(void) : flag(false) {}
	ItemBlock(int _blockID, const Vector& _position) : flag(true), blockID(_blockID), angle(0.0), position(_position) {}
	bool getFlag(void) const { return flag; }
	int getBlockID(void) const { return blockID; }
	Vector getPosition(void) const { return position; }
	Vector getVelocity(void) const { return velocity; }
	int& getBlockID(void) { return blockID; }
	Vector& getPosition(void) { return position; }
	Vector& getVelocity(void) { return velocity; }
	void update(void) { angle += 1.0; }
	void draw(void) const;
};

#endif
