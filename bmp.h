#ifndef _BMP_H_
#define _BMP_H_


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#pragma pack(1)  //设置1字节对齐
struct BmpHeader
{
	unsigned short bfType;//文件类型BM
	unsigned int bfSize;//文件大小
	unsigned int bfReserved;//保留设置为0
	unsigned int bfoffBits;//偏移

	unsigned int biSize;
	unsigned int biWidth;
	unsigned int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	unsigned int biX;
	unsigned int biY;
	unsigned int biClrUsed;
	unsigned int biCrlImportant;
};
#pragma pack(0)

struct BmpFile{
	int width;//宽
	int height;//高
	int pixbyte;//一个像素占用字节数
	unsigned char *bitmap;
};

/*
	函数功能：
		打开bmp文件，获取bmp文件数据存储在结构体struct BmpFile里
	参数：
		bmpfile：bmp文件路径
	返回值：
		成功返回struct BmpFile类型结构体指针，失败返回NULL
*/
struct BmpFile *create_bmp(const char *bmpfile);
/*
	函数功能：
		释放bmp申请的堆内存
	参数：
		bmp：struct BmpFile类型结构体指针
	返回值：
		成功返回true，失败返回false
*/
bool destroy_bmp(struct BmpFile *bmp);
/*
	函数功能：
		把图片转换为destw宽，desth高的图片
	参数：
		bmp：struct BmpFile类型结构体指针
		destw：需要转换成的宽
		desth：需要转换成的高
	返回值：
		成功返回struct BmpFile类型结构体指针，失败返回NULL
*/
struct BmpFile *change_size(struct BmpFile *bmp , int destw, int desth);

#endif//_BMP_H_

























