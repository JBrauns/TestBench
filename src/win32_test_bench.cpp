#define UNICODE

#include <Windows.h>
#include <stdint.h>
#include <mmsystem.h>
#include <intrin.h>

#include "jb_platform.h"

#include "imgui.h"
#include "imgui_draw.cpp"
#include "imgui.cpp"

#include "jb_math.h"

struct Win32OffscreenBuffer
{
    BITMAPINFO Info;
    void *Memory;
    int32 Width;
    int32 Height;
    int32 Pitch;
    int32 BytesPerPixel;
};

struct Win32WindowDimensions
{
    int32 Width;
    int32 Height;
};

struct MemoryContainer
{
    bool32 IsInitialized;
    
    uint64 PermanenStorageSize;
    void *PermanentStorage;
    
    uint64 TransientStorageSize;
    void *TransientStorage;
};

global_variable bool32 g_Running;
global_variable int64 g_PerfCountFrequency;
global_variable Win32OffscreenBuffer g_BackBuffer;

internal void Win32DisplayBufferInWindow(Win32OffscreenBuffer * buffer,
                                         HDC deviceContext,
                                         int32 windowWidth,
                                         int32 windowHeight)
{
    StretchDIBits(deviceContext,
                  0, 0, buffer->Width, buffer->Height,
                  0, 0, windowWidth, windowHeight,
                  buffer->Memory,
                  &buffer->Info,
                  DIB_RGB_COLORS, SRCCOPY);
}

internal Win32WindowDimensions Win32GetWindowDimension(HWND window)
{
    Win32WindowDimensions result;
    RECT clientRect;
    
    GetClientRect(window, &clientRect);
    result.Width = clientRect.right - clientRect.left;
    result.Height = clientRect.bottom - clientRect.top;
    
    return(result);
}

internal void Win32ResizeDIBSection(Win32OffscreenBuffer *buffer, int32 width, int32 height)
{
    if(buffer->Memory)
    {
        VirtualFree(buffer->Memory, 0, MEM_RELEASE);
    }
    
    buffer->Width = width;
    buffer->Height = height;
    
    buffer->BytesPerPixel = 4;
    buffer->Info.bmiHeader.biSize = sizeof(buffer->Info.bmiHeader);
    buffer->Info.bmiHeader.biWidth = buffer->Width;
    buffer->Info.bmiHeader.biHeight = -buffer->Height;
    buffer->Info.bmiHeader.biPlanes = 1;
    buffer->Info.bmiHeader.biBitCount = 32;
    buffer->Info.bmiHeader.biCompression = BI_RGB;
    
    int32 bitmapMemorySize = (buffer->Width*buffer->Height)*buffer->BytesPerPixel;
    buffer->Memory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
    
    buffer->Pitch = width*buffer->BytesPerPixel;    
}

internal LRESULT CALLBACK Win32MainWindowCallback(HWND windowHandle,
                                                  UINT message,
                                                  WPARAM wParam,
                                                  LPARAM lParam)
{
    LRESULT result = 0;
    
    switch(message)
    {
        case WM_SIZE:
        {
            // TODO (joshua): Resize backbuffer
            int32 width = (int32)LOWORD(lParam);
            int32 height = (int32)HIWORD(lParam);
            Win32ResizeDIBSection(&g_BackBuffer, width, height);
        } break;
        
        case WM_SETCURSOR:
        {
            result = DefWindowProc(windowHandle, message, wParam, lParam);
        } break;
        
        case WM_DESTROY:
        {
            g_Running = false;
        } break;
        
        case WM_CLOSE:
        {
            g_Running = false;
        } break;
        
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            Assert("Keyboard input came in through a non-dispatch message!");
        } break;
        
        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC deviceContext;
            deviceContext = BeginPaint(windowHandle, &paint);
            
            Win32WindowDimensions dimension = Win32GetWindowDimension(windowHandle);
            Win32DisplayBufferInWindow(&g_BackBuffer, deviceContext,
                                       dimension.Width, dimension.Height);
            
            EndPaint(windowHandle, &paint);
        } break;
        
        default:
        {
            result = DefWindowProc(windowHandle, message, wParam, lParam);
        }
    }
    
    return(result);
}

