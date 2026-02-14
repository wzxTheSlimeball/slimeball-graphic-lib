#include "Graphics.hpp"
using namespace Graphics;

long long mainWindowPaint(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam,Painter& painter){
    painter.drawBackground(Color((unsigned char)255,0,0,255));
    return 0;
}
long long mainWindowLeftClickInstant(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam,int x,int y){
    Assets::saveScreen(hWnd,L"screen.png",L"png");
    return 0;
}
int main(){
    auto mainWindow=createInitWindow(400,400,200,210,L"Window");
    mainWindow.first->thisPaint=mainWindowPaint;
    mainWindow.first->thisInstantLeftClick=mainWindowLeftClickInstant;
    MSG msg={};
    while(msg.message!=WM_QUIT){
        while(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(16);
        globalHandleManager.updateAll();
    }
}