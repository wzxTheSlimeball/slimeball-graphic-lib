#include "Graphics.hpp"
using namespace Graphics;

long long mainWindowPaint(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam,Painter& painter){
    painter.drawBackground(Color((unsigned char)255,255,200,255));
    static Assets::Image img;
    static long long deg=0;
    deg++;
    img=Assets::Image(L"./www.bmp");
    Assets::Matrix m(deg);
    img.matrix=m;
    img.transformation(false);
    painter.putImage(LOCATEMODE_CENTER,Point(200,200),img,255);
    return 0;
}
int main(){
    auto mainWindow=createInitWindow(400,400,407,430,L"Window");
    mainWindow.first->thisPaint=mainWindowPaint;
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