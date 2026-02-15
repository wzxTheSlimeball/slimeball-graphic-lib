//MIT License

//Copyright (c) 2026 Z-Multiplier
#ifndef WINDOW_HPP
#define WINDOW_HPP
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include <windows.h>
#include <string>
#include <functional>
#include <memory>
#include "Canvas.hpp"
#include "Utils.hpp"
#include "Painter.hpp"

#define KEY_SHIFT VK_SHIFT
#define KEY_LSHIFT VK_LSHIFT
#define KEY_RSHIFT VK_RSHIFT
#define KEY_CTRL VK_CONTROL
#define KEY_RCTRL VK_RCONTROL
#define KEY_LCTRL VK_LCONTROL
#define KEY_CAPSLOCK VK_CAPITAL
#define KEY_TAB VK_TAB
#define KEY_LWIN VK_LWIN
#define KEY_RWIN VK_RWIN
#define KEY_SPACE VK_SPACE
#define KEY_FN VK_FN
#define KEY_BACKSPACE VK_BACK
#define KEY_CLEAR VK_CLEAR
#define KEY_ENTER VK_RETURN
#define KEY_RETURN VK_RETURN
#define KEY_ALT VK_MENU
#define KEY_MENU VK_MENU
#define KEY_LALT VK_LMENU
#define KEY_LMENU VK_LMENU
#define KEY_RMENU VK_RMENU
#define KEY_RALT VK_RMENU
#define KEY_PAUSE VK_PAUSE
#define KEY_ESC VK_ESCAPE
#define KEY_ESCAPE VK_ESCAPE
#define KEY_PGUP VK_PRIOR
#define KEY_PAGEUP VK_PRIOR
#define KEY_PRIOR VK_PRIOR
#define KEY_PGDOWN VK_NEXT
#define KEY_PAGEDOWN VK_NEXT
#define KEY_NEXT VK_NEXT
#define KEY_HOME VK_HOME
#define KEY_END VK_END
#define KEY_ARROWUP VK_UP
#define KEY_UP VK_UP
#define KEY_ARROWDOWN VK_DOWN
#define KEY_DOWN VK_DOWN
#define KEY_ARROWLEFT VK_LEFT
#define KEY_LEFT VK_LEFT
#define KEY_ARROWRIGHT VK_RIGHT
#define KEY_RIGHT VK_RIGHT
#define KEY_SELECT VK_SELECT
#define KEY_PRINT VK_PRINT
#define KEY_SNAPSHOT VK_SNAPSHOT
#define KEY_PRINTSCREEN VK_SNAPSHOT
#define KEY_INSERT VK_INSERT
#define KEY_DELETE VK_DELETE
#define KEY_HELP VK_HELP
#define KEY_0 0x30
#define KEY_1 0x31
#define KEY_2 0x32
#define KEY_3 0x33
#define KEY_4 0x34
#define KEY_5 0x35
#define KEY_6 0x36
#define KEY_7 0x37
#define KEY_8 0x38
#define KEY_9 0x39
#define KEY_A 0x41
#define KEY_B 0x42
#define KEY_C 0x43
#define KEY_D 0x44
#define KEY_E 0x45
#define KEY_F 0x46
#define KEY_G 0x47
#define KEY_H 0x48
#define KEY_I 0x49
#define KEY_J 0x4A
#define KEY_K 0x4B
#define KEY_L 0x4C
#define KEY_M 0x4D
#define KEY_N 0x4E
#define KEY_O 0x4F
#define KEY_P 0x50
#define KEY_Q 0x51
#define KEY_R 0x52
#define KEY_S 0x53
#define KEY_T 0x54
#define KEY_U 0x55
#define KEY_V 0x56
#define KEY_W 0x57
#define KEY_X 0x58
#define KEY_Y 0x59
#define KEY_Z 0x5A
#define KEY_APPS VK_APPS
#define KEY_SLEEP VK_SLEEP
#define KEY_NUMPAD0 VK_NUMPAD0
#define KEY_NUMPAD1 VK_NUMPAD1
#define KEY_NUMPAD2 VK_NUMPAD2
#define KEY_NUMPAD3 VK_NUMPAD3
#define KEY_NUMPAD4 VK_NUMPAD4
#define KEY_NUMPAD5 VK_NUMPAD5
#define KEY_NUMPAD6 VK_NUMPAD6
#define KEY_NUMPAD7 VK_NUMPAD7
#define KEY_NUMPAD8 VK_NUMPAD8
#define KEY_NUMPAD9 VK_NUMPAD9
#define KEY_ADD VK_ADD
#define KEY_MINUS VK_SUBTRACT
#define KEY_SUBTRACT VK_SUBTRACT
#define KEY_MULTIPLY VK_MULTIPLY
#define KEY_DEVIDE VK_DEVIDE
#define KEY_SEPARATOR VK_SEPARATOR
#define KEY_DECIMAL VK_DECIMAL
#define KEY_F1 VK_F1
#define KEY_F2 VK_F2
#define KEY_F3 VK_F3
#define KEY_F4 VK_F4
#define KEY_F5 VK_F5
#define KEY_F6 VK_F6
#define KEY_F7 VK_F7
#define KEY_F8 VK_F8
#define KEY_F9 VK_F9
#define KEY_F10 VK_F10
#define KEY_F11 VK_F11
#define KEY_F12 VK_F12
#define KEY_F13 VK_F13
#define KEY_F14 VK_F14
#define KEY_F15 VK_F15
#define KEY_F16 VK_F16
#define KEY_F17 VK_F17
#define KEY_F18 VK_F18
#define KEY_F19 VK_F19
#define KEY_F20 VK_F20
#define KEY_F21 VK_F21
#define KEY_F22 VK_F22
#define KEY_F23 VK_F23
#define KEY_F24 VK_F24
#define KEY_NUMLOCK VK_NUMLOCK
#define KEY_SCROLLLOCK VK_SCROLL
#define KEY_SCROLL VK_SCROLL

