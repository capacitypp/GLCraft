#include "DisplayListManager.h"

int DisplayListManager::cnt = 0;

int DisplayListManager::getNewID(void)
{
	return ++cnt;
}
