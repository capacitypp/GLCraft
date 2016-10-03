#ifndef ___Class_StringManager
#define ___Class_StringManager

#include <string>

#include <gl\glut.h>

class StringManager {
public:
	static void draw(double x, double y, const std::string& str) {
		glRasterPos3d(x, y, -1);
		for (unsigned i = 0; i < str.size(); i++)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
	static void draw(double x, double y, double value) {
		draw(x, y, std::to_string(value));
	}
	static void draw(double x, double y, int value) {
		draw(x, y, std::to_string(value));
	}
};

#endif