#define WINDOWSTYLE_DEFAULT WS_SYSMENU|WS_CAPTION
#define WINDOWSTYLE_FULLTRANSPARENT WS_EX_LAYERED|WS_EX_TRANSPARENT|WS_EX_TOOLWINDOW
#define WINDOWSTYLE_TRANSPARENT WS_EX_LAYERED|WS_EX_APPWINDOW

#define HITSTATUS_INCLIENT HTCLIENT
#define HITRETURNSTATUS_DRAG HTCAPTION
#define HITRETURNSTATUS_HELP HTHELP
#define HITRETURNSTATUS_CLOSE HTCLOSE
#define HITSTATUS_EMPTY HTNOWHERE
#define HITSTATUS_NOWHERE HTNOWHERE
#define HITSTATUS_OUTSIDE HTNOWHERE
#define HITSTATUS_THROUGH HTTRANSPARENT
#define HITRETURNSTATUS_ERROR HTERROR

#define MOVEMODE_DELTA true
#define MOVEMODE_POS false

namespace Window{
    struct Handle:public std::enable_shared_from_this<Handle>{
        private:
            int x,y;
            int width,height;
            HWND mHWnd;
            int mWindowStyle;
            int mExtraWindowStyle;
            std::wstring mTitle;
            Handle *mParentWindow;
            long long mID;
            bool isActive=true;
            Buffer mBuffer;
            std::chrono::time_point<std::chrono::high_resolution_clock> lbuttonDownTime;
            std::chrono::time_point<std::chrono::high_resolution_clock> rbuttonDownTime;
            bool isCaptured=false;
        public:
            static std::unordered_map<HWND,Handle*> mHWndMap;
            Core::Canvas thisCanvas;
            std::function<long long(HWND,UINT,WPARAM,LPARAM)> thisDestroy;
            std::function<long long(HWND,UINT,WPARAM,LPARAM)> thisOnClose;//return true to close,false to not close
            std::function<long long(HWND,UINT,WPARAM,LPARAM,Window::Painter&)> thisPaint;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,Window::Painter&)> thisRefresh;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,int,int)> thisLeftClick;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,int,int)> thisRightClick;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,int,int,unsigned long long)> thisLeftHeld;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,int,int,unsigned long long)> thisRightHeld;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,int,int)> thisLeftDoubleClick;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,int,int)> thisRightDoubleClick;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,int,int,int)> thisMouseWheel;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,int,int)> thisMouseMove;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,int,int,int)> thisMouseHWheel;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,int,int)> thisOnHover;
            std::function<long long(HWND,UINT,WPARAM,LPARAM)> thisMouseLeave;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,wchar_t,bool,int,bool,bool)> thisGetChar;//washeldBefore,repeat,isExtra,isAltpressed
            std::function<long long(HWND,UINT,WPARAM,LPARAM,UINT,bool,int,bool,bool)> thisGetOtherKey;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,UINT,bool,int,bool,bool)> thisGetSyskey;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,wchar_t,bool,int,bool,bool)> thisGetDeadChar;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,UINT,bool,bool)> thisGetCharacter;//unicode
            std::function<long long(HWND,UINT,WPARAM,LPARAM,int,int)> thisResize;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,int,int)> thisMove;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,bool)> thisActivate;//true-activate,false-inactivate
            std::function<long long(HWND,UINT,WPARAM,LPARAM)> thisCreate;
            std::function<long long(HWND,UINT,WPARAM,LPARAM)> thisBeforeCreate;
            std::function<long long(HWND,UINT,WPARAM,LPARAM)> thisFocused;
            std::function<long long(HWND,UINT,WPARAM,LPARAM)> thisUnFocused;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,HDROP)> thisDropFile;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,int,int)> thisInstantLeftClick;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,int,int)> thisInstantRightClick;
            std::function<long long(HWND,UINT,WPARAM,LPARAM,int,int,LRESULT)> thisHit;
            static LRESULT CALLBACK thisWindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
            HWND initWindow(const wchar_t* className,HINSTANCE hInstance);
            Handle()=default;
            Handle(int x,int y,int w,int h,int windowStyle,std::wstring title);
            Handle(int x,int y,int w,int h,int windowStyle,int exWindowStyle,std::wstring title);
            Handle(int x,int y,int w,int h,int windowStyle,std::wstring title,Handle *ParentWindow);
            ~Handle();
            void destroy();
            void registerToManager();
            static Handle* queryWindow(HWND hWnd);
            RECT getRect();
            HWND getHWnd();
            bool initBuffer();
            HDC getBufferHDC();
            void update();
            void clearBuffer();
            void resizeBuffer();
            Buffer& getBuffer();
            void moveWindow(bool type,int argX,int argY);
    };
    struct HandleManager{
        private:
            long long cnt=1;
            vector<long long> handlesID;
            vector<std::shared_ptr<Handle>> handles;
        public:
            bool isEmpty();
            bool checkAndQuit();
            void push(std::shared_ptr<Handle> handle);
            void pop(long long id);
            long long getCnt();
            void updateAll();
            HandleManager()=default;
            ~HandleManager();
    };
    extern HandleManager globalHandleManager;
}
#endif // WINDOW_HPP