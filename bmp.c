#include "bmp.h"


/*
	函数功能：
		打开bmp文件，获取bmp文件数据存储在结构体struct BmpFile里
	参数：
		bmpfile：bmp文件路径
	返回值：
		成功返回struct BmpFile类型结构体指针，失败返回NULL
*/
struct BmpFile *create_bmp(const char *bmpfile)
{
	// 打开文件
	FILE *file = fopen(bmpfile, "r");
	if(file == NULL)
	{
		perror("fopen error");
		return NULL;
	}
	
	// 读取文件头
	struct BmpHeader header;
	ssize_t ret = fread(&header, 54, 1, file);
	if(ret < 0)
	{
		perror("fread error");
		fclose(file);
		return NULL;
	}
	
	// 申请结构体空间
	struct BmpFile *bmp = malloc(sizeof(struct BmpFile));
	if(bmp == NULL)
	{
		perror("bmp malloc error");
		fclose(file);
		return NULL;
	}
	
	// 初始化宽，高
	bmp->width = header.biWidth;
	bmp->height = header.biHeight;
	bmp->pixbyte = header.biBitCount/8;
	
	// 申请像素存储空间
	bmp->bitmap = (unsigned char *)malloc(bmp->width*bmp->height*bmp->pixbyte);
	
	// 从文件中读取像素
	int i=0;
	unsigned int pix_row_size = bmp->width * bmp->pixbyte;
	int rowsize = pix_row_size;
	if(rowsize%4 != 0) // 判断一行像素占用的字节数是否是4的倍数
	{
		rowsize += (4-rowsize%4);
	}
	// 创建一个缓冲区存储一行数据
	unsigned char rowbuffer[rowsize];
	int j=bmp->height-1;
	for(i=0; i<bmp->height; i++,j--)
	{
		fread(rowbuffer, rowsize, 1, file);
		memcpy(bmp->bitmap+pix_row_size*j, rowbuffer, pix_row_size);
	}
	
	
	fclose(file);
	return bmp;
}

/*
	函数功能：
		释放bmp申请的堆内存
	参数：
		bmp：struct BmpFile类型结构体指针
	返回值：
		成功返回true，失败返回false
*/
bool destroy_bmp(struct BmpFile *bmp)
{
	if(bmp == NULL) return true;
	free(bmp->bitmap); //先释放像素存储空间
	free(bmp); // 再释放结构体空间
}

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
struct BmpFile *change_size(struct BmpFile *bmp, int destw, int desth)
{
	// 申请结构体空间
	struct BmpFile *dest_bmp = malloc(sizeof(struct BmpFile));
	if(dest_bmp == NULL)
	{
		perror("bmp malloc error");
		return NULL;
	}
	
	// 初始化宽，高
	dest_bmp->width = destw;
	dest_bmp->height = desth;
	dest_bmp->pixbyte = bmp->pixbyte;
	
	// 申请像素存储空间
	dest_bmp->bitmap = (unsigned char *)malloc(dest_bmp->width*dest_bmp->height*dest_bmp->pixbyte);
	
	
	int i=0, j=0;
	int src_x=0, src_y=0;
	int dest_x=0, dest_y=0;
	// 以一个像素点为单位进行缩放
	for(i=0; i<desth;i++)
	{
		for(j=0; j<destw; j++)
		{
			dest_x = j; dest_y = i;
			src_x = dest_x * bmp->width / destw;
			src_y = dest_y * bmp->height / desth;
			
			*(dest_bmp->bitmap+(dest_y*destw+dest_x)*3) = *(bmp->bitmap+(src_y*bmp->width+src_x)*3);
			*(dest_bmp->bitmap+(dest_y*destw+dest_x)*3+1) = *(bmp->bitmap+(src_y*bmp->width+src_x)*3+1);
			*(dest_bmp->bitmap+(dest_y*destw+dest_x)*3+2) = *(bmp->bitmap+(src_y*bmp->width+src_x)*3+2);
		}
	}
	
	destroy_bmp(bmp);
	return dest_bmp;
}














