#ifndef ___Class_ItemSlot
#define ___Class_ItemSlot

#include "global.h"
#include "ItemStack.h"
#include "Image.h"
#include "Keyboard.h"

#define GLUT_BUILDING_LIB
#include <gl\glut.h>

class ItemSlot {
	ItemStack itemStacks[MAX_ITEMSLOT];
	int selected;
	Image texture1, texture2;
	GLuint textureID1, textureID2;
	GLuint displayListID1, displayListID2;
public:
	//土ブロックを5個持たせる
	ItemSlot(void) : selected(0) { texture1 = Image("texture\\itemSlot\\itemSlot1.ppm"); texture2 = Image("texture\\itemSlot\\itemSlot2.ppm"); itemStacks[0] = ItemStack(2, 5); }
	void registerTexture(void);
	ItemStack getCurrentItemStack(void) const { return itemStacks[selected]; }
	ItemStack getItemStack(int idx) const { return itemStacks[idx]; }
	ItemStack& getCurrentItemStack(void) { return itemStacks[selected]; }
	ItemStack& getItemStack(int idx) { return itemStacks[idx]; }
	void useItem(void);
	bool addItem(int blockID);
	void draw(void) const;
	void updateSelect(const Keyboard& keyboard);
};

#endif
