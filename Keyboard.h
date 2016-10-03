#ifndef ___Class_Keyboard
#define ___Class_Keyboard

class Keyboard {
	bool key[256];
public:
	Keyboard(void) {
		for (int i = 0; i < sizeof(key) / sizeof(key[0]); i++)
			key[i] = false;
	}
	void down(unsigned char _key) { key[_key] = true; }
	void up(unsigned char _key) { key[_key] = false; }
	bool getKey(unsigned char _key) const { return key[_key]; }
};

#endif
