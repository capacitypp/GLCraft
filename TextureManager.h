#ifndef ___Class_TextureManager
#define ___Class_TextureManager

#define MAX_TEXTURE	100

#include <gl\glut.h>

class TextureManager {
	static int cnt;
	static GLuint IDs[MAX_TEXTURE];
public:
	static void init(void);
	static GLuint getNewID(void);
};

#endif
