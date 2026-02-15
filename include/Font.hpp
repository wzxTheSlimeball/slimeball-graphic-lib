//MIT License

//Copyright (c) 2026 Z-Multiplier
#ifndef FONT_HPP
#define FONT_HPP
#include <windows.h>
#include <string>

#define FONTWEIGHT_MEDIUM FW_MEDIUM
#define FONTWEIGHT_NORMAL FW_NORMAL
#define FONTWEIGHT_BOLD FW_BOLD
#define FONTWEIGHT_EXBOLD FW_EXTRABOLD
#define FONTWEIGHT_THIN FW_THIN
#define FONTWEIGHT_EXLIGHT FW_EXTRALIGHT
#define FONTWEIGHT_LIGHT FW_LIGHT
#define FONTWEIGHT_DONT_CARE FW_DONTCARE
#define FONTWEIGHT_DEMIBOLD FW_DEMIBOLD
#define FONTWEIGHT_SEMIBOLD FW_SEMIBOLD
#define FONTWEIGHT_HEAVY FW_HEAVY
#define FONTWEIGHT_REGULAR FW_REGULAR
#define FONTWEIGHT_ULTRABOLD FW_ULTRABOLD
#define FONTWEIGHT_ULTRALIGHT FW_ULTRALIGHT
#define FONTWEIGHT_BLACK FW_BLACK

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

namespace Assets{
    struct Font{
        private:
            LOGFONTW thisLogFont;
            std::wstring thisName;
            int height;
            int width;
            long weight;
            bool italic;
            bool strikethrough;
            bool underlined;
        public:
            HFONT thisHFont;
            Font();
            Font(std::wstring name);
            Font(std::wstring name,int height,int width);
            Font(std::wstring name,int height,int width,long weight);
            Font(std::wstring name,int height,int width,long weight,bool italic,bool strikethrough,bool underlined);
            ~Font();
            void loadFont();
    };
}

#endif // FONT_HPP