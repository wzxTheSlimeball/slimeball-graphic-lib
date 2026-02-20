//MIT License

//Copyright (c) 2026 Z-Multiplier
#include "Image.hpp"
#include "Window.hpp"
#include "Transformation.hpp"
#include <iostream>
#include <cmath>

namespace Assets{
    namespace{
        struct GDIManager{
            ULONG_PTR GDIToken;
            GDIManager(){
                Gdiplus::GdiplusStartupInput gpsi;
                Gdiplus::GdiplusStartup(&GDIToken,&gpsi,nullptr);
            }
            ~GDIManager(){
                Gdiplus::GdiplusShutdown(GDIToken);
            }
            GDIManager(const GDIManager& other)=delete;
            GDIManager operator=(const GDIManager& other)=delete;
        };
        static GDIManager thisStaticGDIManager;
    }
}

Assets::Image::Image(){
    this->width=0;
    this->height=0;
    this->thisBmp=nullptr;
    this->thisHBITMAP=nullptr;
}
unsigned long long Assets::Image::getWidth()const{
    return this->width;
}
unsigned long long Assets::Image::getHeight()const{
    return this->height;
}
Assets::Image::~Image(){
    if(thisBmp){
        delete thisBmp;
        thisBmp=nullptr;
    }
    if(thisHBITMAP){
        DeleteObject(thisHBITMAP);
        thisHBITMAP=nullptr;
    }
}
Assets::Image::Image(Image&& other)noexcept:width(other.width),height(other.height),thisBmp(other.thisBmp),thisContent(std::move(other.thisContent)){
    other.thisBmp=nullptr;
    other.width=other.height=0;
}
Assets::Image& Assets::Image::operator=(Image&& other)noexcept{
    if(this!=&other){
        if(thisBmp){
            delete thisBmp;
        }
        width=other.width;
        height=other.height;
        thisBmp=other.thisBmp;
        thisContent=other.thisContent;
        other.width=other.height=0;
        other.thisBmp=nullptr;
        other.thisContent.clear();
    }
    return *this;
}
Assets::Image::Image(const std::wstring& widePath){
    thisBmp=Gdiplus::Bitmap::FromFile(widePath.c_str());
    if(thisBmp){
        if(thisBmp->GetLastStatus()==Gdiplus::Ok){
            this->width=thisBmp->GetWidth();
            this->height=thisBmp->GetHeight();
        }
        else{
            delete thisBmp;
            thisBmp=nullptr;
            width=height=0;
        }
    }
}
bool Assets::Image::syncData(){
    thisContent.resize(width*height);
    Gdiplus::BitmapData bmpdata;
    Gdiplus::Rect rect(0,0,static_cast<INT>(width),static_cast<INT>(height));
    Gdiplus::Status status=thisBmp->LockBits(&rect,Gdiplus::ImageLockModeRead,PixelFormat32bppARGB,&bmpdata);
    if(status!=Gdiplus::Ok){
        thisContent.clear();
        return false;
    }
    unsigned int *src=static_cast<unsigned int*>(bmpdata.Scan0);
    size_t count=width*height;
    for(size_t i=0;i<count;i++){
        thisContent[i]=src[i];
    }
    thisBmp->UnlockBits(&bmpdata);
    return true;
}
HBITMAP Assets::Image::getHBITMAP()const{
    if(thisHBITMAP) return thisHBITMAP;
    if(!thisBmp) return nullptr;
    ULONG_PTR imgWidth=thisBmp->GetWidth();
    ULONG_PTR imgHeight=thisBmp->GetHeight();
    Gdiplus::BitmapData bitmapData;
    Gdiplus::Rect rect(0,0,static_cast<INT>(imgWidth),static_cast<INT>(imgHeight));
    if(thisBmp->LockBits(&rect,Gdiplus::ImageLockModeRead,PixelFormat32bppARGB,&bitmapData)!=Gdiplus::Ok){
        return nullptr;
    }
    BITMAPINFO bmi;
    bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth=static_cast<LONG>(imgWidth);
    bmi.bmiHeader.biHeight=-static_cast<LONG>(imgHeight);
    bmi.bmiHeader.biPlanes=1;
    bmi.bmiHeader.biBitCount=32;
    bmi.bmiHeader.biCompression=BI_RGB;
    void* pBits=nullptr;
    HBITMAP hAlphaBitmap=CreateDIBSection(nullptr,&bmi,DIB_RGB_COLORS,&pBits,nullptr,0);
    if(!hAlphaBitmap){
        thisBmp->UnlockBits(&bitmapData);
        return nullptr;
    }
    BYTE* destRow=static_cast<BYTE*>(pBits);
    BYTE* srcRow=static_cast<BYTE*>(bitmapData.Scan0);
    LONG destStride=static_cast<LONG>(imgWidth)*4;
    LONG srcStride=bitmapData.Stride;
    for(ULONG_PTR y=0;y<imgHeight;++y){
        LONG copyBytes=std::min(destStride,abs(srcStride));
        memcpy(destRow,srcRow,copyBytes);
        destRow+=destStride;
        srcRow+=srcStride;
    }
    thisBmp->UnlockBits(&bitmapData);
    thisHBITMAP=hAlphaBitmap;
    return hAlphaBitmap;
}
HBITMAP Assets::saveScreenAsHBITMAP(HWND targetHWnd){
    if(!targetHWnd) return NULL;
    RECT rc;
    GetWindowRect(targetHWnd,&rc);
    int width=rc.right-rc.left;
    int height=rc.bottom-rc.top;
    HDC hdcScreen=GetDC(NULL);
    HDC hdcMem=CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap=CreateCompatibleBitmap(hdcScreen,width,height);
    HBITMAP hOld=(HBITMAP)SelectObject(hdcMem,hBitmap);
    BOOL bRet=BitBlt(hdcMem,0,0,width,height,hdcScreen,rc.left,rc.top,SRCCOPY);
    SelectObject(hdcMem,hOld);
    DeleteDC(hdcMem);
    ReleaseDC(NULL,hdcScreen);
    return bRet?hBitmap:NULL;
}
static int GetEncoderClsid(const WCHAR* mimeType,CLSID* pClsid)
{
    UINT num=0;
    UINT size=0;
    Gdiplus::GetImageEncodersSize(&num,&size);
    if(size==0) return -1;
    Gdiplus::ImageCodecInfo* pImageCodecInfo=(Gdiplus::ImageCodecInfo*)(malloc(size));
    if(pImageCodecInfo==NULL) return -1;
    Gdiplus::GetImageEncoders(num,size,pImageCodecInfo);
    for(UINT j=0;j<num;++j)
    {
        if(wcscmp(pImageCodecInfo[j].MimeType,mimeType)==0)
        {
            *pClsid=pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return static_cast<int>(j);
        }
    }
    free(pImageCodecInfo);
    return -1;
}

