//MIT License

//Copyright (c) 2026 Z-Multiplier

#ifndef UTILS_HPP
#define UTILS_HPP
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include "Window.hpp"
#include <windows.h>
#include <string>

namespace Utils{
    BOOL classIsRegistered(wchar_t const*CLASSNAME);//艹我把const加在wchar_t前面了（
}

#endif