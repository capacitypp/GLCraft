#include "BlockAir.h"
#include "DisplayListManager.h"

#include <gl\glut.h>

using namespace std;

void BlockAir::registerDisplayList(void)
{
	//ディスプレイリストは登録しない
	displayListID = -1;
}
