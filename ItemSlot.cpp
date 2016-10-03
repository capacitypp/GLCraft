#include "ItemSlot.h"
#include "TextureManager.h"
#include "BlockDirt.h"
#include "DisplayListManager.h"
#include "StringManager.h"

#include "BlockManager.h"

void ItemSlot::registerTexture(void)
{
	textureID1 = TextureManager::getNewID();
	textureID2 = TextureManager::getNewID();
	//ブロックのテクスチャ登録を流用
	Block* block = BlockManager::getBlock(2);
	block->registerTexture(texture1, textureID1);
	block->registerTexture(texture2, textureID2);
	displayListID1 = DisplayListManager::getNewID();
	displayListID2 = DisplayListManager::getNewID();
	glNewList(displayListID1, GL_COMPILE);
	glPushMatrix();
	glScaled(2.5, 2.5, 2.5);
	block->drawTexture(textureID1, Vector(0, 0, 0), Vector(1, 0, 0), Vector(1, 1, 0), Vector(0, 1, 0));
	glPopMatrix();
	glEndList();
	glNewList(displayListID2, GL_COMPILE);
	glPushMatrix();
	glScaled(2.5, 2.5, 2.5);
	block->drawTexture(textureID2, Vector(0, 0, 0), Vector(1, 0, 0), Vector(1, 1, 0), Vector(0, 1, 0));
	glPopMatrix();
	glEndList();
}

//選択されているスロットのアイテムを使う
void ItemSlot::useItem(void)
{
	ItemStack& itemStack = itemStacks[selected];
	if (!itemStack.getNum())
		return;
	if (--itemStack.getNum() == 0)
		itemStack = ItemStack();
}

//アイテムを追加する
bool ItemSlot::addItem(int blockID)
{
	for (int i = 0; i < MAX_ITEMSLOT; i++) {
		ItemStack& itemStack = itemStacks[i];
		if (itemStack.getBlockID() == blockID && itemStack.getNum() < MAX_ITEMSTACK) {
			itemStack.getNum()++;
			return true;
		}
	}
	for (int i = 0; i < MAX_ITEMSLOT; i++) {
		if (itemStacks[i].getBlockID() == 0) {
			itemStacks[i].getBlockID() = blockID;
			itemStacks[i].getNum() = 1;
			return true;
		}
	}
	return false;
}

void ItemSlot::draw(void) const
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslated(-0.15, -0.11, -0.2);
	glScaled(0.01, 0.01, 0.01);
	for (int i = 0; i < MAX_ITEMSLOT; i++) {
		glCallList((i != selected) ? displayListID1 : displayListID2);
		if (!itemStacks[i].getNum()) {
			glTranslated(2.5, 0, 0);
			continue;
		}
		glPushMatrix();
		const ItemStack& itemStack = itemStacks[i];
		const Block* block = BlockManager::getBlock(itemStack.getBlockID());
		glTranslated(0.8, 1.2, 0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glPushMatrix();
		glTranslated(0.8, -0.2, 0.01);
		glRotated(180, 0, 1, 0);
		block->draw();
		glPopMatrix();
		glDisable(GL_BLEND);
		glTranslated(0, -1, 1.1);
		StringManager::draw(0, 0, itemStack.getNum());
		glPopMatrix();
		glTranslated(2.5, 0, 0);
	}
	glTranslatef(10, 0, 0);
	BlockManager::getBlock(2)->draw();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//スロットを選択する
void ItemSlot::updateSelect(const Keyboard& keyboard)
{
	for (unsigned char key = '0'; key <= '9'; key++)
	if (keyboard.getKey(key))
		selected = (key - '0' + 9) % 10;
}
