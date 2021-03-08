#include <mainfunc.h>


/*
	函数功能：
		获取文件夹更新信息
	参数：
		file_path：文件路径
		dir_path：文件夹路径
	返回值：
		文件修改时间更新，返回1，没有更新返回0，出错返回-1
*/
int Getupdate(const char *file_path, const char *dir_path)
{
	if(file_path==NULL || dir_path==NULL)
		return;
	
	FILE *filestream;
	int ret;
	char buffer[512];
	char buffer2[512];
	char *fgets_ret;
	int flag = 1;
	int stat_ret;
	struct stat statbuf;
	
	//打开文件
	filestream = fopen(file_path, "r+");
	if(filestream == NULL)
	{
		perror("open file failed");
		return -1;
	}
	
	//读取文件第一行内容，（fgets读取数据带换行符）
	fgets_ret = fgets(buffer, sizeof(buffer),filestream);
	if(fgets_ret == NULL)
	{
		perror("fgets error");
		return -1;
	}
	printf("mtime:%s", buffer);

	//获取文件夹信息
	stat_ret = stat( dir_path, &statbuf );
	if( stat_ret == -1 )
	{
		perror("stat error");
		return -1;
	}
	
	//将最新修改时间写入文本
	sprintf(buffer2, "%s", ctime(&statbuf.st_mtime)); //buffer2以换行符结尾
	printf("文件最后修改时间：%s", buffer2);
	fseek(filestream,0,SEEK_SET);
	fwrite(buffer2, strlen(buffer2), 1, filestream);
	
	
	if(strcmp(buffer, buffer2) == 0)
		flag = 0;
	
	//关闭文件
	ret = fclose(filestream);
	if(ret == EOF)
	{
		perror("fclose error");
		return -1;
	}
	
	return flag;
}

/*
	函数功能：
		搜索文件夹中的bmp图片，获取路径存储在链表中,并且写入文件
	参数：
		head：链表头指针
		dir_path：文件夹路径
		bmptext：文本文件名
	返回值：
		
*/
void GetBmp(struct Link *head, const char *dir_path, char *bmptext )
{
	if(head==NULL || dir_path==NULL || bmptext==NULL)
		return;
	
	DIR *dirstream;
	int retval;
	
	struct dirent *file_info;
	char *pathname;
	FILE *filestream;
	int ret;
	
	//分配空间
	pathname = malloc(4096);
	if(pathname == NULL)
	{
		printf("分配空间失败");
		return;
	}
	
	//打开文件夹
	dirstream = opendir(dir_path);
	if(dirstream == NULL)
	{
			perror("打开目录失败");
			return;
	}
	
	//打开文件
	filestream = fopen(bmptext, "w+");
	if(filestream == NULL)
	{
		perror("open file failed");
		return;
	}

	int len = 4;
	char buffer[len+1];
	char buffer2[len+1];
	bzero(buffer, sizeof(buffer));
	bzero(buffer2, sizeof(buffer2));
	struct Link *pos;
	char *ptr,*ptr2;
	//循环读取
	while(1)
	{
		file_info = readdir(dirstream);
		if(file_info == NULL)
			break;
		
		if(file_info->d_type == DT_REG)
		{
			//找到首个'.'出现的位置
			ptr = strchr(file_info->d_name, '.');	
			if(ptr == NULL)
				continue;
			
			//判断文件名后缀是否为.bmp
			if(strcmp(ptr, ".bmp") == 0)
			{			
				//是则新建节点,组合路径
				struct Link *node = new_node();
				sprintf(pathname, "%s/%s", dir_path, file_info->d_name);
				
				//分配路径存储空间
				node->pname = malloc(strlen(pathname)+1);
				
				//写入pname
				memcpy(node->pname, pathname, strlen(pathname)+1);
				
				//找到合适的插入位置位置
				memcpy(buffer, ptr-4, len);
				for(pos=head->next; pos!=head; pos=pos->next)
				{
					ptr2 = strchr(pos->pname, '.');
					memcpy(buffer2, ptr2-4, len);
					if( strcmp(buffer2, buffer)>0 )
						break;
				}
				
				//插入链表
				head_insert(pos, node);
				
				//printf("第N个节点 %s\n", node->pname);
				//写入文件
				//fprintf(filestream, "%s\n", pathname);
			}
		}
	
	}
	
	//写入文件
	for(pos=head->next; pos!=head; pos=pos->next)
	{
		fprintf(filestream, "%s\n", pos->pname);
		printf("顺序 %s\n", pos->pname);
	}
	
	//关闭文件夹
	retval = closedir(dirstream);
	if(retval == -1)
	{
		perror("关闭目录失败");
		return;
	}
	
	//关闭文件
	ret = fclose(filestream);
	if(ret == EOF)
	{
		perror("fclose error");
		return;
	}
	
	//释放空间
	free(pathname);
}

