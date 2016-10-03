#ifndef ___Class_ItemStack
#define ___Class_ItemStack

class ItemStack {
	int blockID;
	int num;
public:
	ItemStack(void) : blockID(0), num(0) {}
	ItemStack(int _blockID) : blockID(_blockID), num(1) {}
	ItemStack(int _blockID, int _num) : blockID(_blockID), num(_num) {}
	inline int getBlockID(void) const { return blockID; }
	inline int getNum(void) const { return num; }
	inline int& getBlockID(void) { return blockID; }
	inline int& getNum(void) { return num; }
	void draw(void) const;
};

#endif
