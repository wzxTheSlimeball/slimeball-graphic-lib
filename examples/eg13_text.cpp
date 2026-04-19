#define IGNORE_WARNING_LOG //Ignore all the warning logs
#include "Graphics.hpp"//main file
#include "BasicUI.hpp"//UI file
using namespace Graphics;//Just for convenience
int main(){
    auto mainWindow=createInitWindow(0,0,407,430,L"Window");//init 1 window.
    //+7 +30 if for the non-client area
    UI::Button btn(LOCATEMODE_CENTER,{100,100},50,30,Color((unsigned char)200,200,200),Color((unsigned char)0,0,0));
    //init 1 button
    btn.react.push_back([=](){std::cerr<<"Btn clicked!"<<std::endl;return;});//onclick
    UI::Checkbox cb(LOCATEMODE_CENTER,{100,150},12,1,Color((unsigned char)255,127,0),Color((unsigned char)255,255,255),false);
    UI::SingleChooseButtonGroup scbg(
        {
            UI::SingleChooseButtonGroup::SingleChooseButton({50,50},7,Color((unsigned char)50,50,255),Color((unsigned char)255,255,255)),
            UI::SingleChooseButtonGroup::SingleChooseButton({100,50},7,Color((unsigned char)50,50,255),Color((unsigned char)255,255,255)),
            UI::SingleChooseButtonGroup::SingleChooseButton({150,50},7,Color((unsigned char)50,50,255),Color((unsigned char)255,255,255))
        }
    );
    Assets::Font inputf(L"Consolas",20,8,FONTWEIGHT_BLACK,false,false,false);
    inputf.loadFont();
    UI::TextBar input(LOCATEMODE_LEFTUPCORNER,{20,200},2,2,200,24,Core::Color((unsigned char)10,10,10),Core::Color((unsigned char)250,250,250),inputf);
    UI::ProgressBar pb(LOCATEMODE_LEFTUPCORNER,{20,300},200,20,Core::Color((unsigned char)10,10,10),Core::Color((unsigned char)0,150,0));
    mainWindow.first->thisPaint=[&](HWND,UINT,WPARAM,LPARAM,Painter& p)->long long {
        p.drawBackground(Color((unsigned char)255,255,255));
        btn.show(p);
        cb.show(p);
        scbg.show(p);
        input.show(p);
        pb.show(p);
        return 0;
    };//drawing function
    mainWindow.first->thisInstantLeftClick=[&](HWND,UINT,WPARAM,LPARAM,int x,int y)->long long {
        if(btn.isOnHover(x,y)){
            btn.react.back()();
        }
        cb.switchStatus(x,y);
        scbg.react(x,y);
        input.tryCapture(x,y);
        return 0;
    };//clicking function
    mainWindow.first->thisGetChar=[&](HWND,UINT,WPARAM,LPARAM,wchar_t c,bool,int,bool,bool)->long long {
        if(input.captured){
            if(c=='\b'){
                input.pop();
            }
            else{
                input.append(c);
            }
        }
        return 0;
    };
    Clock c([&](){
        if(pb.progress<1){
            pb.delta(0.01);
        }
        else{
            pb.progress=0;
        }
        return;
    });//clock
    while(c){
        c.run();
    }
    return 0;
}