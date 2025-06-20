#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "menu.h"

RECORD_LIST fmReadAll(const char* filename);
int fmCreate(const char* filename, const PLAYER_RECORD* rec);
int fmUpdate(const char* filename, const PLAYER_RECORD* rec);
int fmDelete(const char* filename, const char* name);
void fmFreeList(RECORD_LIST* list);

#endif // FILE_MANAGER_H
