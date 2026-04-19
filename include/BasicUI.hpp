//MIT License

//Copyright (c) 2026 Z-Multiplier
#ifndef BASICUI_HPP
#define BASICUI_HPP
#include <optional>
#include <variant>
#include <utility>
#include <vector>
#include <functional>
#include "Image.hpp"
#include "Color.hpp"
#include "Painter.hpp"
namespace UI{
    using Texture=std::variant<Core::Color,Assets::Image>;
    struct Button{
        char locateMode;
        Window::Point locator;
        int width;
        int height;
        Texture texture;
        Core::Color frame;
        bool usingImage;
        bool show(Window::Painter& p);
        bool isOnHover(int,int);
        std::vector<std::function<void()>> react;
        Button(char locateMode,Window::Point locator,int w,int h,Assets::Image img,
               Core::Color frame):
               locateMode(locateMode),locator(locator),width(w),height(h),texture(std::move(img)),frame(frame),
               usingImage(true){};
        Button(char locateMode,Window::Point locator,int w,int h,Core::Color body,
               Core::Color frame):
               locateMode(locateMode),locator(locator),width(w),height(h),texture(body),frame(frame),
               usingImage(false){};
        ~Button()=default;
        Button()=default;
        Button(Button& btn)=delete;
        Button& operator=(Button& btn)=delete;
        Button(Button&& btn)=default;
        Button& operator=(Button&& btn)=default;
    };
    struct Checkbox{
        char locateMode;
        Window::Point locator;
        int size;
        int borderRadius;
        Core::Color body;
        Core::Color tick;
        bool ticked;
        bool show(Window::Painter& p);
        bool switchStatus(int x,int y);
        ~Checkbox()=default;
        Checkbox()=delete;
        Checkbox(char locateMode,Window::Point locator,int size,int radius,Core::Color body,Core::Color tick,
                 bool defaultStatus):locateMode(locateMode),locator(locator),size(size),borderRadius(radius),
                 body(body),tick(tick),ticked(defaultStatus){};
    };
    struct SingleChooseButtonGroup{
        struct SingleChooseButton{
            Window::Point center;
            int size;
            Core::Color color;
            Core::Color backGround;
            bool choosed;
            SingleChooseButton(Window::Point center,int size,Core::Color main,Core::Color bkground):
                               center(center),size(size),color(main),backGround(bkground),choosed(false){};
        };
        std::vector<SingleChooseButton> group;
        bool show(Window::Painter& p);
        bool react(int x,int y);
        SingleChooseButtonGroup(std::vector<SingleChooseButton> arr):group(arr){};
    };
    struct TextBar{
        char locateMode;
        Window::Point locator;
        int xoffset,yoffset;//for text;
        int width,height;
        Core::Color bkc,txtc;
        Assets::Font font;
        std::wstring str;
        bool captured;
        TextBar(char locateMode,Window::Point locator,int xoffset,int yoffset,int w,int h,Core::Color bkc,
                Core::Color txtc,Assets::Font font):locateMode(locateMode),locator(locator),xoffset(xoffset),yoffset(yoffset),
                                                    width(w),height(h),bkc(bkc),txtc(txtc),font(font),captured(false){
                                                        str=L"";
                                                    };
        bool show(Window::Painter& p);
        void append(wchar_t wc){
            str+=wc;
        }
        void pop(){
            if(!str.empty())
                str.pop_back();
        }
        bool tryCapture(int x,int y);
    };
    struct ProgressBar{
        char locateMode;
        Window::Point locator;
        float progress;//in [0,1]
        int width,height;
        Core::Color body;
        Core::Color showc;
        ProgressBar(char locateMode,Window::Point locator,int width,int height,Core::Color body,Core::Color showc):
                    locateMode(locateMode),locator(locator),progress(0.0f),width(width),height(height),body(body),showc(showc){};
        bool show(Window::Painter& p);
        void delta(float delta);
    };
};

#endif