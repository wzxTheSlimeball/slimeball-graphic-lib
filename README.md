[![Windows](https://img.shields.io/badge/platform-Windows-blue)]() [![C++](https://img.shields.io/badge/language-C%2B%2B-00599C)]() [![License](https://img.shields.io/badge/license-MIT-green)]() [![Status](https://img.shields.io/badge/status-active-brightgreen)]()

# Z-Multiplier Lib
------------------
#### **Still under development**

本图形库是基于GDI和GDI+的**CPU 2D**的，**Windows平台**~~图形~~游戏库，可以自主实现3D
This library is a **CPU-based 2D ~~graphics~~ game library** for **Windows**, built on GDI and GDI+,allows you to implement 3D rendering manually.

#### 主要特点：

- 易用性和易维护性平衡：函数调用自然，融入逻辑，同时几乎所有绘制函数都返回`bool`类型进行错误处理
- 完善的日志系统：通过`logger`调用日志，支持绑定不同的输出流，预设globalLogger绑定`clog`，可通过`freopen`等手段重定向至文件
- 功能全面：支持常用的图形，图片加载和字体加载，窗口回调齐全，均可自定义，详见下方样例
- 模块化：易于寻找声明及实现

#### Main features:

- Balance of ease of use and maintenance: function calls are natural and integrated into logic, while almost all drawing functions return the `bool` type for error handling
- Complete log system: call logs through `logger`, support binding different output streams, default globalLogger is bound to `clog`, and can be redirected to files through `freopen` and other means
- Comprehensive functions: supports commonly used graphics, image loading and font loading, comprehensive window callback support, all can be customized, see the example below for details
- Modular: easy to find declarations and implementations

下面是一个示例按钮的样例代码：
Here is an example code of an example button:
```cpp
#define IGNORE_WARNING_LOG //Ignore all the warning logs
#include "Graphics.hpp"//main file
#include "BasicUI.hpp"//UI file
using namespace Graphics;//Just for convenience
int main(){
    auto mainWindow=createInitWindow(0,0,207,230,L"Window");//init 1 window.
    //+7 +30 if for the non-client area
    UI::Button btn(LOCATEMODE_CENTER,{100,100},50,30,Color((unsigned char)200,200,200),Color((unsigned char)0,0,0));
    //init 1 button
    btn.react.push_back([=](){std::cerr<<"Btn clicked!"<<std::endl;return;});//onclick
    mainWindow.first->thisPaint=[&](HWND,UINT,WPARAM,LPARAM,Painter& p)->long long {
        p.drawBackground(Color((unsigned char)255,255,255));
        btn.show(p);
        return 0;
    };//drawing function
    mainWindow.first->thisInstantLeftClick=[&](HWND,UINT,WPARAM,LPARAM,int x,int y)->long long {
        if(btn.isOnHover(x,y)){
            btn.react.back()();
        }
        return 0;
    };//clicking function
    Clock c([&](){
        return;
    });//clock
    while(c){
        c.run();
    }
    return 0;
}
```

你可以查看`./include/BasicUI.hpp`以获取所有UI控件

Check `./include/BasicUI.hpp` for all the UIs.

本项目采用[MIT许可](/COPYING)

This project is licensed under the [MIT license](/COPYING).

#### 安装与配置

#### Installation

**Notice that you need GDI and GDI+ to compile,which usually already exist in Windows SDK,but just in case some beginners may put meaningless issues**

I'm sorry,but there is not a command that could copy now.
If you wanna use now,just download the [`/src`](/src/) and [`/include`](/include/), and that should be enough.