bool Assets::saveScreen(HWND targetHWnd,std::wstring fileName,std::wstring type){
    if(!targetHWnd)return false;
    RECT windowRect;
    if(!GetWindowRect(targetHWnd,&windowRect))return false;
    RECT clientRect;
    if(!GetClientRect(targetHWnd,&clientRect))return false;
    POINT clientTopLeft={clientRect.left,clientRect.top};
    if(!ClientToScreen(targetHWnd,&clientTopLeft))return false;
    int clientW=clientRect.right-clientRect.left;
    int clientH=clientRect.bottom-clientRect.top;
    if(clientW<=0||clientH<=0)return false;
    HBITMAP hBmp=NULL;
    HDC hdcWindow=GetWindowDC(targetHWnd);
    if(hdcWindow){
        HDC hdcFull=CreateCompatibleDC(hdcWindow);
        if(hdcFull){
            int fullW=windowRect.right-windowRect.left;
            int fullH=windowRect.bottom-windowRect.top;
            HBITMAP hFullBmp=CreateCompatibleBitmap(hdcWindow,fullW,fullH);
            if(hFullBmp){
                HBITMAP oldFull=(HBITMAP)SelectObject(hdcFull,hFullBmp);
                BOOL ok=PrintWindow(targetHWnd,hdcFull,PW_RENDERFULLCONTENT);
                SelectObject(hdcFull,oldFull);
                if(ok){
                    HDC hdcClientMem=CreateCompatibleDC(hdcWindow);
                    if(hdcClientMem){
                        hBmp=CreateCompatibleBitmap(hdcWindow,clientW,clientH);
                        if(hBmp){
                            HBITMAP oldClient=(HBITMAP)SelectObject(hdcClientMem,hBmp);
                            int offX=clientTopLeft.x-windowRect.left;
                            int offY=clientTopLeft.y-windowRect.top;
                            HDC hdcFullSelect=CreateCompatibleDC(hdcWindow);
                            HBITMAP oldFull2=(HBITMAP)SelectObject(hdcFullSelect,hFullBmp);
                            BitBlt(hdcClientMem,0,0,clientW,clientH,hdcFullSelect,offX,offY,SRCCOPY);
                            SelectObject(hdcFullSelect,oldFull2);
                            DeleteDC(hdcFullSelect);
                            SelectObject(hdcClientMem,oldClient);
                        }
                        DeleteDC(hdcClientMem);
                    }
                }
                DeleteObject(hFullBmp);
            }
            DeleteDC(hdcFull);
        }
        ReleaseDC(targetHWnd,hdcWindow);
    }
    if(!hBmp){
        HDC hdcClient=GetDC(targetHWnd);
        if(hdcClient){
            HDC hdcMem=CreateCompatibleDC(hdcClient);
            if(hdcMem){
                hBmp=CreateCompatibleBitmap(hdcClient,clientW,clientH);
                if(hBmp){
                    HBITMAP oldBmp=(HBITMAP)SelectObject(hdcMem,hBmp);
                    LRESULT r=SendMessage(targetHWnd,WM_PRINT,(WPARAM)hdcMem,PRF_CLIENT|PRF_CHILDREN);
                    BOOL ok=(r!=0);
                    if(!ok)ok=PrintWindow(targetHWnd,hdcMem,PW_RENDERFULLCONTENT);
                    SelectObject(hdcMem,oldBmp);
                    if(!ok){DeleteObject(hBmp);hBmp=NULL;}
                }
                DeleteDC(hdcMem);
            }
            ReleaseDC(targetHWnd,hdcClient);
        }
    }
    if(!hBmp){
        HDC hdcScreen=GetDC(NULL);
        HDC hdcMem2=CreateCompatibleDC(hdcScreen);
        hBmp=CreateCompatibleBitmap(hdcScreen,clientW,clientH);
        if(hBmp){
            HBITMAP old2=(HBITMAP)SelectObject(hdcMem2,hBmp);
            BOOL bRet=BitBlt(hdcMem2,0,0,clientW,clientH,hdcScreen,clientTopLeft.x,clientTopLeft.y,SRCCOPY);
            SelectObject(hdcMem2,old2);
            if(!bRet){DeleteObject(hBmp);hBmp=NULL;}
        }
        DeleteDC(hdcMem2);
        ReleaseDC(NULL,hdcScreen);
        if(!hBmp)return false;
    }
    Gdiplus::Bitmap* bmp=Gdiplus::Bitmap::FromHBITMAP(hBmp,NULL);
    if(!bmp){
        DeleteObject(hBmp);
        return false;
    }
    std::wstring t=type;
    for(auto &c:t) c=towlower(c);
    if(t.size()>0&&t[0]==L'.') t=t.substr(1);
    const WCHAR* mime=L"image/png";
    if(t==L"png") mime=L"image/png";
    else if(t==L"jpg"||t==L"jpeg") mime=L"image/jpeg";
    else if(t==L"bmp") mime=L"image/bmp";
    else if(t==L"gif") mime=L"image/gif";
    else if(t==L"tiff"||t==L"tif") mime=L"image/tiff";
    CLSID clsid;
    if(GetEncoderClsid(mime,&clsid)<0){
        delete bmp;
        DeleteObject(hBmp);
        return false;
    }
    Gdiplus::Status status=bmp->Save(fileName.c_str(),&clsid,NULL);
    delete bmp;
    DeleteObject(hBmp);
    return status==Gdiplus::Ok;
}
void Assets::Image::transformation(bool usingGDIplus){
    if(this->width==0||this->height==0) return;
    if(!thisBmp) return;
    INT w=static_cast<INT>(this->width);
    INT h=static_cast<INT>(this->height);
    std::vector<Core::Color> srcPixels;
    srcPixels.resize(static_cast<size_t>(w)*static_cast<size_t>(h));
    Gdiplus::BitmapData bmpdata;
    Gdiplus::Rect fullRect(0,0,w,h);
    if(thisBmp->LockBits(&fullRect,Gdiplus::ImageLockModeRead,PixelFormat32bppARGB,&bmpdata)!=Gdiplus::Ok){
        return;
    }
    for(INT row=0;row<h;++row){
        intptr_t offset=static_cast<intptr_t>(row)*static_cast<intptr_t>(bmpdata.Stride);
        BYTE *rowPtr=reinterpret_cast<BYTE*>(bmpdata.Scan0)+offset;
        for(INT col=0;col<w;++col){
            unsigned int px=*reinterpret_cast<unsigned int*>(rowPtr+col*4);
            srcPixels[static_cast<size_t>(row)*static_cast<size_t>(w)+static_cast<size_t>(col)]=Core::Color(px);
        }
    }
    thisBmp->UnlockBits(&bmpdata);
    thisContent.clear();
    double cx=(static_cast<double>(w))*0.5;
    double cy=(static_cast<double>(h))*0.5;
    double a1=matrix.a1;double a2=matrix.a2;
    double b1=matrix.b1;double b2=matrix.b2;
    double cornersX[4]={0.0,static_cast<double>(w),0.0,static_cast<double>(w)};
    double cornersY[4]={0.0,0.0,static_cast<double>(h),static_cast<double>(h)};
    double minTx=1e300,minTy=1e300,maxTx=-1e300,maxTy=-1e300;
    for(int i=0;i<4;++i){
        double ox=cornersX[i]-cx;
        double oy=cornersY[i]-cy;
        double tx=a1*ox+a2*oy;
        double ty=b1*ox+b2*oy;
        if(tx<minTx) minTx=tx;
        if(tx>maxTx) maxTx=tx;
        if(ty<minTy) minTy=ty;
        if(ty>maxTy) maxTy=ty;
    }
    int destW=static_cast<int>(std::ceil(maxTx-minTx));
    int destH=static_cast<int>(std::ceil(maxTy-minTy));
    if(destW<=0) destW=w;
    if(destH<=0) destH=h;
    double mcx=a1*cx+a2*cy;
    double mcy=b1*cx+b2*cy;
    double dx=-mcx-minTx;
    double dy=-mcy-minTy;
    if(usingGDIplus){
        Gdiplus::Matrix gm;
        gm.SetElements(static_cast<Gdiplus::REAL>(a1),static_cast<Gdiplus::REAL>(a2),
                       static_cast<Gdiplus::REAL>(b1),static_cast<Gdiplus::REAL>(b2),
                       static_cast<Gdiplus::REAL>(dx),static_cast<Gdiplus::REAL>(dy));
        Gdiplus::Bitmap *dest=new Gdiplus::Bitmap(destW,destH,PixelFormat32bppARGB);
        Gdiplus::Graphics g(dest);
        g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
        g.SetTransform(&gm);
        g.DrawImage(thisBmp,0,0);
        if(thisBmp) {delete thisBmp;}
        thisBmp=dest;
        this->width=static_cast<unsigned long long>(destW);
        this->height=static_cast<unsigned long long>(destH);
        if(thisHBITMAP){ DeleteObject(thisHBITMAP);thisHBITMAP=nullptr;}
        return;
    }
    double det=a1*b2-a2*b1;
    if(det==0.0){
        return;
    }
    double ia1=b2/det;
    double ia2=-a2/det;
    double ib1=-b1/det;
    double ib2=a1/det;
    std::vector<Core::Color> dstPixels(static_cast<size_t>(destW)*static_cast<size_t>(destH));
    for(int y=0;y<destH;++y){
        for(int x=0;x<destW;++x){
            double dx_local=static_cast<double>(x)+minTx;
            double dy_local=static_cast<double>(y)+minTy;
            double srcX=cx+(ia1*dx_local+ia2*dy_local);
            double srcY=cy+(ib1*dx_local+ib2*dy_local);
            int ix=static_cast<int>(std::floor(srcX+0.5));
            int iy=static_cast<int>(std::floor(srcY+0.5));
            Core::Color px;
            if(ix>=0&&ix<w&&iy>=0&&iy<h){
                px=srcPixels[static_cast<size_t>(iy)*static_cast<size_t>(w)+static_cast<size_t>(ix)];
            }else{
                px=Core::Color((unsigned char)0,0,0,0);
            }
            dstPixels[static_cast<size_t>(y)*static_cast<size_t>(destW)+static_cast<size_t>(x)]=px;
        }
    }
    Gdiplus::Bitmap* destBmp=new Gdiplus::Bitmap(destW,destH,PixelFormat32bppARGB);
    Gdiplus::BitmapData destData;
    Gdiplus::Rect destRect(0,0,destW,destH);
    if(destBmp->LockBits(&destRect,Gdiplus::ImageLockModeWrite,PixelFormat32bppARGB,&destData)==Gdiplus::Ok){
        for(int row=0;row<destH;++row){
            intptr_t off=static_cast<intptr_t>(row)*static_cast<intptr_t>(destData.Stride);
            BYTE* dstRow=reinterpret_cast<BYTE*>(destData.Scan0)+off;
            for(int col=0;col<destW;++col){
                Core::Color c=dstPixels[static_cast<size_t>(row)*static_cast<size_t>(destW)+static_cast<size_t>(col)];
                unsigned int packed=(static_cast<unsigned int>(c.a)<<24)|(static_cast<unsigned int>(c.r)<<16)|(static_cast<unsigned int>(c.g)<<8)|(static_cast<unsigned int>(c.b));
                *reinterpret_cast<unsigned int*>(dstRow+col*4)=packed;
            }
        }
        destBmp->UnlockBits(&destData);
        if(thisBmp) delete thisBmp;
        thisBmp=destBmp;
        this->width=static_cast<unsigned long long>(destW);
        this->height=static_cast<unsigned long long>(destH);
        if(thisHBITMAP){ DeleteObject(thisHBITMAP);thisHBITMAP=nullptr;}
    }else{
        delete destBmp;
    }
}