/*
	函数功能：
		从文本中获取bmp图片路径，存储在链表中
	参数：
		head：链表头指针
		bmptext：文本文件名
	返回值：
		
*/
void Getbmppath(struct Link *head, const char *bmptext)
{
	if(head==NULL || bmptext==NULL)
		return;
	
	FILE *filestream;
	int ret;
	char buffer[512];
	char *fgets_ret;
	
	//打开文件
	filestream = fopen(bmptext, "r");
	if(filestream == NULL)
	{
		perror("open file failed");
		return;
	}
	
	while(1)
	{
		//读取文件内容
		fgets_ret = fgets(buffer, sizeof(buffer),filestream);
		if(fgets_ret == NULL)
		{
			break;
		}
		buffer[strlen(buffer)-1]=0;
		
		//新建节点
		struct Link *node = new_node();
		//分配路径存储空间
		node->pname = malloc(strlen(buffer)+1);
		//写入pname
		memcpy(node->pname, buffer, strlen(buffer)+1);
		printf("bmppath %s\n", node->pname);
		//插入链表
		head_insert(head, node);
	}
	
	//关闭文件
	ret = fclose(filestream);
	if(ret == EOF)
	{
		perror("fclose error");
		return;
	}
}

/*
	函数功能：
		从文件中获取数据，并存储在text中
	参数：
		text：存储文本的字符串地址
		file_path：文件名
	返回值：
		
*/
void GetString(char *text, const char *file_path)
{
	if(text==NULL || file_path==NULL)
		return;
	
	FILE *filestream;
	int ret;
	char buffer[512]={"a"};
	char *fgets_ret;
	int len = 0;
	char dest[1024];
	
	//打开文件
	filestream = fopen(file_path, "r+");
	if(filestream == NULL)
	{
		perror("open file failed");
		return;
	}
	
	while(1)
	{
		//读取文件内容
		fgets_ret = fgets(buffer, sizeof(buffer),filestream);
		if(fgets_ret == NULL)
		{
			if(strcmp(buffer, "a") == 0)
			{
				printf("fgets error\n");
			}
			break;
		}
		
		//写入text
		buffer[strlen(buffer)-1]=0; //去掉换行符
		sprintf(dest, "%s%s", buffer, "      ");//增加空格
		memcpy(text+len, dest, strlen(dest));//写入
		len += strlen(dest);//记录长度
	}
	
	//关闭文件
	ret = fclose(filestream);
	if(ret == EOF)
	{
		perror("fclose error");
		return;
	}
}

/*
	函数功能：
		获取时间日期信息，并存储在str_time和str_date中
	参数：
		str_time：存储时间的字符串地址
		str_date：存储日期的字符串地址
	返回值：
		
*/
void Gettime(char *str_time, char *str_date)
{
	time_t tt;
	time(&tt);
	
	struct tm *time_test = gmtime(&tt);
	
	sprintf(str_time,"%02d:%02d:%02d", time_test->tm_hour, time_test->tm_min, time_test->tm_sec);
	sprintf(str_date,"%4d/%02d/%02d", time_test->tm_year+1900, time_test->tm_mon+1, time_test->tm_mday);
	
}

