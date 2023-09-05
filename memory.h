#ifndef HEADER_TED_MEMORY
#define HEADER_TED_MEMORY

#include <windows.h>
#include "types.h"
#include "macros.h"
#include "tilemap.h"

typedef struct EditorMemory {
    bool32 initilised;
    TilemapData *tilemapData;
} EditorMemory;

void *allocateMemory(LPVOID memoryStartAddress, uint64 memorySizeInBytes);

void freeMemory(void *address);

#endif