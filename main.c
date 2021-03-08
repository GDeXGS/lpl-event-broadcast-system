#include <mainfunc.h>

int main()
{
	//初始化lcd
	LcdDevice *lcd = lcd_init("/dev/fb0");
	
	//初始化字库
	font *f = fontLoad("./simfang.ttf");
	if(f == NULL)
	{
		perror("font loaderror");
		return 0;
	}
	
	//初始化bmp链表
	struct Link *head = new_node();
	
	//判断文件修改时间是否变动，变动返回true
	if( Getupdate("mtime.txt", "bmp") == 1)
	{
		printf("search dir\n");
		//搜索文件夹获取图片路径
		GetBmp(head, "bmp", "bmptext.txt");
	}
	else
	{
		printf("get path from text\n");
		//从文本中获取图片路径
		Getbmppath(head, "bmptext.txt");
	}
	//sort(head);
	
	//获取消息文本
	char *text;
	text = malloc(4096*100); // 分配空间
	bzero(text, sizeof(text));
	GetString(text, "./text.txt");
	
	//获取显示配置信息
	struct showconfig p[5]={0};
	Getshowconfig(p, "showconfig.txt");
	
	//清屏
	lcd_clear(lcd, 0xffffffff);
	
	struct Link *pos = head->next;
	//char *text_pos = text;
	struct showconfig con_start=p[0];//{0, 0, 800, 480, 0, 0, 0};

	//开机界面
	while(1)
	{
		//printf("%s\n", pos->pname);
		ShowPictrue(lcd, pos->pname, &con_start);
		sleep(1);
		pos=pos->next;
		if(pos == head)
		{
			pos=pos->next;
			break;
		}
	}
	
	//清屏
	lcd_clear(lcd, 0xffffffff);
	
	int i=5;
	int offset=0;
	int num = strlen(text)/3 + 10; //有英文字符干扰，所以加了个10
	char str_time[32]={0};
	char str_date[32]={0};
	struct showconfig con_time=p[3];//{600, 0, 200, 180, 50, 115, 30};
	struct showconfig con_date=p[4];//{600, 180, 200, 180, 35, 35, 30};
	struct showconfig con_text=p[2];//{0, 360, 800, 120, 0, 45, 50};
	struct showconfig con_bmp=p[1];//{0, 0, 600, 360, 0, 0, 0};
	
	//主界面
	while(1)
	{
		ShowPictrue(lcd, pos->pname, &con_bmp);
		pos=pos->next;
		if(pos == head)
			pos=pos->next;
		
		for(i=33; i>0; i--)
		{	
			//ShowRollText(lcd, f, text, text_pos, &con_text);
			//text_pos += 3;
			//if( strlen(text_pos) < 3 )
			//	text_pos = text;
			
			ShowRollText2(lcd, f, text, num, offset, &con_text);
			offset += 3;
			if(offset > num*50)
				offset = 0;
			
			Gettime(str_time, str_date);
			//ShowTime(lcd, f2, str_time, 600, 100);
			Show(lcd, f, str_time, &con_time);
			//ShowDate(lcd, f2, str_date, 600, 200);
			Show(lcd, f, str_date, &con_date);
			
		}
	}

	
	//关闭lcd
	lcd_destroy(lcd);
	//关闭字库
	fontUnload(f);
	//摧毁链表
	list_destroy(head);
	//释放空间
	free(text);
	
	return 0;
}






