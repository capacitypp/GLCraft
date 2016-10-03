#ifndef ___Class_Block
#define ___Class_Block

#include <string>

#include "Vector.h"
#include "Image.h"
#include "Player.h"

class Block {
protected:
	std::string blockName;
	int displayListID;
	Vector blockSize;
	static int lastHitReachFace;
private:
	int sign(double x) const;
	bool isHit(const Vector& vertex1, const Vector& vertex2, const Vector& vertex3, const Player& player) const;
	bool isHit(const Vector& vertex1, const Vector& vertex2, const Vector& vertex3, const Vector& vertex4, const Vector& position) const;
	bool isHit(const Vector& vertex1, const Vector& vertex2, const Vector& vertex3, const Vector& vertex4, const Player& player, Vector& position) const;
	double getDistance(const Vector& vertex1, const Vector& vertex2, const Vector& vertex3, const Vector& playerPosition) const;
public:
	Block(const std::string& _blockName) : blockName(_blockName), blockSize(Vector(1, 1, 1)) {}
	Block(const std::string& _blockName, const Vector& _blockSize) : blockName(_blockName), blockSize(_blockSize) {}
	inline const std::string& getBlockName(void) const { return blockName; }
	virtual void readTextures(void) {}
	void registerTexture(const Image& image, GLuint id);
	virtual void registerTextures(void) {}
	virtual void registerDisplayList(void) = 0;
	void drawTexture(GLuint id, const Vector& p1, const Vector& p2, const Vector& p3, const Vector& p4) const;
	inline virtual void draw(void) const { if (displayListID != -1) glCallList(displayListID); }
	inline virtual void draw(int) const { draw(); }
	bool isHit(const Vector& blockPosition, const Vector& position) const;
	bool isHitPlayer(const Vector& blockPosition, const Vector& playerPosition) const;
	bool isHitReach(const Vector& blockPosition, const Player& player) const;
	static int getLastHitReachFace(void) { return lastHitReachFace; }
};

#endif
