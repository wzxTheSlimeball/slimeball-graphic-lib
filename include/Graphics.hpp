// The final file.
// ATK 99  DEF 99 (?)
// It includes all necessary graphics-related headers for easy access.

//MIT License

//Copyright (c) 2026 Z-Multiplier

#pragma once
#include "Canvas.hpp"
#include "Color.hpp"
#include "Logger.hpp"
#include "Window.hpp"
#include "EntryPoint.hpp"
#include "Painter.hpp"

#define SCREEN_MAXX GetSystemMetrics(SM_CXSCREEN)
#define SCREEN_MAXY GetSystemMetrics(SM_CYSCREEN)

extern HINSTANCE mainHInstance;
Core::logger globalLogger;

inline std::pair<std::shared_ptr<Window::Handle>, HWND> 
createInitWindow(int x,int y,int width,int height,std::wstring title,HINSTANCE instance=mainHInstance) 
{
    auto window=std::make_shared<Window::Handle>(x,y,width,height,WINDOWSTYLE_DEFAULT,0,title);
    window->registerToManager();
    HWND hWnd=window->initWindow(title.c_str(),instance);
    SetPropW(hWnd,L"isTransparent",(HANDLE)FALSE);
    bool result=window->initBuffer();
    if(!result){
        globalLogger.traceLog(Core::logger::LOG_WARNING,"Failed to init buffer, maybe will lead to UB");
    }
    return {window, hWnd};
}

inline std::pair<std::shared_ptr<Window::Handle>, HWND> 
createInitTransparentWindow(int x,int y,int width,int height,std::wstring title,Core::Color targetTransparentPixel,HINSTANCE instance=mainHInstance) 
{
    auto window=std::make_shared<Window::Handle>(x,y,width,height,WS_POPUP,WINDOWSTYLE_FULLTRANSPARENT|WS_EX_APPWINDOW,title);
    window->registerToManager();
    HWND hWnd=window->initWindow(title.c_str(),instance);
    SetLayeredWindowAttributes(hWnd,targetTransparentPixel.toCOLORREF(),0,LWA_COLORKEY);
    SetPropW(hWnd,L"isTransparent",(HANDLE)TRUE);
    bool result=window->initBuffer();
    if(!result){
        globalLogger.traceLog(Core::logger::LOG_WARNING,"Failed to init buffer, maybe will lead to UB");
    }
    return {window, hWnd};
}

inline std::pair<std::shared_ptr<Window::Handle>, HWND> 
createInitTransparentTopWindow(int x,int y,int width,int height,std::wstring title,Core::Color targetTransparentPixel,HINSTANCE instance=mainHInstance) 
{
    auto window=std::make_shared<Window::Handle>(x,y,width,height,WS_OVERLAPPEDWINDOW,WINDOWSTYLE_FULLTRANSPARENT|WS_EX_TOPMOST|WS_EX_APPWINDOW,title);
    window->registerToManager();
    HWND hWnd=window->initWindow(title.c_str(),instance);
    SetLayeredWindowAttributes(hWnd,targetTransparentPixel.toCOLORREF(),0,LWA_COLORKEY);
    SetPropW(hWnd,L"isTransparent",(HANDLE)TRUE);
    bool result=window->initBuffer();
    if(!result){
        globalLogger.traceLog(Core::logger::LOG_WARNING,"Failed to init buffer, maybe will lead to UB");
    }
    return {window, hWnd};
}
namespace Graphics{
    using Core::Canvas;
    using Core::Color;
    using Core::logger;
    using Window::Handle; 
    using Window::globalHandleManager;
    using Window::Painter;
    using Window::Point;
}