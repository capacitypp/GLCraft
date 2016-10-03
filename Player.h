#ifndef ___Class_Player
#define ___Class_Player

#include "global.h"

#include "Vector.h"
#include "ItemSlot.h"
#include "Keyboard.h"

class Player {
	bool _isFlying;
	Vector position;
	Vector velocity;
	Vector eyePosition;
	Vector direction;
	ItemSlot itemSlot;
public:
	Player(void) : _isFlying(false), position(Vector(0, GROUND_HEIGHT, 0)), eyePosition(Vector(0, 1.6, 0)), direction(Vector(0, 0, 1)) { /*for (int i = 0; i < MAX_ITEMSLOT; i++) itemSlot.getItemStack(i) = ItemStack(3, 2);*/ }
	inline bool isFlying(void) const { return _isFlying; }
	inline Vector getPosition(void) const { return position; }
	inline Vector getVelocity(void) const { return velocity; }
	inline Vector getEyePosition(void) const { return eyePosition; }
	inline Vector getDirection(void) const { return direction; }
	inline bool& isFlying(void) { return _isFlying; }
	inline Vector& getPosition(void) { return position; }
	inline Vector& getVelocity(void) { return velocity; }
	inline Vector& getEyePosition(void) { return eyePosition; }
	inline Vector& getDirection(void) { return direction; }
	inline ItemSlot& getItemSlot(void) { return itemSlot; }
	void drawItemSlot(void) const;
	void updateItemSlot(const Keyboard& keyboard);
};

#endif