/*
	函数功能：
		获取显示配置信息，存储到结构体中
	参数：
		p:显示配置结构体指针
		configtext：配置信息文本文件路径
	返回值：
		
*/
void Getshowconfig(struct showconfig *p, const char *configtext)
{
	if(p==NULL || configtext==NULL)
		return;
	
	FILE *filestream;
	int ret;
	int fscanf_ret;
	char buffer[512];
	char *fgets_ret;

	
	//打开文件
	filestream = fopen(configtext, "r");
	if(filestream == NULL)
	{
		perror("open file failed");
		return;
	}
	
	int i,j;
	for(i=0; i<5; i++)
	{
		for(j=0; j<7; j++)
		{
			fscanf_ret = fscanf(filestream, "%d", (int *)&p[i]+j);
			if(fscanf_ret == EOF)
			{
				break;
			}
			//printf("%d %d %d\n", i, j, *( (int *)&p[i]+j ));
			//sleep(1);
		}
	}
	
	//关闭文件
	ret = fclose(filestream);
	if(ret == EOF)
	{
		perror("fclose error");
		return;
	}
}

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
void ShowPictrue(LcdDevice *lcd, const char *picname, const struct showconfig *p)
{
	if(lcd==NULL || picname == NULL || p==NULL)
		return;
	
	//打开bmp文件
	printf("%s\n", picname);
	struct BmpFile *bmp = create_bmp(picname);
	if(bmp == NULL)
	{
		printf("bmp open failed\n");
		return;
	}
	
	//改变bmp图片大小为指定宽高
	bmp = change_size(bmp, p->w, p->h);
	
	//刷到lcd指定区域
	unsigned int *ptr = (unsigned int*)lcd->mptr+p->y*lcd->width+p->x;
	int i=0, j=0;
	for(i=0; i<bmp->height; i++)
	{
		for(j=0; j<bmp->width; j++)
		{
			memcpy(ptr+i*lcd->width+j, bmp->bitmap+(i*bmp->width+j)*3, 3);
		}
	}
	
	//关闭bmp文件
	destroy_bmp(bmp);
}

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
void ShowRollText(LcdDevice *lcd, font *f, char *text, const char*text_pos, const struct showconfig *p)
{
	if(lcd==NULL || f == NULL || text==NULL || text_pos==NULL || p==NULL)
		return;
	
	//读取text的N个字符到buffer
	char buffer[52];
	bzero(buffer, sizeof(buffer));
	if(strlen(text_pos) >= sizeof(buffer))
		memcpy(buffer, text_pos, sizeof(buffer));
	else
	{
		memcpy(buffer, text_pos, strlen(text_pos));
		memcpy(buffer+strlen(text_pos), text, sizeof(buffer)-strlen(text_pos));
	}
	
	//创建一个绘制汉字区域
	bitmap *bm = createBitmapWithInit(p->w, p->h, 4, 0xffffffff);
	
	//设置字体大小
	fontSetSize(f, p->font_size);
	
	//把汉字打印到bitmap区域中
	fontPrint(f, bm, p->font_x, p->font_y, buffer, 0x00000000, 0);

	//把bm区域刷到lcd上显示
	unsigned int *ptr = (unsigned int *)lcd->mptr+p->y*lcd->width+p->x;
	int i=0, j=0;
	for(i=0; i<bm->height; i++)
	{
		for(j=0; j<bm->width; j++)
		{
			memcpy(ptr+j+i*lcd->width, bm->map+(bm->width*i+j)*4, 4);
		}
	}
	
	//摧毁文字区域
	destroyBitmap(bm);
}

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
void ShowRollText2(LcdDevice *lcd, font *f, char *text, int num, int offset, const struct showconfig *p)
{
	if(lcd==NULL || f == NULL || text==NULL || p==NULL)
		return;
	
	//创建一个绘制汉字区域
	bitmap *bm = createBitmapWithInit(p->w, p->h, 4, 0xffffffff);
	
	//设置字体大小
	fontSetSize(f, p->font_size);
	
	//偏移offset量，把汉字打印到bitmap区域中
	fontPrint(f, bm, p->font_x - offset, p->font_y, text, 0x00000000, 0);
	fontPrint(f, bm, num*50 - offset, p->font_y, text, 0x00000000, 0);

	//把bm区域刷到lcd上显示
	unsigned int *ptr = (unsigned int *)lcd->mptr+p->y*lcd->width+p->x;
	int i=0, j=0;
	for(i=0; i<bm->height; i++)
	{
		for(j=0; j<bm->width; j++)
		{
			memcpy(ptr+j+i*lcd->width, bm->map+(bm->width*i+j)*4, 4);
		}
	}
	
	//摧毁文字区域
	destroyBitmap(bm);
}

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
void Show(LcdDevice *lcd, font *f, char *str, const struct showconfig *p)
{
	if(lcd==NULL || f == NULL || str==NULL || p==NULL)
		return;
	
	//创建一个绘制汉字区域
	bitmap *bm = createBitmapWithInit(p->w, p->h, 4, 0xffffffff);
	
	//设置字体大小
	fontSetSize(f, p->font_size);
	
	//把汉字打印到bitmap区域中
	fontPrint(f, bm, p->font_x, p->font_y, str, 0x00000000, 0);

	//把bm区域刷到lcd上显示
	unsigned int *ptr = (unsigned int *)lcd->mptr+p->y*lcd->width+p->x;
	int i=0, j=0;
	for(i=0; i<bm->height; i++)
	{
		for(j=0; j<bm->width; j++)
		{
			memcpy(ptr+j+i*lcd->width, bm->map+(bm->width*i+j)*4, 4);
		}
	}
	
	//摧毁文字区域
	destroyBitmap(bm);
}

