//MIT License

//Copyright (c) 2026 Z-Multiplier

#include "Utils.hpp"
#include <windows.h>

namespace Utils{
    BOOL classIsRegistered(const wchar_t *CLASSNAME){
        WNDCLASSEX wc={};
        return GetClassInfoEx(NULL,CLASSNAME,&wc);//艹
    }
}
