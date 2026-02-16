#include "Graphics.hpp"
using namespace Graphics;
long long mainWindowPaint(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam,Painter& painter){
    painter.drawBackground(Color((unsigned char)255,200,0,255));
    vector<Point> diamond={{200,0},{400,200},{200,400},{0,200}};
    painter.solidPolygon(diamond,Color((unsigned char)255,0,0,255));
    Assets::Font font(L"宋体",200,120,FONTWEIGHT_BLACK,false,false,false);
    font.loadFont();
    painter.putText(LOCATEMODE_CENTER,Point(200,200),font,L"福",Color((unsigned char)0,0,0,255));
    return 0;
}
int main(){
    auto mainWindow=createInitWindow(200,200,407,430,L"新年快乐");
    mainWindow.first->thisPaint=mainWindowPaint;
    MSG msg={};
    while(msg.message!=WM_QUIT){
        while(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(16);//60FPS
        globalHandleManager.updateAll();
    }
    return 0;
}