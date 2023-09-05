#ifndef HEADER_TED_GO
#define HEADER_TED_GO

#include "types.h"
#include "graphics.h"

#define MAX_OBJECTS 10

typedef struct GameObject {
    bool32 set;
    const char *reference;
    uint32 offsetX;
    uint32 offsetY;
    uint32 width;
    uint32 height;
} GameObject;

void registerObject(GameObject *objects,
                    const char *unique_reference,
                    uint32 offsetX,
                    uint32 offsetY,
                    uint32 width,
                    uint32 height,
                    Win32FrameBuffer *frameBuffer,
                    bool32 drawRectangle,
                    uint32 hex,
                    Colour colour);

#endif