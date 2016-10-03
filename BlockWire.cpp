#include "BlockWire.h"
#include "DisplayListManager.h"

#include <gl\glut.h>

using namespace std;

void BlockWire::registerDisplayList(void)
{
	displayListID = DisplayListManager::getNewID();
	glNewList(displayListID, GL_COMPILE);
	glPushMatrix();
	glTranslated(blockSize.getX() / 2, blockSize.getY() / 2, blockSize.getZ() / 2);
	glScaled(blockSize.getX(), blockSize.getY(), blockSize.getZ());
	glutWireCube(1.0);
	glPopMatrix();
	glEndList();

}
