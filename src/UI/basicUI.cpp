//MIT License

//Copyright (c) 2026 Z-Multiplier
#include "BasicUI.hpp"
bool UI::Button::show(Window::Painter& p){
    Window::Point locate=Window::calculateDrawPosition(locateMode,locator,width,height);
    if(usingImage){
        if(!p.putImage(locateMode,locator,std::get<Assets::Image>(texture),255)){
            return false;
        }
        if(!p.hollowPolygon({locate,Window::Point(locate.x+width,locate.y),
            Window::Point(locate.x+width,locate.y+height),Window::Point(locate.x,locate.y+height)},
            frame)){
            return false;
        }
    }
    else{
        if(!p.solidPolygon({locate,Window::Point(locate.x+width,locate.y),
            Window::Point(locate.x+width,locate.y+height),Window::Point(locate.x,locate.y+height)},
            std::get<Core::Color>(texture))){
            return false;
        }
        if(!p.hollowPolygon({locate,Window::Point(locate.x+width,locate.y),
            Window::Point(locate.x+width,locate.y+height),Window::Point(locate.x,locate.y+height)},
            frame)){
            return false;
        }
    }
    return true;
}
bool UI::Button::isOnHover(int x,int y){
    auto [lx,ly]=Window::calculateDrawPosition(locateMode,locator,width,height);
    return lx<x&&
           ly<y&&
           lx+this->width>x&&
           ly+this->height>y;
}
bool UI::Checkbox::show(Window::Painter& p){
    Window::Point center=Window::calculateDrawPosition(locateMode,locator,size,size);
    center.x+=size/2;
    center.y+=size/2;
    if(this->borderRadius>this->size/2){
        this->borderRadius=this->size/2;
    }
    int half=this->size/2;
    int innerHalf=half-this->borderRadius;
    int cx=center.x;
    int cy=center.y;
    if(innerHalf>0){
        std::vector<Window::Point> rect={
            {cx-innerHalf,cy-innerHalf},
            {cx+innerHalf,cy-innerHalf},
            {cx+innerHalf,cy+innerHalf},
            {cx-innerHalf,cy+innerHalf}
        };
        p.solidPolygon(rect,this->body);
    }
    if(this->borderRadius>0){
        std::vector<Window::Point> topRect={
            {cx-innerHalf,cy-half},
            {cx+innerHalf,cy-half},
            {cx+innerHalf,cy-innerHalf},
            {cx-innerHalf,cy-innerHalf}
        };
        p.solidPolygon(topRect,this->body);
        std::vector<Window::Point> bottomRect={
            {cx-innerHalf,cy+innerHalf},
            {cx+innerHalf,cy+innerHalf},
            {cx+innerHalf,cy+half},
            {cx-innerHalf,cy+half}
        };
        p.solidPolygon(bottomRect,this->body);
        std::vector<Window::Point> leftRect={
            {cx-half,cy-innerHalf},
            {cx-innerHalf,cy-innerHalf},
            {cx-innerHalf,cy+innerHalf},
            {cx-half,cy+innerHalf}
        };
        p.solidPolygon(leftRect,this->body);
        std::vector<Window::Point> rightRect={
            {cx+innerHalf,cy-innerHalf},
            {cx+half,cy-innerHalf},
            {cx+half,cy+innerHalf},
            {cx+innerHalf,cy+innerHalf}
        };
        p.solidPolygon(rightRect,this->body);
    }
    if(this->borderRadius>0){
        p.solidCircle({cx-innerHalf,cy-innerHalf},this->borderRadius,this->body);
        p.solidCircle({cx+innerHalf,cy-innerHalf},this->borderRadius,this->body);
        p.solidCircle({cx-innerHalf,cy+innerHalf},this->borderRadius,this->body);
        p.solidCircle({cx+innerHalf,cy+innerHalf},this->borderRadius,this->body);
    }
    if(this->ticked){
        p.setSize(2);
        p.line({cx-innerHalf,cy},{cx,cy+innerHalf},this->tick);
        p.line({cx,cy+innerHalf},{cx+innerHalf,cy-innerHalf},this->tick);
        p.setSize(0);
    }
    return true;
}
bool UI::Checkbox::switchStatus(int x,int y){
    auto [cx,cy]=Window::calculateDrawPosition(locateMode,locator,size,size);
    if(x>cx&&x<cx+size&&y>cy&&y<cy+size){
        this->ticked=!this->ticked;
        return true;
    }
    return false;
}
bool UI::SingleChooseButtonGroup::show(Window::Painter& p){
    for(const auto& scb:group){
        if(!p.solidCircle(scb.center,scb.size-1,scb.backGround)){
            return false;
        }
        if(!p.solidCircle(scb.center,scb.size-3,scb.choosed?scb.color:scb.backGround)){
            return false;
        }
        if(!p.hollowCircle(scb.center,scb.size,scb.color)){
            return false;
        }
    }
    return true;
}
bool UI::SingleChooseButtonGroup::react(int x,int y){
    bool ret=false;
    size_t idx=-1;
    for(size_t i=0;i<group.size();i++){
        auto& scb=group[i];
        auto [cx,cy]=scb.center;
        double dist=sqrt((cx-x)*(cx-x)+(cy-y)*(cy-y));
        if(dist<scb.size){
            ret=true;
            idx=i;
        }
    }
    if(ret){
        for(size_t i=0;i<group.size();i++){
            auto& scb=group[i];
            if(i!=idx){
                scb.choosed=false;
            }
            else{
                scb.choosed=true;
            }
        }
    }
    return ret;
}
bool UI::TextBar::show(Window::Painter& p){
    auto [cx,cy]=Window::calculateDrawPosition(locateMode,locator,width,height);
    if(!p.solidPolygon({{cx,cy},{cx+width,cy},{cx+width,cy+height},{cx,cy+height}},bkc)){
        return false;
    }
    if(!p.putText(LOCATEMODE_LEFTUPCORNER,{cx+xoffset,cy+yoffset},font,str,txtc)){
        return false;
    }
    return true;
}
bool UI::TextBar::tryCapture(int x,int y){
    auto [cx,cy]=Window::calculateDrawPosition(locateMode,locator,width,height);
    if(x>cx&&x<cx+width&&y>cy&&y<cy+height){
        this->captured=true;
    }
    else{
        this->captured=false;
    }
    return this->captured;
}
bool UI::ProgressBar::show(Window::Painter& p){
    auto [cx,cy]=Window::calculateDrawPosition(locateMode,locator,width,height);
    if(!p.solidPolygon({{cx,cy},{cx+width,cy},{cx+width,cy+height},{cx,cy+height}},body)){
        return false;
    }
    if(!p.solidPolygon({{cx+1,cy+1},{cx+static_cast<int>(width*progress)-1,cy+1},{cx+static_cast<int>(width*progress)-1,cy+height-1},{cx+1,cy+height-1}},showc)){
        return false;
    }
    return true;
}
void UI::ProgressBar::delta(float delta){
    this->progress+=delta;
}