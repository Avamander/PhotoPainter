#pragma once

#include "../Config/DEV_Config.h"

#include "fatfs-spi.h"

#define fileNumber 100
#define fileLen 100

char sdTest();
void sdInitTest();

void run_mount();
void run_unmount();

void file_cat();

void sdScanDir();

char isFileExist(const char *path);
void setFilePath();

void updatePathIndex();
void file_sort();
