#ifndef ___Class_Mouse
#define ___Class_Mouse

#include "global.h"
#include "Player.h"

class Mouse {
	int bx, by;
	int x, y;
	bool left, right;
	int leftCnt;
public:
	Mouse(void) : bx(WINDOW_WIDTH / 2), by(WINDOW_HEIGHT / 2), left(false), right(false), leftCnt(0) {}
	inline void setPosition(int x, int y) { this->x = x; this->y = y; }
	inline bool getLeft(void) const { return left; }
	inline bool getRight(void) const { return right; }
	inline int getLeftCount(void) const { return leftCnt; }
	inline bool& getLeft(void) { return left; }
	inline bool& getRight(void) { return right; }
	inline int& getLeftCount(void) { return leftCnt; }
	void update(void);
	void updatePlayerDirection(Player& player);
};

#endif
