# SlimeballGraphicsLib帮助文档

*If you don't speak Chinese,scroll all the way down, you will find an English edition.(not yet)

- **版本** v0.0
- **状态** 施工中
- **最后更新** 26/02/07

## 项目信息

- **背景** 因为没有找到合适的多窗口库，又稍微懂一点面向对象，所以尝试再造一个轮子
- **目标** 
- - 主要目标：2D功能完全可用且单函数效率控制在可接受范围内（正常情况下，至少让窗口达到60fps）
- - 次要目标：如果时间宽裕，可能会尝试完成物理模拟及3D渲染（不着急，可能1~2年后）

函数/类对照表
| 名称 | 命名空间或所属类 | 参数类型 | 返回值 | 用途 |
|:---|:---|:---|:---|:---|
|Color|Core/Graphics|N/A|N/A|通过RGBA或HSLA创建颜色|
|toCOLORREF|Color|void|COLORREF|将本库的颜色结构体转化为COLORREF，注意这会失去alpha通道|
|FromCOLORREF|Color|COLORREF|Color|将COLORREF转化为本库结构体，alpha值为255|
|Canvas|Core/Graphics|N/A|N/A|本库的画布结构体，**目前为废弃状态**，通过std::vector存储，这里暂时将不再赘述结构体的方法|
|logger|Core/Graphics|N/A|N/A|日志输出结构体，负责进行日志的输出，可以绑定不同的std::ostream，默认为clog|
|traceLog|logger|LogLevel,std::string|void|日志打印纯文本字符串|
|varLog|logger|LogLevel,std::string,any|void|formatLog的快速书写版本，可以输出单个变量的值，string为变量名|
|formatLog|logger|LogLevel,std::string,any...|void|与printf()相同的格式化输出，除了LogLevel其余与printf用法相同|
|classIsRegistered|Utils|wchar_t const*|BOOL|返回窗口类名是否被注册|
