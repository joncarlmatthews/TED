#ifndef HEADER_TED_GRAPHICS
#define HEADER_TED_GRAPHICS

#include <windows.h>
#include "types.h"
#include "macros.h"

typedef struct Colour {
    float32 r; // Between 0.0f and 1.0f
    float32 g; // Between 0.0f and 1.0f
    float32 b; // Between 0.0f and 1.0f
    float32 a; // Between 0.0f and 1.0f
} Colour;

/**
 * Struct for the Win32 screen buffer
 */
typedef struct Win32FrameBuffer
{
    // The width in pixels of the buffer.
    uint32 width;

    // The height in pixels of the buffer.
    uint32 height;

    // 1 byte each for R, G & B and 1 byte for padding to match byte boundries (4)
    // Therefore our pixels are always 32-bits wide and are in Little Endian 
    // memory order 0xPPRRGGBB
    uint16 bytesPerPixel;

    // The number of bytes per row. (width * bytesPerPixel)
    uint32 byteWidthPerRow;

    // Bitmap parameters
    BITMAPINFO info;

    // Pointer to an allocated block of heap memory to hold the data of the buffer.
    void *memory;
} Win32FrameBuffer;

typedef struct Win32ClientDimensions
{
    uint32 width;
    uint32 height;
} Win32ClientDimensions;

void win32InitFrameBuffer(Win32FrameBuffer *buffer, uint32 width, int32 height);

void win32DisplayFrameBuffer(HDC deviceHandleForWindow,
                                Win32FrameBuffer buffer,
                                uint32 clientWindowWidth,
                                uint32 clientWindowHeight);

void writeRectangle(Win32FrameBuffer *buffer,
                    float32 xOffsetf,
                    float32 yOffsetf,
                    uint32 width,
                    uint32 height,
                    uint32 hex,
                    Colour colour);

#endif