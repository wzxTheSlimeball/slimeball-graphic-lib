#include "Graphics.hpp"

using namespace Graphics;

enum CollisionAxis:unsigned char{CollisionNone=0,XCollide=1,YCollide=2,BothCollide=3};

struct block{
    int x,y,width,height;
    block(int x,int y,int w,int h){
        this->x=x;
        this->y=y;
        this->width=w;
        this->height=h;
    }
    bool detectX(int x)const{
        return (x>this->x)&&(x<this->x+this->width);
    }
    bool detectY(int y)const{
        return (y>this->y)&&(y<this->y+this->height);
    }
    CollisionAxis intersectsAABB(int left,int top,int right,int bottom)const{
        bool overlapX=!(right<=this->x || left>=this->x+this->width);
        bool overlapY=!(bottom<=this->y || top>=this->y+this->height);
        if(overlapX&&overlapY) return BothCollide;
        if(overlapX) return XCollide;
        if(overlapY) return YCollide;
        return CollisionNone;
    }
};
vector<block> globalBlockManager;
static Assets::Image normal,left,right;
static int playerX=100,playerY=120;
static bool shouldReduceSpeed=false;
static int currentSpeed=0;
static int currentXSpeed=0;
static bool landedSomething=false;
static bool ignoreCollide=false;
long long mainWindowPaint(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam,Painter& painter){
    landedSomething=false;
    painter.drawBackground(Color((unsigned char)200,200,200,255));
    for(const auto& earth:globalBlockManager){
        painter.line(Point(earth.x,earth.y),Point(earth.x,earth.y+earth.height),Color((unsigned char)0,0,0,255));
        painter.line(Point(earth.x+earth.width,earth.y+earth.height),Point(earth.x,earth.y+earth.height),Color((unsigned char)0,0,0,255));
        painter.line(Point(earth.x+earth.width,earth.y),Point(earth.x+earth.width,earth.y+earth.height),Color((unsigned char)0,0,0,255));
        painter.line(Point(earth.x,earth.y),Point(earth.x+earth.width,earth.y),Color((unsigned char)0,0,0,255));
    }
    int nextPlayerX=playerX+currentXSpeed;
    int nextPlayerY=playerY-currentSpeed;
    int plH=nextPlayerX-13;
    int prH=nextPlayerX+13;
    int ptH=playerY-13;
    int pbH=playerY+13;
    bool collideH=false;
    for(const auto& earth:globalBlockManager){
        CollisionAxis ca=earth.intersectsAABB(plH,ptH,prH,pbH);
        globalLogger.formatLog(logger::LOG_DEBUG,"H box L=%d T=%d R=%d B=%d, earth x=%d y=%d w=%d h=%d, ca=%d",plH,ptH,prH,pbH,earth.x,earth.y,earth.width,earth.height,(int)ca);
        if(ca==XCollide){collideH=true;break;}
    }
    if(collideH){currentXSpeed=0;}else{playerX=nextPlayerX;}
    int plV=playerX-13;
    int prV=playerX+13;
    int ptV=nextPlayerY-13;
    int pbV=nextPlayerY+13;
    bool collideV=false;
    const block* collidedBlock=nullptr;
    for(const auto& earth:globalBlockManager){
        CollisionAxis ca=earth.intersectsAABB(plV,ptV,prV,pbV);
        globalLogger.formatLog(logger::LOG_DEBUG,"V box L=%d T=%d R=%d B=%d, earth x=%d y=%d w=%d h=%d, ca=%d",plV,ptV,prV,pbV,earth.x,earth.y,earth.width,earth.height,(int)ca);
        if(ca==YCollide||ca==BothCollide){collideV=true;collidedBlock=&earth;break;}
    }
    if(collideV&&!ignoreCollide){
        landedSomething=true;
        if(collidedBlock){
            if(currentSpeed<0){playerY=collidedBlock->y-13;}else if(currentSpeed>0){playerY=collidedBlock->y+collidedBlock->height+13;}
        }
        currentSpeed=0;
    }else{playerY=nextPlayerY;}
    if(ignoreCollide){
        ignoreCollide=!ignoreCollide;
    }
    if(currentXSpeed!=0&&shouldReduceSpeed){
        if(currentXSpeed<0){
            currentXSpeed++;
        }
        else{
            currentXSpeed--;
        }
    }
    if(!landedSomething){
        currentSpeed-=2;
    }
    shouldReduceSpeed=!shouldReduceSpeed;
    if(currentXSpeed==0)
    painter.putImage(LOCATEMODE_CENTER,Point(playerX,playerY),normal,255);
    else if(currentXSpeed>0)
    painter.putImage(LOCATEMODE_CENTER,Point(playerX,playerY),right,255);
    else if(currentXSpeed<0)
    painter.putImage(LOCATEMODE_CENTER,Point(playerX,playerY),left,255);
    return 0;
}
long long mainWindowGetChar(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam,wchar_t get,bool,int,bool,bool){
    if(get=='d'){
        if(currentXSpeed<8)
            currentXSpeed+=2;
    }
    if(get=='a'){
        if(currentXSpeed>-8)
            currentXSpeed-=2;
    }
    if(get=='w'){
        if(currentSpeed==0){
            ignoreCollide=true;
            currentSpeed=12;
        }
    }
    return 0;
}
int main(){
    block birth=block(50,150,100,50);
    block sec=block(50,60,100,50);
    globalBlockManager.push_back(birth);
    globalBlockManager.push_back(sec);
    normal=Assets::Image(L"./player_normal.png");
    left=Assets::Image(L"./player_left.png");
    right=Assets::Image(L"./player_right.png");
    auto mainWindow=createInitWindow(400,400,200,200,L"Window");
    mainWindow.first->thisPaint=mainWindowPaint;
    mainWindow.first->thisGetChar=mainWindowGetChar;
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