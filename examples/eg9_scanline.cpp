#include "Graphics.hpp"
using namespace Graphics;
long long mainWindowDrawer(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam,Painter& painter){
    auto start=std::chrono::high_resolution_clock::now();
    static int x=0;//static variables to replace the global&main variables
    x++;
    x%=200;
    painter.drawBackground(Color((unsigned char)255,255,255,255));
    vector<Point> polygon({{50+x,50},{100+x,50},{125+x,75},{100+x,100},{50+x,100},{25+x,75}});
    painter.solidPolygon(polygon,Color((unsigned char)0,0,0,255));//scanline fill function
    auto end=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);
    globalLogger.varLog(Core::logger::LOG_INFO,"duration",duration.count());
    return 0;
}//this is the window's "thisPaint" function
int main(){
    auto mainWindow=createInitWindow(0,0,800,600,0,L"Window");
    std::function<long long(HWND,UINT,WPARAM,LPARAM,Painter&)> mainWindowDrawerFunc=mainWindowDrawer;
    mainWindow.first->thisPaint=mainWindowDrawerFunc;
    //the window should proceed the paint function every update.
    MSG msg={};//again,don't forget this
    while(msg.message!=WM_QUIT){
        while(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        //Proceed your main loop code here
        if(mainWindow.second!=NULL){
            //globalLogger.traceLog(Core::logger::LOG_NOTE,"loop #");
        }//Just make the compiler shut up
        Sleep(16);//60FPS
        auto start=std::chrono::high_resolution_clock::now();
        globalHandleManager.updateAll();
        auto end=std::chrono::high_resolution_clock::now();
        auto duration=std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);
        globalLogger.varLog(Core::logger::LOG_INFO,"update duration",duration.count());
    }
    return 0;
}