#include "Graphics.hpp"
using namespace Graphics;

long long mainWindowDrawer(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam,Painter& painter){
    painter.drawBackground(Color((unsigned char)255,255,255,255));
    painter.bezierCurve({{10,10},{10,190},{190,190}},5,Color((unsigned char)0,0,0,255));
    painter.bezierCurve({{210,10},{210,190},{390,190}},10,Color((unsigned char)0,0,0,255));
    painter.bezierCurve({{410,10},{410,190},{590,190}},20,Color((unsigned char)0,0,0,255));
    return 0;
}

int main(){
    auto mainWindow=createInitWindow(0,0,607,230,L"Window");
    std::function<long long(HWND,UINT,WPARAM,LPARAM,Painter&)> mainWindowDrawerFunc=mainWindowDrawer;
    mainWindow.first->thisPaint=mainWindowDrawerFunc;

    MSG msg={};
    while(msg.message!=WM_QUIT){
        while(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(16);
        globalHandleManager.updateAll();
        PostMessage(mainWindow.second,WM_PAINT,0,0);
    }
    return 0;
}