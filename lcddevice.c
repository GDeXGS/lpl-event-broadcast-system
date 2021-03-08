#include <lcddevice.h>
#include <linux/fb.h>

#if 0
typedef  struct  LcdDevice{
	int fd  ;//文件描述符
	void *mptr ;//映射空间首地址
	int width, heigth;//宽,高
	int pixbyte;//单个像素占用的字节数
	unsigned int screen_size;//总占用字节数
} LcdDevice;
#endif


/*
	函数功能：
		初始化lcd设备，把lcd设备信息存储到LcdDevice类型结构体中
	参数：
		devname：设备文件路径
	返回值：
		成功返回LcdDevice类型结构体指针，失败返回NULL
*/
LcdDevice *lcd_init(const char *devname)
{
	//定义lcd结构体对象
	LcdDevice *lcd = (LcdDevice*)malloc(sizeof(LcdDevice));
	if(lcd == NULL)
	{
		perror("lcd init error");
		return NULL;
	}

	//打开lcd设备
	lcd->fd = open(devname, O_RDWR);
	if(lcd->fd < 0) 
	{
		perror("open error");
		free(lcd);
		return NULL;
	}

	//获取lcd设备信息
	struct fb_var_screeninfo info;
	int ret  = ioctl(lcd->fd, FBIOGET_VSCREENINFO, &info);
	
	lcd->width=info.xres_virtual; 
	lcd->heigth=info.yres_virtual;
	//lcd->width=info.xres; 
	//lcd->heigth=info.yres;
	lcd->pixbyte = info.bits_per_pixel/8;
	lcd->screen_size = lcd->width*lcd->heigth*lcd->pixbyte;
	
	//printf("x,y  %d,%d\n", info.xres, info.yres);
	//printf("x,y  %d,%d\n", info.xres_virtual, info.yres_virtual);
	
	

	//映射
	lcd->mptr = mmap(NULL, lcd->screen_size, 
	                 PROT_READ|PROT_WRITE, MAP_SHARED, 
	                 lcd->fd, 0);

	if(lcd->mptr == MAP_FAILED)
	{
		perror("映射失败");
		close(lcd->fd);
		free(lcd);
		return NULL;
	}
	
	return lcd;
}

/*
	函数功能：
		释放lcd结构体申请的内存
	参数：
		lcd：LcdDevice类型结构体指针
	返回值：
		成功返回ture，失败返回false
*/
bool lcd_destroy(struct LcdDevice* lcd)
{
	if(lcd == NULL)return true;
	if(lcd->mptr != MAP_FAILED) //先解除映射
	{
		munmap(lcd->mptr, lcd->screen_size);
	}
	if(lcd->fd > 0) //再关闭文件
	{
		close(lcd->fd);
	}
	free(lcd);//释放结构体空间
	return true;
}

/*
	函数功能：
		释放lcd结构体申请的内存
	参数：
		lcd：LcdDevice类型结构体指针
		x：绘制点x坐标
		y：绘制点y坐标
	返回值：
		
*/
void draw_point(struct LcdDevice *lcd,  int x, int y,  unsigned int color)
{
	//用ptr保存绘制的开始位置
	unsigned int *ptr =( (unsigned int*)lcd->mptr) + y*lcd->width + x;
	ptr[0] = color;
}

/*
	函数功能：
		清屏
	参数：
		lcd：LcdDevice类型结构体指针
		color：要刷的颜色
	返回值：
		
*/
void lcd_clear(struct LcdDevice *lcd, unsigned int color)
{
	int w = lcd->width;
	int h = lcd->heigth;
	unsigned int *ptr =(unsigned int*)lcd->mptr; 
	
	int i=0, j=0;
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			ptr[i*w+j] = color;
		}
	}
}















