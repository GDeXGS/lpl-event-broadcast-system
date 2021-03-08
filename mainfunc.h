#ifndef _MAINFUNC_H_
#define _MAINFUNC_H_

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <strings.h>
#include <unistd.h>

#include <bmp_list.h>
#include <lcddevice.h>
#include <bmp.h>
#include <font.h>

//显示配置结构体
struct showconfig
{
	int x;
	int y;
	int w;
	int h;
	int font_x;
	int font_y;
	int font_size;
};

/*
	函数功能：
		获取文件夹更新信息
	参数：
		file_path：文件路径
		dir_path：文件夹路径
	返回值：
		文件修改时间更新，返回1，没有更新返回0，出错返回-1
*/
int Getupdate(const char *file_path, const char *dir_path);
/*
	函数功能：
		搜索文件夹中的bmp图片，获取路径存储在链表中,并且写入文件
	参数：
		head：链表头指针
		dir_path：文件夹路径
		bmptext：文本文件名
	返回值：
		
*/
void GetBmp(struct Link *head, const char *dir_path, char *bmptext);
/*
	函数功能：
		从文本中获取bmp图片路径，存储在链表中
	参数：
		head：链表头指针
		bmptext：文本文件名
	返回值：
		
*/
void Getbmppath(struct Link *head, const char *bmptext);
/*
	函数功能：
		从文件中获取数据，并存储在text中（用于获取滚动显示的文本）
	参数：
		text：存储文本的字符串地址
		file_path：文件名
	返回值：
		
*/
void GetString(char *text, const char *file_path);
/*
	函数功能：
		获取时间日期信息，并存储在str_time和str_date中
	参数：
		str_time：存储时间的字符串地址
		str_date：存储日期的字符串地址
	返回值：
		
*/
void Gettime(char *str_time, char *str_date);
/*
	函数功能：
		获取显示配置信息，存储到结构体中
	参数：
		p:显示配置结构体指针
		configtext：配置信息文本文件路径
	返回值：
		
*/
void Getshowconfig(struct showconfig *p, const char *configtext);
/*
	函数功能：
		在lcd上显示滚动显示消息（每次移动3个字节，一个中文字符，三个英文字符）
	参数：
		lcd：lcd设备结构体指针
		f：字体库指针
		text：要显示的字符串指针
		text_pos：记录字符串显示位置
		p：显示配置结构体指针
	返回值：
		
*/
void ShowRollText(LcdDevice *lcd, font *f, char *text, const char*text_pos, const struct showconfig *p);
/*
	函数功能：
		在lcd上显示滚动显示消息（每次固定像素）
	参数：
		lcd：lcd设备结构体指针
		f：字体库指针
		num：字符串中中文字符数
		offset：打印到bitmap区域的偏移量
		p：显示配置结构体指针
	返回值：
		
*/
void ShowRollText2(LcdDevice *lcd, font *f, char *text, int num, int offset, const struct showconfig *p);

/*
	函数功能：
		在lcd上显示bmp图片
	参数：
		lcd：lcd设备结构体指针
		picname：图片路径
		x、y：起点坐标
		w、h：图片显示的宽、高
	返回值：
		
*/
void ShowPictrue(LcdDevice *lcd, const char *picname, const struct showconfig *p);
/*
	函数功能：
		在lcd上显示str（用于显示时间和日期）
	参数：
		lcd：lcd设备结构体指针
		f：字体库指针
		str：要显示的字符串
		p：显示配置结构体指针
	返回值：
		
*/
void Show(LcdDevice *lcd, font *f, char *str, const struct showconfig *p);
//显示时间
void ShowTime(LcdDevice *lcd, font *f, char *str_time, int x, int y);
//显示日期
void ShowDate(LcdDevice *lcd, font *f, char *str_date, int x, int y);





#endif