//MIT License

//Copyright (c) 2026 Z-Multiplier
#ifndef IMAGE_HPP
#define IMAGE_HPP
#include "Color.hpp"
#include <windows.h>
#include <string>
#include <vector>
#include <gdiplus.h>

namespace Assets{
    struct Image{
        private:
            unsigned long long width;
            unsigned long long height;
            Gdiplus::Bitmap *thisBmp;
            std::vector<Core::Color> thisContent;
            mutable HBITMAP thisHBITMAP;
        public:
            Image();
            Image(const std::wstring& widePath);
            Image(const Image& other)=delete;
            Image& operator=(const Image& other)=delete;
            Image(Image&& other)noexcept;
            Image& operator=(Image&& other)noexcept;
            ~Image();
            unsigned long long getWidth() const;
            unsigned long long getHeight() const;
            bool syncData();
            HBITMAP getHBITMAP() const;
    };
    HBITMAP saveScreenAsHBITMAP(HWND targetHWnd);
    bool saveScreen(HWND targetHWnd,std::wstring fileName,std::wstring type);
};

#endif // IMAGE_HPP