#include "Graphics.hpp"
using namespace Graphics;
long long mainWindowDrawer(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam,Painter& painter){
    auto start=std::chrono::high_resolution_clock::now();
    painter.drawBackground(Color((unsigned char)255,255,255,255));
    painter.hollowCircle({100,100},30,Color((unsigned char)0,0,0,255));
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