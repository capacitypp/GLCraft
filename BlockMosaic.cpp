#include "BlockMosaic.h"
#include "DisplayListManager.h"
#include "TextureManager.h"

#include <gl\glut.h>

using namespace std;

void BlockMosaic::readTextures(void)
{
	for (int i = 0; i < BLOCK_MOSAIC_TEXTURE_NUM; i++) {
		string fpath("texture\\block\\blockMosaic");
		fpath += to_string(i + 1);
		fpath += ".ppm";
		texture[i] = Image(fpath);
	}
}

void BlockMosaic::registerTextures(void)
{
	for (int i = 0; i < BLOCK_MOSAIC_TEXTURE_NUM; i++) {
		textureID[i] = TextureManager::getNewID();
		registerTexture(texture[i], textureID[i]);
	}
}

void BlockMosaic::registerDisplayList(void)
{
	static Vector geometry[] = {
		Vector(-0.01, -0.01, -0.01),
		Vector(1.01, -0.01, -0.01),
		Vector(1.01, -0.01, 1.01),
		Vector(-0.01, -0.01, 1.01),
		Vector(-0.01, 1.01, -0.01),
		Vector(1.01, 1.01, -0.01),
		Vector(1.01, 1.01, 1.01),
		Vector(-0.01, 1.01, 1.01),
	};

	readTextures();
	registerTextures();

	for (int i = 0; i < BLOCK_MOSAIC_TEXTURE_NUM; i++) {
		displayListIDs[i] = DisplayListManager::getNewID();
		glNewList(displayListIDs[i], GL_COMPILE);
		//アルファブレンドを使う（Worldクラスで描画されないため）
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glPushMatrix();
		glScaled(blockSize.getX(), blockSize.getY(), blockSize.getZ());
		drawTexture(textureID[i], geometry[7], geometry[6], geometry[5], geometry[4]);
		drawTexture(textureID[i], geometry[4], geometry[5], geometry[1], geometry[0]);
		drawTexture(textureID[i], geometry[5], geometry[6], geometry[2], geometry[1]);
		drawTexture(textureID[i], geometry[6], geometry[7], geometry[3], geometry[2]);
		drawTexture(textureID[i], geometry[7], geometry[4], geometry[0], geometry[3]);
		drawTexture(textureID[i], geometry[0], geometry[1], geometry[2], geometry[3]);
		glPopMatrix();
		glDisable(GL_BLEND);
		glEndList();
	}
}

void BlockMosaic::draw(int cnt) const
{
	//cntの値に応じてテクスチャを変えて描画
	cnt %= 15;
	glCallList(displayListIDs[(int)(cnt / 15.0 * 10)]);
}
