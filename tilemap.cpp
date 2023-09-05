#include "tilemap.h"
#include "memory.h"

bool initTilemap(TilemapData *tilemapData)
{
    tilemapData->sizeInBytes = ( ((TILEMAP_MAX_TILE_DIMS * TILEMAP_MAX_TILE_DIMS) * TILEMAP_MAX_Z) * sizeof(uint32));
    tilemapData->tilemap = (uint32 *)allocateMemory(0, tilemapData->sizeInBytes);
    tilemapData->zIndex = 0;
    return true;
}