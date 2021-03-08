项目名称：
LPL赛事播报系统

项目实现功能：
在lcd屏上循环显示bmp文件夹内的bmp图片，滚动显示text.txt中的文本内容，自动获取并显示当前系统时间日期。

使用说明：
可配置showconfig.txt文件调节各部分显示位置及尺寸，可修改text.txt播报最新消息，将需要播放的图片准备好放入bmp文件夹内，运行程序即可。


库函数：
simfang.ttf font.c font.h truetype.h truetype.c 
lcddevice.c lcddevice.h bmp_list.c bmp_list.h 
bmp.c bmp.h
主函数功能实现：
mainfunc.c mainfunc.h
主函数：
main.c
编译管理文件：
Makefile
资源文件：
bmp bmptext.txt mtime.txt text.txt showconfig.txt

说明：
粤嵌gec6818，板子屏幕为800*480

关于showconfig.txt的配置，每行配置信息对应，依次为：
开机界面图片显示配置
主界面图片显示配置
滚动显示消息配置
时间显示配置
日期显示配置

每行配置参数依次为：
起点横向坐标 起点纵向坐标 宽 高 文字打印横向坐标 文字打印纵向坐标 文字字体大小

对应范围：
[0,800]  [0,480] [0,800] [0,480] [0,宽] [0,高] [自行决定]

前面四个参数决定了lcd上显示区域大小，后面3个参数决定文字在显示区域内打印的位置及打印的大小，可以知道对显示图片而言，后面3个参数无效，给予0即可。