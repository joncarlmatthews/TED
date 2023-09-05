#ifndef HEADER_TED_EDITOR
#define HEADER_TED_EDITOR

#include "graphics.h"
#include "game_objects.h"
#include "memory.h"

void updateAndRender(Win32ClientDimensions clientDimensions,
                        Win32FrameBuffer *frameBuffer,
                        EditorMemory *editorMemory,
                        GameObject *gameObjects);

#endif