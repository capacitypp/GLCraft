#include "ItemBlock.h"
#include "BlockManager.h"

#include <GL/glut.h>

void ItemBlock::draw(void) const
{
	glPushMatrix();
	glTranslated(position.getX(), position.getY(), position.getZ());
	glRotated(angle, 0, 1, 0);
	glScaled(0.3, 0.3, 0.3);
	glTranslated(-0.5, -0.5, -0.5);
	BlockManager::getBlock(blockID).draw();
	glPopMatrix();
}
