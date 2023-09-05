#include "macros.h"
#include "graphics.h"
#include "intrinsics.h"
#include "memory.h"

void win32InitFrameBuffer(Win32FrameBuffer *buffer,
                            uint32 width,
                            int32 height)
{
    // Does the bitmapMemory already exist from a previous WM_SIZE call?
    if(buffer->memory != NULL){

        // Yes, then free the memory allocated.
        // We do this because we have to redraw it as this method
        // (win32InitFrameBuffer) is called on a window resize.
        VirtualFree(buffer->memory, NULL, MEM_RELEASE);
    }

    buffer->bytesPerPixel = 4;
    buffer->width = width;
    buffer->height = height;

    buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
    buffer->info.bmiHeader.biWidth = width;
    buffer->info.bmiHeader.biHeight = -height; // If negative, it's drawn top down. If positive, it's drawn bottom up.
    buffer->info.bmiHeader.biPlanes = 1;
    buffer->info.bmiHeader.biBitCount = (buffer->bytesPerPixel * 8); // 32-bits per pixel
    buffer->info.bmiHeader.biCompression = BI_RGB;

    // How many bytes do we need for our bitmap?
    // viewport width * viewport height = viewport area
    // then viewport area * how many bytes we need per pixel.
    uint32 bitmapMemorySizeInBytes = ((buffer->width * buffer->height) * buffer->bytesPerPixel);

    // Now allocate the memory using VirtualAlloc to the size of the previously
    // calculated bitmapMemorySizeInBytes
    buffer->memory = allocateMemory(0, bitmapMemorySizeInBytes);

    // Calculate the width in bytes per row.
    buffer->byteWidthPerRow = (buffer->width * buffer->bytesPerPixel);
}

void win32DisplayFrameBuffer(HDC deviceHandleForWindow,
                                Win32FrameBuffer buffer,
                                uint32 clientWindowWidth,
                                uint32 clientWindowHeight)
{
    uint32 destinationWidth = buffer.width;
    uint32 destinationHeight = buffer.height;

    // Draw the frame with margin?
    int32 offsetX = 0;
    int32 offsetY = 0;

    // If the physical window is larger than our destination width/height, then
    // anything else in the window will be the colour set in PatBlt()
    StretchDIBits(deviceHandleForWindow,
        offsetX,
        offsetY,
        destinationWidth,
        destinationHeight,
        0,
        0,
        buffer.width,
        buffer.height,
        buffer.memory,
        &buffer.info,
        DIB_RGB_COLORS,
        SRCCOPY);
}

void writeRectangle(Win32FrameBuffer *buffer,
                    float32 xOffsetf,
                    float32 yOffsetf,
                    uint32 width,
                    uint32 height,
                    uint32 hex,
                    Colour colour)
{
    assert(width >= 0.0f);
    assert(height >= 0.0f);

    int32 xOffset = intrin_roundF32ToI32(xOffsetf);
    int32 yOffset = intrin_roundF32ToI32(yOffsetf);

    // Bounds checking
    if(xOffset >= (int32)buffer->width){
        return;
    }

    if(yOffset >= (int32)buffer->height){
        return;
    }

    // Min x
    if(xOffset < 0){
        width = (width - (xOffset * -1));
        if(width <= 0){
            return;
        }
        xOffset = 0;
    }

    // Min y
    if(yOffset < 0){
        height = (height - (yOffset * -1));
        if(height <= 0){
            return;
        }
        yOffset = 0;
    }

    // Max x
    uint32 maxX = (xOffset + width);

    if(maxX > buffer->width){
        maxX = (buffer->width - xOffset);
        if(width > maxX){
            width = maxX;
        }
    }

    // Max y
    uint32 maxY = (yOffset + height);

    if(maxY > buffer->height){
        maxY = (buffer->height - yOffset);
        if(height > maxY){
            height = maxY;
        }
    }

    uint32 alpha, red, green, blue = 0;

    // Set the colour
    if(hex){

        blue    = hex & 0xFF;
        green   = (hex >> 8) & 0xFF;
        red     = (hex >> 16) & 0xFF;
        alpha   = (hex >> 24) & 0xFF;

    } else{
        alpha   = ((uint32)(255.0f * colour.a) << 24);
        red     = ((uint32)(255.0f * colour.r) << 16);
        green   = ((uint32)(255.0f * colour.g) << 8);
        blue    = ((uint32)(255.0f * colour.b) << 0);
    }

    // Bug! Should be shift, not OR
    uint32 hexColour = ( (alpha << 24) | (red << 16) | (green << 8) | blue);

    // Write the memory
    uint32 *row = (uint32 *)buffer->memory;

    // Move to last row as starting position (bottom left of axis)
    row = (row + ((buffer->width * buffer->height) - buffer->width));

    // Move up to starting row
    row = (row - (buffer->width * yOffset));

    // Move in from left to starting absolutePosition
    row = (row + xOffset);

    // Up (rows) y
    for(uint32 i = 0; i < height; i++){

        // Accross (columns) x
        uint32 *pixel = (uint32 *)row;
        for(uint32 x = 0; x < width; x++){

            *pixel = hexColour;
            pixel = (pixel + 1);
        }

        // Move up one entire row
        row = (row - buffer->width);
    }
}