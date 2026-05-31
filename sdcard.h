#ifndef SDCARD_H
#define SDCARD_H

#include "init.h"

// 注意：不直接包含 menu.h，避免循环依赖
// 但在 .cpp 中会包含 menu.h

// 函数声明
bool initSDCard();
void reinitEPDSPI();
void listDirectory(String path, FileItem* items, int* count);
void browseDirectory(String path);

#endif