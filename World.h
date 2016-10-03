#ifndef ___Class_World
#define ___Class_World

#include "WorldBlock.h"
#include "Player.h"
#include "FPS.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "ItemBlock.h"

class World {
	WorldBlock chunk[WORLD_WIDTH][WORLD_HEIGHT][WORLD_WIDTH];
	ItemBlock itemBlocks[MAX_ITEMBLOCK];
	int displayListID;
	Player& player;
	FPS& fps;
	Keyboard& keyboard;
	Mouse& mouse;
	bool _isLastHitReach;
	Vector lastHitBlockPosition;
	double lastHitBlockDistance;
	int lastHitBlockIdx[3];
	int lastHitBlockFace;
	bool isBusyUpdateChunkVisible;
private:
	void generateWood(int x, int y, int z);
	void generateWoods(void);
	void readChunk(void);
	void readItemSlot(void);
	void readPlayerPosition(void);
	void generateChunk(void);
public:
	World(Player& _player, FPS& _fps, Keyboard& _keyboard, Mouse& _mouse) : player(_player), fps(_fps), keyboard(_keyboard), mouse(_mouse) { generateChunk(); }
	virtual void registerDisplayList(void);
	void updateDisplayList(void);
	void threadUpdateChunkVisible(void);
	void updateChunkVisible(void);
	void waitForUpdateChunkVisible(void);
	void draw(void) const;
	bool tryMovePlayer(const Vector& moveVector);
	void movePlayer(void);
	void leftClickPlayer(void);
	void rightClickPlayer(void);
	void keyboardPlayer(void);
	bool isHitChunk(void);
	bool isHitReachChunk(void);
	bool isLastHitReach(void) const { return _isLastHitReach; }
	void drawItemBlocks(void) const;
	void updateItemBlocks(void);
	void spawnItemBlock(int blockID, const Vector& position);
	Vector getLastHitBlockPosition(void) const { return lastHitBlockPosition; }
	Vector getBlockDirection(const Vector& blockPosition);
	void writeChunk(void) const;
	void writeItemSlot(void) const;
	void writePlayerPosition(void) const;
};

#endif
