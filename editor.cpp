#include "editor.h"
#include "tilemap.h"
#include "game_objects.h"

void updateAndRender(Win32ClientDimensions clientDimensions,
                        Win32FrameBuffer *frameBuffer,
                        EditorMemory *editorMemory,
                        GameObject gameObjects[])
{
    // Initialisation...
 
    // @TODO(JM) dont need to do this each loop, set initialisation memory

    // Grey bg for entire app
    writeRectangle(frameBuffer,
                    0.0f, 0.0f,
                    SCREEN_SIZE_X, SCREEN_SIZE_Y,
                    0xDADADA, {});

    // Down z plane
    registerObject(gameObjects,
                    "decrement_z_plane",
                    SCREEN_SIZE_X-200.0f, 0.0f,
                    50, 50,
                    frameBuffer,
                    false, 0, {});

    writeRectangle(frameBuffer,
                    SCREEN_SIZE_X - 200.0f, 0.0f,
                    50, 50,
                    0x00ff00, {});

    // Up z plane
    registerObject(gameObjects,
                    "increment_z_plane",
                    SCREEN_SIZE_X - 100.0f, 0.0f,
                    50, 50,
                    frameBuffer,
                    false, 0, {});

    writeRectangle(frameBuffer,
                    SCREEN_SIZE_X - 100.0f, 0.0f,
                    50, 50,
                    0x0000ff, {});

    // End of initialisation

    /*
     * Tilemap
     */

    // How many x tiles can we fit on the screen?
    uint32 maxTilesX = (uint32)((SCREEN_SIZE_X * TILEMAP_SCREEN_PORTION) / TILE_DIMS_PX);
    uint32 maxTilesY = (uint32)((SCREEN_SIZE_Y * TILEMAP_SCREEN_PORTION) / TILE_DIMS_PX);

    registerObject(gameObjects,
                    "tilemap_area",
                    0, 0,
                    (maxTilesX * TILE_DIMS_PX),
                    (maxTilesY * TILE_DIMS_PX),
                    frameBuffer,
                    false, 0, {});

    // Draw tilemap
    for(size_t y = 0; y < maxTilesY; y++){
        for(size_t x = 0; x < maxTilesX; x++){

            uint32 *tile = editorMemory->tilemapData->tilemap + (editorMemory->tilemapData->zIndex * (TILEMAP_MAX_TILE_DIMS * TILEMAP_MAX_TILE_DIMS)) + (y * TILEMAP_MAX_TILE_DIMS) + x;

            if(!tile){
                continue;
            }

            uint32 tileColour = 0xffffff;

            switch(*tile){
                default:
                    tileColour = 0xffffff;
                    break;
                case 1:
                    tileColour = 0xff0000;
                    break;
            }

            float32 offsetX = (float32)(x * TILE_DIMS_PX);
            float32 offsetY = (float32)(y * TILE_DIMS_PX);

            writeRectangle(frameBuffer,
                offsetX, offsetY,
                TILE_DIMS_PX, TILE_DIMS_PX,
                tileColour, {});
        }
    }
}