//MIT License

//Copyright (c) 2026 Z-Multiplier
#include "EntryPoint.hpp"
#include <windows.h>
HINSTANCE mainHInstance;

BOOL SetHighDPIAwareness() {
    HMODULE hUser32=LoadLibraryA("user32.dll");
    if (hUser32){
        typedef BOOL (WINAPI *SetProcessDpiAwarenessContextProc)(HANDLE);
        SetProcessDpiAwarenessContextProc SetProcessDpiAwarenessContext=(SetProcessDpiAwarenessContextProc)GetProcAddress(hUser32,"SetProcessDpiAwarenessContext");
        if(SetProcessDpiAwarenessContext){
            return SetProcessDpiAwarenessContext((HANDLE)DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        }
        FreeLibrary(hUser32);
    }
    return FALSE;
}

extern int main();

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR lpCmdLine,int nCmdShow)
{
    SetHighDPIAwareness();
    mainHInstance=hInstance;
    return main();
}