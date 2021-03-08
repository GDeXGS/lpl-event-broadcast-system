#ifndef _LCDDEDVICE_H_
#define _LCDDEDVICE_H_ 

#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

typedef  struct  LcdDevice{
	int fd  ;//文件描述符
	void  *mptr ;//映射空间首地址
	int width, heigth;//宽,高
	int pixbyte;//单个像素占用的字节数
	unsigned int screen_size;
} LcdDevice;

/*
	函数功能：
		初始化lcd设备，把lcd设备信息存储到LcdDevice类型结构体中
	参数：
		devname：设备文件路径
	返回值：
		成功返回LcdDevice类型结构体指针，失败返回NULL
*/
LcdDevice *lcd_init(const char *devname);

/*
	函数功能：
		释放lcd结构体申请的内存
	参数：
		lcd：LcdDevice类型结构体指针
	返回值：
		成功返回ture，失败返回false
*/
bool lcd_destroy(struct LcdDevice* lcd);

/*
	函数功能：
		释放lcd结构体申请的内存
	参数：
		lcd：LcdDevice类型结构体指针
		x：绘制点x坐标
		y：绘制点y坐标
	返回值：
		
*/
void  draw_point(struct LcdDevice *lcd,  int x, int y,  unsigned int color);

/*
	函数功能：
		清屏
	参数：
		lcd：LcdDevice类型结构体指针
		color：要刷的颜色
	返回值：
		
*/
void lcd_clear(struct LcdDevice *lcd, unsigned int color);

#endif//_LCDDEDVICE_H_ 
