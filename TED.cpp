// TED.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "TED.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HWND *hWnd, HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

global_var BOOL running = TRUE;
global_var BOOL mouseLeftButtonIsDown = FALSE;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TED, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    HWND hWnd;
    if (!InitInstance (&hWnd, hInstance, nCmdShow)){
        return 1;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TED));

    HDC deviceHandleForWindow = GetDC(hWnd);

    // Init frame buffer
    Win32FrameBuffer frameBuffer;

    // @TODO(JM) replace SCREEN_SIZE_X and SCREEN_SIZE_Y with device caps
    win32InitFrameBuffer(&frameBuffer, SCREEN_SIZE_X, SCREEN_SIZE_Y);

    // Init tilemap
    TilemapData tilemapData = { 0 };
    initTilemap(&tilemapData);

    EditorMemory *editorMemory = (EditorMemory *)allocateMemory(0, sizeof(EditorMemory));
    editorMemory->tilemapData = &tilemapData;

    // Init game objects
    GameObject gameObjects[MAX_OBJECTS] = { 0 };
    
    // Main loop.
    do{

        Win32ClientDimensions clientDimensions = win32GetClientDimensions(hWnd);

        updateAndRender(clientDimensions, &frameBuffer, editorMemory, gameObjects);
    
        win32ProcessMessages(hWnd, editorMemory, gameObjects);
        win32DisplayFrameBuffer(deviceHandleForWindow, 
                                frameBuffer, 
                                clientDimensions.width,
                                clientDimensions.height);
    } while(running);

    // Exit app.
    return 0;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TED));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TED);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HWND *hWnd, HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   *hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!*hWnd)
   {
      return FALSE;
   }

   ShowWindow(*hWnd, nCmdShow);
   UpdateWindow(*hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
            break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // Get the window's height and width
            Win32ClientDimensions clientDimensions = win32GetClientDimensions(hWnd);

            // Paint the whole screen black (stops the artifacts around the frame buffer)
            PatBlt(hdc,
                0, 0,
                clientDimensions.width,
                clientDimensions.height,
                BLACKNESS);

            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

/**
 * Gets the height and width of the actual window. This changes if the window is
 * resized, maximised etc
 */
internal_func
Win32ClientDimensions win32GetClientDimensions(HWND window)
{
    RECT clientRect;
    GetClientRect(window, &clientRect);

    Win32ClientDimensions dim = { 0 };

    dim.width = clientRect.right;
    dim.height = clientRect.bottom;

    return dim;
}

internal_func
void win32ProcessMessages(HWND window, EditorMemory *editorMemory, GameObject objects[])
{
    MSG message = { 0 };

    // Win32 Message loop. Retrieves all messages (from the calling thread's message queue)
    // that are sent to the window. E.g. clicks and key inputs.
    while(PeekMessage(&message, window, 0, 0, PM_REMOVE)){

        switch(message.message){

            // If the message received was a quit message, then toggle our
            // running flag to false to break out of this loop on the next
            // iteration.
            case WM_QUIT:
            {
                running = false;
            } break;

            // Mouse left click down
            case WM_LBUTTONDOWN:
            {
                mouseLeftButtonIsDown = TRUE;
                SetCapture(window);
            } break;

            // Mouse left click up
            case WM_LBUTTONUP:
            {
                
                // X and Y coordinates of the mouse click
                uint16 mouseX = (LOWORD(message.lParam) % SCREEN_SIZE_X);
                uint16 mouseY = ((SCREEN_SIZE_Y - HIWORD(message.lParam)) % SCREEN_SIZE_Y);

                uint16 scale = 1;

                if(mouseLeftButtonIsDown){
                    mouseLeftButtonIsDown = FALSE;
                    clickHandler(editorMemory, mouseX, mouseY, scale, objects, TRUE);
                } else{
                    clickHandler(editorMemory, mouseX, mouseY, scale, objects, FALSE);
                }

                ReleaseCapture();
            
            } break;

            case WM_MOUSEMOVE:
            {
                if(mouseLeftButtonIsDown){
                    // X and Y coordinates of the mouse click
                    uint16 mouseX = (LOWORD(message.lParam) % SCREEN_SIZE_X);
                    uint16 mouseY = ((SCREEN_SIZE_Y - HIWORD(message.lParam)) % SCREEN_SIZE_Y);

                    uint16 scale = 1;

                    clickHandler(editorMemory, mouseX, mouseY, scale, objects, TRUE);
                }
            }break;

            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            {
            } break;

            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
            {
            } break;

            case WM_KEYUP:
            case WM_SYSKEYUP:
            {
                // Which key was pressed?
                WPARAM vkCode = message.wParam;

                switch(vkCode){
                    
                    case VK_ESCAPE:
                    {
                        running = FALSE;
                    } break;
                   
                }
            }

            // The standard request from GetMessage().
            default:
            {

                // Dispatch the message to the application's window procedure, WndProc
                TranslateMessage(&message); // Get the message ready for despatch.
                DispatchMessage(&message); // Actually do the despatch

            } break;

        } // message switch

    } // PeekMessage loop
}

internal_func
void clickHandler(EditorMemory *editorMemory,
                    uint16 mouseX,
                    uint16 mouseY,
                    uint16 scale,
                    GameObject objects[],
                    BOOL hover)
{
    uint32 tileValue = 1;

    const char *clickedObjectRef = clickedObject(mouseX, mouseY, scale, objects);

    OutputDebugString(L"Clicked ");
    OutputDebugStringA(clickedObjectRef);
    OutputDebugString(L"\n");


    if("tilemap_area" == clickedObjectRef){

        uint32 tileX = (mouseX / TILE_DIMS_PX);
        uint32 tileY = (mouseY / TILE_DIMS_PX);

        uint32 *tile = editorMemory->tilemapData->tilemap + (editorMemory->tilemapData->zIndex * (TILEMAP_MAX_TILE_DIMS * TILEMAP_MAX_TILE_DIMS)) + (tileY * TILEMAP_MAX_TILE_DIMS) + tileX;

        if(hover){
            *tile = tileValue;
        } else{
            if(*tile > 0){
                *tile = 0;
            } else{
                *tile = tileValue;
            }
        }
    } else if("increment_z_plane" == clickedObjectRef){
        if(editorMemory->tilemapData->zIndex != (TILEMAP_MAX_Z-1)){
            editorMemory->tilemapData->zIndex += 1;
        }
    } else if("decrement_z_plane" == clickedObjectRef){
        if(editorMemory->tilemapData->zIndex >= 1){
            editorMemory->tilemapData->zIndex -= 1;
        }
    }

}

internal_func
const char *clickedObject(uint16 mouseX,
                            uint16 mouseY,
                            uint16 scale,
                            GameObject objects[])
{
    uint16 bufferX = (mouseX / scale);
    uint16 bufferY = (mouseY / scale);

    for(size_t i = 0; i < MAX_OBJECTS; i++){
        GameObject object = objects[i];
        if(1 != object.set){
            continue;
        }
        if(bufferX >= object.offsetX && bufferX < object.offsetX + object.width &&
            bufferY >= object.offsetY && bufferY < object.offsetY + object.height){
            return object.reference;
        }
    }
    return "";
}

BOOL objectClicked(uint16 mouseX, uint16 mouseY, uint16 scale, GameObject objects[], const char *reference)
{
    uint16 bufferX = (mouseX / scale);
    uint16 bufferY = (mouseY / scale);

    for(size_t i = 0; i < MAX_OBJECTS; i++){
        GameObject object = objects[i];
        if(1 != object.set){
            continue;
        }
        if(object.reference == reference){
            if(bufferX >= object.offsetX && bufferX < object.offsetX + object.width &&
                bufferY >= object.offsetY && bufferY < object.offsetY + object.height){
                return TRUE;
            }
        }
    }
    return FALSE;
}