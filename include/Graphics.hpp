// The final file.
// ATK 99  DEF 99 (?)
// It includes all necessary graphics-related headers for easy access.

//MIT License

//Copyright (c) 2026 wzxTheSlimeball

#pragma once
#include "Canvas.hpp"
#include "Color.hpp"
#include "Logger.hpp"
#include "Window.hpp"
#include "EntryPoint.hpp"
#include "Painter.hpp"

extern HINSTANCE mainHInstance;
Core::logger globalLogger;

inline std::pair<std::shared_ptr<Window::Handle>, HWND> 
createInitWindow(int x,int y,int width,int height,int style,std::wstring title,HINSTANCE instance=mainHInstance) 
{
    auto window=std::make_shared<Window::Handle>(x,y,width,height,style,title);
    window->registerToManager();
    HWND hWnd=window->initWindow(title.c_str(),instance);
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