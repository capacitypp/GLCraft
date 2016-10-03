#include "BlockWood.h"
#include "DisplayListManager.h"
#include "TextureManager.h"

#include <GL/glut.h>

using namespace std;

void BlockWood::readTextures(void)
{
	texture1 = Image("texture/block/blockWood1.ppm");
	texture2 = Image("texture/block/blockWood2.ppm");
}

void BlockWood::registerTextures(void)
{
	textureID1 = TextureManager::getNewID();
	textureID2 = TextureManager::getNewID();
	registerTexture(texture1, textureID1);
	registerTexture(texture2, textureID2);
}

void BlockWood::registerDisplayList(void)
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
	drawTexture(textureID1, geometry[7], geometry[6], geometry[5], geometry[4]);
	drawTexture(textureID2, geometry[4], geometry[5], geometry[1], geometry[0]);
	drawTexture(textureID2, geometry[5], geometry[6], geometry[2], geometry[1]);
	drawTexture(textureID2, geometry[6], geometry[7], geometry[3], geometry[2]);
	drawTexture(textureID2, geometry[7], geometry[4], geometry[0], geometry[3]);
	drawTexture(textureID1, geometry[0], geometry[1], geometry[2], geometry[3]);
	glPopMatrix();
	glEndList();

}
