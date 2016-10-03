#include "BlockLeaf.h"
#include "DisplayListManager.h"
#include "TextureManager.h"

#include <GL/glut.h>

using namespace std;

void BlockLeaf::readTextures(void)
{
	texture = Image("texture\\block\\blockLeaf.ppm");
}

void BlockLeaf::registerTextures(void)
{
	textureID = TextureManager::getNewID();
	registerTexture(texture, textureID);
}

void BlockLeaf::registerDisplayList(void)
{
	static Vector geometry[] = {
		Vector(0, 0, 0),
		Vector(1, 0, 0),
		Vector(1, 0, 1),
		Vector(0, 0, 1),
		Vector(0, 1, 0),
		Vector(1, 1, 0),
		Vector(1, 1, 1),
		Vector(0, 1, 1),
	};

	readTextures();
	registerTextures();

	displayListID = DisplayListManager::getNewID();
	glNewList(displayListID, GL_COMPILE);
	glPushMatrix();
	glScaled(blockSize.getX(), blockSize.getY(), blockSize.getZ());
	drawTexture(textureID, geometry[7], geometry[6], geometry[5], geometry[4]);
	drawTexture(textureID, geometry[4], geometry[5], geometry[1], geometry[0]);
	drawTexture(textureID, geometry[5], geometry[6], geometry[2], geometry[1]);
	drawTexture(textureID, geometry[6], geometry[7], geometry[3], geometry[2]);
	drawTexture(textureID, geometry[7], geometry[4], geometry[0], geometry[3]);
	drawTexture(textureID, geometry[0], geometry[1], geometry[2], geometry[3]);
	glPopMatrix();
	glEndList();

}
