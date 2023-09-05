#ifndef HEADER_TED_TILEMAP
#define HEADER_TED_TILEMAP

#include "types.h"

#define TILEMAP_MAX_TILE_DIMS 1000 // Maximum x and y tilemap dimension per plane
#define TILEMAP_MAX_Z 2 // Maximum z-planes
#define TILE_DIMS_PX 40

// What percentage of the tilemap editor should be allocated to the tilemap itself
#define TILEMAP_SCREEN_PORTION 0.85

typedef struct TilemapData
{
    uint64 sizeInBytes;
    uint32 *tilemap;

    // Which area are we currently looking at?
    uint32 offsetX;
    uint32 offsetY;
    uint32 zIndex;

} TilemapData;

bool initTilemap(TilemapData *tilemapData);

#endif