//显示时间
void ShowTime(LcdDevice *lcd, font *f, char *str_time, int x, int y)
{
	if(lcd==NULL || f == NULL || str_time==NULL)
		return;
	
	//创建一个绘制汉字区域
	bitmap *bm = createBitmapWithInit(200, 60, 4, 0xffffffff);
	
	//设置字体大小
	fontSetSize(f, 30);
	
	//把汉字打印到bitmap区域中
	fontPrint(f, bm, 50, 15, str_time, 0x00000000, 0);

	//把bm区域刷到lcd上显示
	unsigned int *ptr = (unsigned int *)lcd->mptr+y*lcd->width+x;
	int i=0, j=0;
	for(i=0; i<bm->height; i++)
	{
		for(j=0; j<bm->width; j++)
		{
			memcpy(ptr+j+i*lcd->width, bm->map+(bm->width*i+j)*4, 4);
		}
	}
	
	//摧毁文字区域
	destroyBitmap(bm);
}

//显示日期
void ShowDate(LcdDevice *lcd, font *f, char *str_date, int x, int y)
{
	if(lcd==NULL || f == NULL || str_date==NULL)
		return;
	
	//创建一个绘制汉字区域
	bitmap *bm = createBitmapWithInit(200, 60, 4, 0xffffffff);
	
	//设置字体大小
	fontSetSize(f, 30);
	
	//把汉字打印到bitmap区域中
	fontPrint(f, bm, 35, 15, str_date, 0x00000000, 0);

	//把bm区域刷到lcd上显示
	unsigned int *ptr = (unsigned int *)lcd->mptr+y*lcd->width+x;
	int i=0, j=0;
	for(i=0; i<bm->height; i++)
	{
		for(j=0; j<bm->width; j++)
		{
			memcpy(ptr+j+i*lcd->width, bm->map+(bm->width*i+j)*4, 4);
		}
	}
	
	//摧毁文字区域
	destroyBitmap(bm);
}



























