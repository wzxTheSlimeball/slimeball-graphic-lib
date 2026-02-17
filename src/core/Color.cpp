//MIT License

//Copyright (c) 2026 Z-Multiplier

#include "Color.hpp"
#include <cmath>
#include <windows.h>

Core::Color::Color(float hue,float saturation,float lightness,unsigned char alpha)
{
    float c=(1.0f-fabs(2.0f*lightness-1.0f))*saturation;
    float x=static_cast<float>(c*(1.0f-fabs(fmod(hue/60.0f,2)-1.0f)));
    float m=lightness-c/2.0f;
    float r1,g1,b1;
    if(hue>=0.0f&&hue<60.0f){
        r1=c;g1=x;b1=0.0f;
    }else if(hue>=60.0f&&hue<120.0f){
        r1=x;g1=c;b1=0.0f;
    }else if(hue>=120.0f&&hue<180.0f){
        r1=0.0f;g1=c;b1=x;
    }else if(hue>=180.0f&&hue<240.0f){
        r1=0.0f;g1=x;b1=c;
    }else if(hue>=240.0f&&hue<300.0f){
        r1=x;g1=0.0f;b1=c;
    }else{
        r1=c;g1=0.0f;b1=x;
    }
    r=static_cast<unsigned char>((r1+m)*255.0f);
    g=static_cast<unsigned char>((g1+m)*255.0f);
    b=static_cast<unsigned char>((b1+m)*255.0f);
    a=alpha;
}
Core::Color::Color(unsigned int src){
    this->a=(src>>24)&0xff;
    this->r=(src>>16)&0xff;
    this->g=(src>>8)&0xff;
    this->b=src&0xff;
}