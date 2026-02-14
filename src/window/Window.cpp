//MIT License

//Copyright (c) 2026 wzxTheSlimeball

#include "Window.hpp"
#include "Logger.hpp"
#include "Utils.hpp"
#include "Painter.hpp"
#include <winuser.h>
#include <memory>
#include <algorithm>
#include <thread>
#include <future>
namespace Window{
    Core::logger WindowLogger;
    HandleManager globalHandleManager;
    std::unordered_map<HWND,Handle*> Handle::mHWndMap;
    LRESULT CALLBACK Handle::Handle::thisWindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam){
        Handle *pThis=nullptr;
        if(mHWndMap.find(hWnd)!=mHWndMap.end()){
            pThis=mHWndMap[hWnd];
        }
        if(uMsg==WM_NCCREATE||uMsg==WM_CREATE){
            CREATESTRUCT* cs=reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis=reinterpret_cast<Handle*>(cs->lpCreateParams);
            if(pThis){
                pThis->mHWnd=hWnd;
                mHWndMap[hWnd]=pThis;
            }
        }
        if(pThis==nullptr){
            WindowLogger.traceLog(Core::logger::LOG_INFO,"Failed to find pointer \"pThis\",This will return default(or if this is the first log that the Window throws,it's all fine)");
            return DefWindowProc(hWnd,uMsg,wParam,lParam);
        }
        if(pThis->mID==0){
            WindowLogger.traceLog(Core::logger::LOG_ERROR,"You have not registered this to the manager yet!");
            return 0;
        }
        switch(uMsg){
            case WM_DESTROY:{
                pThis->destroy();
                globalHandleManager.checkAndQuit();
                if(!pThis->thisDestroy){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"The function \"thisDestroy\" is not defined yet,Skipping.");
                }
                else{
                    return pThis->thisDestroy(hWnd,uMsg,wParam,lParam);
                }
                break;
            }
            case WM_PAINT:{
                pThis->resizeBuffer();
                Window::Painter thisPainter(hWnd,pThis);
                thisPainter.switchHDC();
                if(!pThis->thisPaint){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"The function \"thisPaint\" is not defined yet,Skipping.");
                }
                else{
                    return pThis->thisPaint(hWnd,uMsg,wParam,lParam,thisPainter);
                }
                break;
            }
            case WM_CLOSE:{
                if(!pThis->thisOnClose){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"The function \"thisOnClose\" is not defined yet,Skipping.");
                }
                else{
                    if(pThis->thisOnClose(hWnd,uMsg,wParam,lParam)){
                        pThis->destroy();
                        DestroyWindow(hWnd);
                    }
                    else{
                        return 0;
                    }
                }
                break;
            }
            case WM_LBUTTONDOWN:{
                int x=LOWORD(lParam);
                int y=HIWORD(lParam);
                pThis->lbuttonDownTime=std::chrono::high_resolution_clock::now();
                SetCapture(pThis->mHWnd);
                pThis->isCaptured=true;
                if(!pThis->thisInstantLeftClick){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"Left instant activity undefined");
                }
                else{
                    return pThis->thisInstantLeftClick(hWnd,uMsg,wParam,lParam,x,y);
                }
                break;
            }
            case WM_LBUTTONUP:{
                int x=LOWORD(lParam);
                int y=HIWORD(lParam);
                auto end=std::chrono::high_resolution_clock::now();
                auto duration=std::chrono::duration_cast<std::chrono::nanoseconds>(end-pThis->lbuttonDownTime);
                if(pThis->isCaptured){
                    ReleaseCapture();
                    pThis->isCaptured=false;
                }
                if(duration.count()<=500000000){
                    if(!pThis->thisLeftClick){
                        WindowLogger.traceLog(Core::logger::LOG_WARNING,"Left click activity undefined");
                    }
                    else{
                        return pThis->thisLeftClick(hWnd,uMsg,wParam,lParam,x,y);
                    }
                }
                else{
                    if(!pThis->thisLeftHeld){
                        WindowLogger.traceLog(Core::logger::LOG_WARNING,"Left held activity undefined");
                    }
                    else{
                        return pThis->thisLeftHeld(hWnd,uMsg,wParam,lParam,x,y,duration.count());
                    }
                }
                break;
            }
            case WM_LBUTTONDBLCLK:{
                int x=LOWORD(lParam);
                int y=HIWORD(lParam);
                if(!pThis->thisLeftDoubleClick){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"Left dblclick activity undefined");
                }
                else{
                    return pThis->thisLeftDoubleClick(hWnd,uMsg,wParam,lParam,x,y);
                }
                break;
            }
            case WM_RBUTTONDOWN:{
                int x=LOWORD(lParam);
                int y=HIWORD(lParam);
                pThis->rbuttonDownTime=std::chrono::high_resolution_clock::now();
                SetCapture(pThis->mHWnd);
                pThis->isCaptured=true;
                if(!pThis->thisInstantRightClick){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"Right instant activity undefined");
                }
                else{
                    return pThis->thisInstantRightClick(hWnd,uMsg,wParam,lParam,x,y);
                }
                break;
            }
            case WM_RBUTTONUP:{
                int x=LOWORD(lParam);
                int y=HIWORD(lParam);
                auto end=std::chrono::high_resolution_clock::now();
                auto duration=std::chrono::duration_cast<std::chrono::nanoseconds>(end-pThis->rbuttonDownTime);
                if(pThis->isCaptured){
                    ReleaseCapture();
                    pThis->isCaptured=false;
                }
                if(duration.count()<=500000000){
                    if(!pThis->thisRightClick){
                        WindowLogger.traceLog(Core::logger::LOG_WARNING,"Right click activity undefined");
                    }
                    else{
                        return pThis->thisRightClick(hWnd,uMsg,wParam,lParam,x,y);
                    }
                }
                else{
                    if(!pThis->thisRightHeld){
                        WindowLogger.traceLog(Core::logger::LOG_WARNING,"Right held activity undefined");
                    }
                    else{
                        return pThis->thisRightHeld(hWnd,uMsg,wParam,lParam,x,y,duration.count());
                    }
                }
                break;
            }
            case WM_RBUTTONDBLCLK:{
                int x=LOWORD(lParam);
                int y=HIWORD(lParam);
                if(!pThis->thisRightDoubleClick){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"Right dblclick activity undefined");
                }
                else{
                    return pThis->thisRightDoubleClick(hWnd,uMsg,wParam,lParam,x,y);
                }
                break;
            }
            case WM_MOUSEWHEEL:{
                int delta=GET_WHEEL_DELTA_WPARAM(wParam);
                int x=LOWORD(lParam);
                int y=HIWORD(lParam);
                if(!pThis->thisMouseWheel){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"Mouse wheel activity undefined");
                }
                else{
                    return pThis->thisMouseWheel(hWnd,uMsg,wParam,lParam,x,y,delta);
                }
                break;
            }
            case WM_MOUSEMOVE:{
                int x=LOWORD(lParam);
                int y=HIWORD(lParam);
                if(!pThis->thisMouseMove){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"Mouse moving activity undefined");
                }
                else{
                    return pThis->thisMouseMove(hWnd,uMsg,wParam,lParam,x,y);
                }
                break;
            }
            case WM_MOUSEHWHEEL:{
                int delta=GET_WHEEL_DELTA_WPARAM(wParam);
                int x=LOWORD(lParam);
                int y=HIWORD(lParam);
                if(!pThis->thisMouseHWheel){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"Mouse wheel activity undefined");
                }
                else{
                    return pThis->thisMouseHWheel(hWnd,uMsg,wParam,lParam,x,y,delta);
                }
                break;
            }
            case WM_MOUSEHOVER:{
                int x=LOWORD(lParam);
                int y=HIWORD(lParam);
                if(!pThis->thisOnHover){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"Mouse hover activity undefined");
                }
                else{
                    return pThis->thisOnHover(hWnd,uMsg,wParam,lParam,x,y);
                }
                break;
            }
            case WM_MOUSELEAVE:{
                if(!pThis->thisMouseLeave){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"Mouse leave activity undefined");
                }
                else{
                    return pThis->thisMouseLeave(hWnd,uMsg,wParam,lParam);
                }
                break;
            }
            case WM_CHAR:{
                wchar_t ch=static_cast<wchar_t>(wParam);
                int repeat=LOWORD(lParam);
                bool isExtra=HIWORD(lParam)&0x0100;
                bool isAltPressed=HIWORD(lParam)&0x2000;
                bool wasHeldBefore=HIWORD(lParam)&0x4000;
                if(!pThis->thisGetChar){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"Get char activity undefined");
                }
                else{
                    return pThis->thisGetChar(hWnd,uMsg,wParam,lParam,ch,wasHeldBefore,repeat,isExtra,isAltPressed);
                }
                break;
            }
            case WM_KEYDOWN:{
                UINT ch=static_cast<UINT>(wParam);
                int repeat=LOWORD(lParam);
                bool isExtra=HIWORD(lParam)&0x0100;
                bool isAltPressed=HIWORD(lParam)&0x2000;
                bool wasHeldBefore=HIWORD(lParam)&0x4000;
                if(!pThis->thisGetOtherKey){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"Get key activity undefined");
                }
                else{
                    return pThis->thisGetOtherKey(hWnd,uMsg,wParam,lParam,ch,wasHeldBefore,repeat,isExtra,isAltPressed);
                }
                break;
            }
            case WM_SYSKEYDOWN:{
                UINT ch=static_cast<UINT>(wParam);
                int repeat=LOWORD(lParam);
                bool isExtra=HIWORD(lParam)&0x0100;
                bool isAltPressed=HIWORD(lParam)&0x2000;
                bool wasHeldBefore=HIWORD(lParam)&0x4000;
                if(!pThis->thisGetSyskey){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"Get syskey activity undefined");
                }
                else{
                    return pThis->thisGetSyskey(hWnd,uMsg,wParam,lParam,ch,wasHeldBefore,repeat,isExtra,isAltPressed);
                }
                break;
            }
            case WM_DEADCHAR:{
                wchar_t ch=static_cast<wchar_t>(wParam);
                int repeat=LOWORD(lParam);
                bool isExtra=HIWORD(lParam)&0x0100;
                bool isAltPressed=HIWORD(lParam)&0x2000;
                bool wasHeldBefore=HIWORD(lParam)&0x4000;
                if(!pThis->thisGetDeadChar){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"Get deadchar activity undefined");
                }
                else{
                    return pThis->thisGetDeadChar(hWnd,uMsg,wParam,lParam,ch,wasHeldBefore,repeat,isExtra,isAltPressed);
                }
                break;
            }
            case WM_IME_CHAR:{
                UINT ch=static_cast<UINT>(wParam);
                bool isExtra=HIWORD(lParam)&0x0100;
                bool isAltPressed=HIWORD(lParam)&0x2000;
                if(!pThis->thisGetCharacter){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"Get imekey activity undefined");
                }
                else{
                    return pThis->thisGetCharacter(hWnd,uMsg,wParam,lParam,ch,isExtra,isAltPressed);
                }
                break;
            }
            case WM_SIZE:{
                pThis->resizeBuffer();
                int newWidth=LOWORD(lParam);
                int newHeight=HIWORD(lParam);
                InvalidateRect(pThis->mHWnd,NULL,TRUE);
                if(!pThis->thisResize){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"The function \"thisResize\" is not defined yet,Skipping.");
                }
                else{
                    return pThis->thisResize(hWnd,uMsg,wParam,lParam,newWidth,newHeight);
                }
                break;
            }
            case WM_MOVE:{
                int newX=LOWORD(lParam);
                int newY=HIWORD(lParam);
                if(!pThis->thisMove){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"The function \"thisMove\" is not defined yet,Skipping.");
                }
                else{
                    return pThis->thisMove(hWnd,uMsg,wParam,lParam,newX,newY);
                }
                break;
            }
            case WM_ACTIVATE:{
                bool isActive=(LOWORD(wParam)!=WA_INACTIVE);
                if(!pThis->thisActivate){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"The function \"thisActivate\" is not defined yet,Skipping.");
                }
                else{
                    return pThis->thisActivate(hWnd,uMsg,wParam,lParam,isActive);
                }
                break;
            }
            case WM_NCCREATE:{
                if(!pThis->thisBeforeCreate){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"The function \"thisBeforeCreate\" is not defined yet,Skipping.");
                }
                else{
                    return pThis->thisBeforeCreate(hWnd,uMsg,wParam,lParam);
                }
                break;
            }
            case WM_SETFOCUS:{
                if(!pThis->thisFocused){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"The function \"thisFocused\" is not defined yet,Skipping.");
                }
                else{
                    return pThis->thisFocused(hWnd,uMsg,wParam,lParam);
                }
                break;
            }
            case WM_KILLFOCUS:{
                if(!pThis->thisUnFocused){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"The function \"thisUnFocused\" is not defined yet,Skipping.");
                }
                else{
                    return pThis->thisUnFocused(hWnd,uMsg,wParam,lParam);
                }
                break;
            }
            case WM_DROPFILES:{
                HDROP hDrop=reinterpret_cast<HDROP>(wParam);
                if(!pThis->thisDropFile){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"The function \"thisDropFile\" is not defined yet,Skipping.");
                }
                else{
                    return pThis->thisDropFile(hWnd,uMsg,wParam,lParam,hDrop);
                }
                break;
            }
            case WM_CREATE:{
                if(!pThis->thisCreate){
                    WindowLogger.traceLog(Core::logger::LOG_WARNING,"The function\"thisCreate\" is not defined yet,Skipping.");
                }
                else{
                    return pThis->thisCreate(hWnd,uMsg,wParam,lParam);
                }
                break;
            }
            case WM_ERASEBKGND:{
                return 0;
                break;
            }
            case WM_NCCALCSIZE:{
                if(wParam==TRUE&&GetProp(pThis->mHWnd,L"isTransparent")==(HANDLE)TRUE){
                    return 0;
                }
                break;
            }
            case WM_NCHITTEST:{
                LRESULT hit=DefWindowProc(hWnd,uMsg,wParam,lParam);
                POINT hitPos={LOWORD(lParam),HIWORD(lParam)};
                ScreenToClient(hWnd,&hitPos);
                if(!pThis->thisHit)
                return hit;
                return pThis->thisHit(hWnd,uMsg,wParam,lParam,hitPos.x,hitPos.y,hit);
            }
            case WM_DPICHANGED:{
                UINT newDpi=LOWORD(wParam);
                RECT* prcNewWindow=(RECT*)lParam;
                SetWindowPos(hWnd,NULL,prcNewWindow->left,prcNewWindow->top,prcNewWindow->right-prcNewWindow->left,prcNewWindow->bottom-prcNewWindow->top,SWP_NOZORDER|SWP_NOACTIVATE);
                return 0;
            }
        }
        return DefWindowProc(hWnd,uMsg,wParam,lParam);
    }
    HWND Handle::Handle::initWindow(const wchar_t* className,HINSTANCE hInstance){
        if(Utils::classIsRegistered(className)==FALSE){
            const wchar_t *CLASSNAME=className;
            WNDCLASS wc={};
            wc.lpfnWndProc=thisWindowProc;
            wc.lpszClassName=CLASSNAME;
            wc.hInstance=hInstance;
            wc.hbrBackground=NULL;
            RegisterClass(&wc);
        }
        this->mHWnd=CreateWindowEx(this->mExtraWindowStyle,className,this->mTitle.c_str(),this->mWindowStyle,this->x,this->y,this->width,this->height,
                                   this->mParentWindow==nullptr?NULL:this->mParentWindow->mHWnd,NULL,hInstance,this);
        if(this->mHWnd!=NULL){
            SetWindowPos(this->mHWnd,NULL,this->x,this->y,0,0,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOZORDER|SWP_NOSIZE);
            ShowWindow(this->mHWnd, SW_SHOW);
            UpdateWindow(this->mHWnd);
        }
        return this->mHWnd;
    }
    Handle::Handle(int x,int y,int w,int h,int windowStyle,std::wstring title){
        this->x=x;
        this->y=y;
        this->width=w;
        this->height=h;
        this->mWindowStyle=windowStyle;
        this->mExtraWindowStyle=WS_OVERLAPPEDWINDOW;
        this->mTitle=title;
        this->mHWnd=NULL;
        this->mParentWindow=nullptr;
        this->mID=globalHandleManager.getCnt();
        this->thisCanvas=Core::Canvas(w,h);
    }
    Handle::Handle(int x,int y,int w,int h,int windowStyle,int exWindowStyle,std::wstring title){
        this->x=x;
        this->y=y;
        this->width=w;
        this->height=h;
        this->mWindowStyle=windowStyle;
        this->mExtraWindowStyle=exWindowStyle;
        this->mTitle=title;
        this->mHWnd=NULL;
        this->mParentWindow=nullptr;
        this->mID=globalHandleManager.getCnt();
        this->thisCanvas=Core::Canvas(w,h);
    }
    Handle::Handle(int x,int y,int w,int h,int windowStyle,std::wstring title,Handle *ParentWindow){
        this->x=x;
        this->y=y;
        this->width=w;
        this->height=h;
        this->mWindowStyle=windowStyle;
        this->mTitle=title;
        this->mHWnd=NULL;
        this->mParentWindow=ParentWindow;
        this->mID=globalHandleManager.getCnt();
        this->thisCanvas=Core::Canvas(w,h);
    }
    Handle::~Handle(){
        mHWndMap.erase(this->mHWnd);
    }
    bool HandleManager::isEmpty(){
        return this->handles.empty();
    }
    bool HandleManager::checkAndQuit(){
        if(this->isEmpty()){
            WindowLogger.traceLog(Core::logger::LOG_INFO,"Quitting program");
            PostQuitMessage(0);
            return true;
        }
        return false;
    }
    void HandleManager::push(std::shared_ptr<Handle> handle){
        this->handles.push_back(handle);
        this->handlesID.push_back(cnt++);
    }
    long long HandleManager::getCnt(){
        return cnt;
    }
    HandleManager::~HandleManager(){
        for(auto &handle:handles){
            handle.reset();
        }
    }
    void HandleManager::pop(long long ID){
        auto it=std::find(this->handlesID.begin(),this->handlesID.end(), ID);
        if(it!=this->handlesID.end()){
            size_t index=std::distance(this->handlesID.begin(),it);
            this->handles.erase(this->handles.begin()+index);
            this->handlesID.erase(it);
        }
    }
    void Handle::registerToManager(){
        this->mID=globalHandleManager.getCnt();
        globalHandleManager.push(shared_from_this());
    }
    void Handle::destroy(){
        this->isActive=false;
        globalHandleManager.pop(this->mID);
    }
    Handle* Handle::queryWindow(HWND hWnd){
        if(Handle::mHWndMap.find(hWnd)!=Handle::mHWndMap.end()){
            return Handle::mHWndMap[hWnd];
        }
        return nullptr;
    }
    Handle* queryWindow(HWND hWnd){
        if(Handle::mHWndMap.find(hWnd)!=Handle::mHWndMap.end()){
            return Handle::mHWndMap[hWnd];
        }
        return nullptr;
    }
    RECT Handle::getRect(){
        RECT rect;
        GetWindowRect(this->mHWnd,&rect);
        return rect;
    }
    HWND Handle::getHWnd(){
        return this->mHWnd;
    }
    void HandleManager::updateAll(){
        for(unsigned long long idx=0;idx<handles.size();idx++){
            InvalidateRect(handles[idx]->getHWnd(),NULL,TRUE);
            UpdateWindow(handles[idx]->getHWnd());
            handles[idx]->update();
        }
    }
    bool Handle::initBuffer(){
        if(!mHWnd){
            WindowLogger.traceLog(Core::logger::LOG_ERROR,"The window doesn't have an HWnd yet,maybe it's not created properly?");
            return false;
        }
        RECT rect;
        GetClientRect(this->mHWnd,&rect);
        int rectwidth=rect.right;
        int rectheight=rect.bottom;
        HDC windowDC=GetDC(this->mHWnd);
        HDC hdc=CreateCompatibleDC(windowDC);
        if(!hdc){
            WindowLogger.traceLog(Core::logger::LOG_ERROR,"Failed to create compatible DC for buffer");
            return false;
        }
        HBITMAP hbmp=CreateCompatibleBitmap(windowDC,rectwidth,rectheight);
        ReleaseDC(this->mHWnd,windowDC);
        if(!hbmp){
            WindowLogger.traceLog(Core::logger::LOG_ERROR,"Failed to create compatible bitmap for buffer");
            DeleteDC(hdc);
            return false;
        }
        this->mBuffer.memHDC=hdc;
        this->mBuffer.hBmp=hbmp;
        this->mBuffer.oldBmp=(HBITMAP)SelectObject(hdc,hbmp);
        this->mBuffer.width=rectwidth;
        this->mBuffer.height=rectheight;
        clearBuffer();
        return true;
    }
    HDC Handle::getBufferHDC(){
        return this->mBuffer.memHDC;
    }
    void Handle::clearBuffer(){
        if(!this->mBuffer.memHDC){
            WindowLogger.traceLog(Core::logger::LOG_ERROR,"The buffer is not initialized yet,maybe you forgot to call initBuffer()?");
            return;
        }
        RECT rect;
        GetClientRect(this->mHWnd,&rect);
        FillRect(this->mBuffer.memHDC,&rect,GetSysColorBrush(COLOR_WINDOW));
    }
    void Handle::update(){
        if(!this->mBuffer.memHDC||!this->mHWnd){
            WindowLogger.traceLog(Core::logger::LOG_ERROR,"The buffer or the window is not initialized yet, your fault:P");
            return;
        }
        HDC hdc=GetDC(this->mHWnd);
        BitBlt(hdc,0,0,this->mBuffer.width,this->mBuffer.height,this->mBuffer.memHDC,0,0,SRCCOPY);
        ReleaseDC(this->mHWnd,hdc);
    }
    void Handle::resizeBuffer(){
        if(!this->mHWnd){
            WindowLogger.traceLog(Core::logger::LOG_ERROR,"window not initialized");
            return;
        }
        RECT rect;
        GetClientRect(this->mHWnd,&rect);
        int newWidth=rect.right;
        int newHeight=rect.bottom;
        if(newWidth==this->mBuffer.width&&newHeight==this->mBuffer.height){
            return;
        }
        HDC hdcScreen=GetDC(this->mHWnd);
        if(!hdcScreen){
            WindowLogger.traceLog(Core::logger::LOG_ERROR,"GetDC failed");
            return;
        }
        HDC newMemDC=CreateCompatibleDC(hdcScreen);
        HBITMAP newBitmap=CreateCompatibleBitmap(hdcScreen, newWidth, newHeight);
        ReleaseDC(this->mHWnd,hdcScreen);
        if(!newMemDC||!newBitmap){
            if(newMemDC) DeleteDC(newMemDC);
            if(newBitmap) DeleteObject(newBitmap);
            WindowLogger.traceLog(Core::logger::LOG_ERROR,"failed to create DC/bitmap");
            return;
        }
        HBITMAP prevOld=(HBITMAP)SelectObject(newMemDC,newBitmap);
        if(this->mBuffer.memHDC){
            if(this->mBuffer.oldBmp){
                SelectObject(this->mBuffer.memHDC,this->mBuffer.oldBmp);
            }
            if(this->mBuffer.hBmp){
                DeleteObject(this->mBuffer.hBmp);
            }
            DeleteDC(this->mBuffer.memHDC);
            this->mBuffer.memHDC=NULL;
            this->mBuffer.oldBmp=NULL;
            this->mBuffer.hBmp=NULL;
        }
        this->mBuffer.memHDC=newMemDC;
        this->mBuffer.hBmp=newBitmap;
        this->mBuffer.oldBmp=prevOld;
        this->mBuffer.width=newWidth;
        this->mBuffer.height=newHeight;
        clearBuffer();
    }
    Buffer& Handle::getBuffer(){
        return this->mBuffer;
    }
    void Handle::moveWindow(bool type,int argX,int argY){
        if(type){
            this->x+=argX;
            this->y+=argY;
        }
        else{
            this->x=argX;
            this->y=argY;
        }
        MoveWindow(this->mHWnd,this->x,this->y,this->width,this->height,TRUE);
    }
}