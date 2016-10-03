#include "Player.h"

void Player::drawItemSlot(void) const
{
	itemSlot.draw();
}

void Player::updateItemSlot(const Keyboard& keyboard)
{
	itemSlot.updateSelect(keyboard);
}
