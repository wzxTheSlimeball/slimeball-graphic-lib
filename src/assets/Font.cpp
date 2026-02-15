//MIT License

//Copyright (c) 2026 Z-Multiplier
#include "Font.hpp"
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

Assets::Font::Font(){
    this->thisName=L"Arial";
    this->height=20;
    this->width=10;
    this->weight=FONTWEIGHT_BLACK;
    this->italic=this->strikethrough=this->underlined=false;
    ZeroMemory(&this->thisLogFont,sizeof(LOGFONT));
    this->thisLogFont.lfHeight=20;
    this->thisLogFont.lfWidth=10;
    this->thisLogFont.lfWeight=FW_BLACK;
    wcsncpy(this->thisLogFont.lfFaceName,L"Arial",LF_FACESIZE-1);
    this->thisLogFont.lfFaceName[LF_FACESIZE-1]=L'\0';
    this->thisLogFont.lfUnderline=false;
    this->thisLogFont.lfItalic=false;
    this->thisLogFont.lfStrikeOut=false;
}
Assets::Font::Font(std::wstring name){
    this->thisName=name;
    this->height=20;
    this->width=10;
    this->weight=FONTWEIGHT_BLACK;
    this->italic=this->strikethrough=this->underlined=false;
    ZeroMemory(&this->thisLogFont,sizeof(LOGFONT));
    this->thisLogFont.lfHeight=20;
    this->thisLogFont.lfWidth=10;
    this->thisLogFont.lfWeight=FW_BLACK;
    wcsncpy(this->thisLogFont.lfFaceName,name.c_str(),LF_FACESIZE-1);
    this->thisLogFont.lfFaceName[LF_FACESIZE-1]=L'\0';
    this->thisLogFont.lfUnderline=false;
    this->thisLogFont.lfItalic=false;
    this->thisLogFont.lfStrikeOut=false;
}
Assets::Font::Font(std::wstring name,int height,int width){
    this->thisName=name;
    this->height=height;
    this->width=width;
    this->weight=FONTWEIGHT_BLACK;
    this->italic=this->strikethrough=this->underlined=false;
    ZeroMemory(&this->thisLogFont,sizeof(LOGFONT));
    this->thisLogFont.lfHeight=height;
    this->thisLogFont.lfWidth=width;
    this->thisLogFont.lfWeight=FW_BLACK;
    wcsncpy(this->thisLogFont.lfFaceName,name.c_str(),LF_FACESIZE-1);
    this->thisLogFont.lfFaceName[LF_FACESIZE-1]=L'\0';
    this->thisLogFont.lfUnderline=false;
    this->thisLogFont.lfItalic=false;
    this->thisLogFont.lfStrikeOut=false;
}
Assets::Font::Font(std::wstring name,int height,int width,long weight){
    this->thisName=name;
    this->height=height;
    this->width=width;
    this->weight=weight;
    this->italic=this->strikethrough=this->underlined=false;
    ZeroMemory(&this->thisLogFont,sizeof(LOGFONT));
    this->thisLogFont.lfHeight=height;
    this->thisLogFont.lfWidth=width;
    this->thisLogFont.lfWeight=weight;
    wcsncpy(this->thisLogFont.lfFaceName,name.c_str(),LF_FACESIZE-1);
    this->thisLogFont.lfFaceName[LF_FACESIZE-1]=L'\0';
    this->thisLogFont.lfUnderline=false;
    this->thisLogFont.lfItalic=false;
    this->thisLogFont.lfStrikeOut=false;
}
Assets::Font::Font(std::wstring name,int height,int width,long weight,bool italic,bool strikethrough,bool underlined){
    this->thisName=name;
    this->height=height;
    this->width=width;
    this->weight=weight;
    this->italic=italic;
    this->strikethrough=strikethrough;
    this->underlined=underlined;
    ZeroMemory(&this->thisLogFont,sizeof(LOGFONT));
    this->thisLogFont.lfHeight=height;
    this->thisLogFont.lfWidth=width;
    this->thisLogFont.lfWeight=weight;
    wcsncpy(this->thisLogFont.lfFaceName,name.c_str(),LF_FACESIZE-1);
    this->thisLogFont.lfFaceName[LF_FACESIZE-1]=L'\0';
    this->thisLogFont.lfUnderline=underlined;
    this->thisLogFont.lfItalic=italic;
    this->thisLogFont.lfStrikeOut=strikethrough;
}
void Assets::Font::loadFont(){
    this->thisHFont=CreateFontIndirectW(&this->thisLogFont);
}
Assets::Font::~Font(){
    if(this->thisHFont){
        DeleteObject(this->thisHFont);
        this->thisHFont=nullptr;
    }
}