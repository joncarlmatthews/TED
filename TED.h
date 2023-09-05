#pragma once

#include "resource.h"
#include "types.h"
#include "macros.h"
#include "intrinsics.h"
#include "graphics.h"
#include "memory.h"
#include "tilemap.h"
#include "game_objects.h"
#include "editor.h"

internal_func
Win32ClientDimensions win32GetClientDimensions(HWND window);

internal_func
void win32ProcessMessages(HWND window, EditorMemory *editorMemory, GameObject objects[]);

internal_func
void clickHandler(EditorMemory *editorMemory,
                    uint16 mouseX,
                    uint16 mouseY,
                    uint16 scale,
                    GameObject objects[],
                    BOOL hover);

internal_func
const char * clickedObject(uint16 mouseX,
                            uint16 mouseY,
                            uint16 scale,
                            GameObject objects[]);

internal_func
BOOL objectClicked(uint16 mouseX,
                    uint16 mouseY,
                    uint16 scale,
                    GameObject objects[],
                    const char *reference);