#include "game_objects.h"

void registerObject(GameObject *objects,
                    const char *unique_reference,
                    uint32 offsetX,
                    uint32 offsetY,
                    uint32 width,
                    uint32 height,
                    Win32FrameBuffer *frameBuffer,
                    bool32 drawRectangle,
                    uint32 hex,
                    Colour colour)
{
    for(size_t i = 0; i < MAX_OBJECTS; i++){
        if(objects[i].set != 0){
            continue;
        }
        GameObject to = { 0 };
        to.set = 1;
        to.reference = unique_reference;
        to.offsetX = offsetX;
        to.offsetY = offsetY;
        to.width = width;
        to.height = height;
        objects[i] = to;

        if(drawRectangle){
            writeRectangle(frameBuffer,
                            (float32)offsetX,
                            (float32)offsetY,
                            width, height,
                            hex,
                            colour);
        }

        
        break;
    }
}