enum KeyboardSpecialKey
{
    KeyboardSpecialKey_Alt = (1 << 0),
    KeyboardSpecialKey_Strg = (1 << 1),
    KeyboardSpecialKey_Shift = (1 << 2),
};

enum MouseButton
{
    MouseButton_Left = (1 << 0),
    MouseButton_Middle = (1 << 1),
    MouseButton_Right = (1 << 2),
};

struct Input
{
    bool32 IsKeyPressed[256];
    int32 SpecialKeys;
    
    int32 MouseButtons;
    v2 MousePosition;
    
    real32 MouseWheel;
};

inline void SetMouseButtonFlag(Input *input, int32 flags)
{
    input->MouseButtons |= flags;
}

inline void ClearMouseButtonFlag(Input *input, int32 flags)
{
    input->MouseButtons &= ~flags;
}

internal void Win32ProcessPendingMessages(Input *input)
{
    MSG message;
    while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {
        switch(message.message)
        {
            case WM_QUIT:
            {
                g_Running = false;
            } break;
            
            case WM_CLOSE:
            {
                PostQuitMessage(0);
                g_Running = false;
            } break;
            
            case WM_KEYDOWN:
            {
                input->IsKeyPressed[message.wParam] = false;
            } break;
            
            case WM_KEYUP:
            {
                input->IsKeyPressed[message.wParam] = true;
            } break;
            
            case WM_LBUTTONDOWN:
            {
                SetMouseButtonFlag(input, MouseButton_Left);
            } break;
            
            case WM_LBUTTONUP:
            {
                ClearMouseButtonFlag(input, MouseButton_Left);
            } break;
            
            case WM_MBUTTONDOWN:
            {
                SetMouseButtonFlag(input, MouseButton_Middle);
            } break;
            
            case WM_MBUTTONUP:
            {
                ClearMouseButtonFlag(input, MouseButton_Middle);
            } break;
            
            case WM_RBUTTONDOWN:
            {
                SetMouseButtonFlag(input, MouseButton_Right);
            } break;
            
            case WM_RBUTTONUP:
            {
                ClearMouseButtonFlag(input, MouseButton_Right);
            } break;
            
            case WM_MOUSEMOVE:
            {
                input->MousePosition.X = (int16)(message.lParam);
                input->MousePosition.Y = (int16)(message.lParam >> 16);
            } break;
            
            case WM_CHAR:
            {
                // TODO (joshua): Capture keys that get into here 
            } break;
            
            default:
            {
                
            } break;
        }
        
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

inline LARGE_INTEGER Win32GetWallClock(void)
{
    LARGE_INTEGER result;
    QueryPerformanceCounter(&result);
    return(result);
}

inline real32 Win32GetSecondsElapsed(LARGE_INTEGER start, LARGE_INTEGER end)
{
    real32 result = ((real32)(end.QuadPart - start.QuadPart) /
        (real32)(g_PerfCountFrequency));
    return(result);
}

int CALLBACK WinMain(HINSTANCE instance,
                     HINSTANCE prevInstance,
                     LPSTR commandLine,
                     int showCode)
{
    LARGE_INTEGER perfCountFrequencyResult;
    QueryPerformanceFrequency(&perfCountFrequencyResult);
    g_PerfCountFrequency = perfCountFrequencyResult.QuadPart;
    
    UINT desiredSchedulerMS = 1;
    bool32 sleepIsGranular = (timeBeginPeriod(desiredSchedulerMS) == TIMERR_NOERROR);
    
    WNDCLASS windowClass = {};
    
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = Win32MainWindowCallback;
    windowClass.hInstance = instance;
    windowClass.hCursor = LoadCursor(0, IDC_ARROW);
    windowClass.lpszClassName = L"DataSnifferClass";
    
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = 960.0f;
    io.DisplaySize.y = 540.0f;
    
    unsigned char *pixels;
    int texWidtht = 960;
    int texHeight = 540;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &texWidtht, &texHeight);
    
    if(RegisterClass(&windowClass))
    {
        HWND windowHandle = CreateWindowEx(0,
                                           windowClass.lpszClassName,
                                           L"Data Sniffer",
                                           WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                           CW_USEDEFAULT,
                                           CW_USEDEFAULT,
                                           CW_USEDEFAULT,
                                           CW_USEDEFAULT,
                                           0, 0, instance, 0);
        DWORD lastError = GetLastError();
        
        if(windowHandle)
        {
            g_Running = true;
            
            int32 monitorRefreshHz = 60;
            HDC refreshDC = GetDC(windowHandle);
            int32 win32RefreshRate = GetDeviceCaps(refreshDC, VREFRESH);
            ReleaseDC(windowHandle, refreshDC);
            if(win32RefreshRate > 1)
            {
                monitorRefreshHz = win32RefreshRate;
            }
            real32 updateHz = (monitorRefreshHz / 2.0f);
            real32 targetSecondsPerFrame = 1.0f / (real32)updateHz;
            
            MemoryContainer memoryContainer = {};
            memoryContainer.PermanenStorageSize = Megabytes(32);
            memoryContainer.TransientStorageSize = Megabytes(128);
            
            int64 totalSize = memoryContainer.PermanenStorageSize + memoryContainer.TransientStorageSize;
            memoryContainer.PermanentStorage = VirtualAlloc(0, (size_t)totalSize,
                                                            MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            memoryContainer.TransientStorage = ((uint8 *)memoryContainer.PermanentStorage + 
                memoryContainer.PermanenStorageSize);
            
            Input input[2] = {};
            Input *newInput = &input[0];
            Input *oldInput = &input[1];
            
            LARGE_INTEGER lastCounter = Win32GetWallClock();
            LARGE_INTEGER flipWallClock = Win32GetWallClock();
                
            while(g_Running)
            {
                Win32ProcessPendingMessages(newInput);
                                        
                io.MousePos.x = newInput->MousePosition.X;
                io.MousePos.y = newInput->MousePosition.Y;
                
                ImGui::NewFrame();
                
                uint8 *row = (uint8 *)g_BackBuffer.Memory;
                for(int32 y = 0;
                    y < g_BackBuffer.Height;
                    ++y)
                {
                    uint32 *pixel = (uint32 *)row;
                    for(int32 x = 0;
                        x < g_BackBuffer.Width;
                        ++x)
                    {
                        real32 r = (Sin((real32)x*0.05f) + 1.0f)*0.5f;
                        real32 g = 0.0f;
                        real32 b = 0.0f;
                        
                        uint32 color = (uint32)((RoundReal32ToUInt32(r * 255.0f) << 16) |
                                                (RoundReal32ToUInt32(g * 255.0f) << 8) |
                                                (RoundReal32ToUInt32(b * 255.0f) << 0));
                                        
                        *pixel++ = color;
                    }
                    
                    row += g_BackBuffer.Pitch;
                }
                
                Win32WindowDimensions dimension = Win32GetWindowDimension(windowHandle);
                HDC deviceContext = GetDC(windowHandle);
                Win32DisplayBufferInWindow(&g_BackBuffer, deviceContext,
                                           dimension.Width, dimension.Height);
                ReleaseDC(windowHandle, deviceContext);
                
                Input *temp = newInput;
                newInput = oldInput;
                oldInput = temp;
                
                ImGui::Begin("My window");
                ImGui::Text("Hello World!");
                ImGui::End();
                
                ImGui::Render();
            }
        }
    }
}