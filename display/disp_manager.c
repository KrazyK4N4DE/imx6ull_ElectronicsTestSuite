#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <disp_manager.h>

/*
* 管理底层的LCD、WEB或其他显示方式
*/

static PDispOpr g_DispDevs = NULL;  // 显示方式头结点
static PDispOpr g_DispDefault = NULL;  // 默认显示
static DispBuff g_DispBuff;
static int line_width;  // 每行所有像素的大小
static int pixel_width;  // 每个像素的大小
// 添加显示方式结构体，注册进链表中
void RegisterDisplay(PDispOpr ptDispOpr)
{
	ptDispOpr->ptNext = g_DispDevs;
	g_DispDevs = ptDispOpr;
}
// 来调用各个显示方式的Init，如FramebufferInit、WebInit
void DisplayInit()
{
	FramebufferInit();
	// WebInit();
}
// 来选择链表中的显示方式
int SelectDefaultDisplay(char* name)
{
	PDispOpr tmp = g_DispDevs;
	while(tmp)
	{
		if(strcmp(tmp->name, name) == 0)
		{
			g_DispDefault = tmp;
			return 0;
		}
		tmp = tmp->ptNext;
	}
	return -1;
}
// 初始化默认的显示
int InitDefaultDisplay()
{
	int ret;
	ret = g_DispDefault->DeviceInit();
	if(ret)
	{
		printf("DeviceInit Fail!\n");
		return -1;
	}
	ret = g_DispDefault->GetBuffer(&g_DispBuff); 
	if(ret)
	{
		printf("GetBuffer Fail!\n");
		return -1;
	}
	line_width = g_DispBuff.iXres * g_DispBuff.iBpp / 8;
	pixel_width = g_DispBuff.iBpp / 8;
	return 0;
}
// 返回结构体
PDispBuff GetDisplayBuffer()
{
	return &g_DispBuff;
}


/*
* 通用函数
*/

// 绘制像素
int PutPixel(int x, int y, unsigned int dwColor)
{
	unsigned char *pen_8 = (unsigned char* )(g_DispBuff.buff + y * line_width + x * pixel_width);
	unsigned short *pen_16;	
	unsigned int *pen_32;	

	unsigned int red, green, blue;	

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int *)pen_8;

	switch (g_DispBuff.iBpp)
	{
		case 8:
		{
			*pen_8 = dwColor;
			break;
		}
		case 16:
		{
			/* 565 */
			red   = (dwColor >> 16) & 0xff;
			green = (dwColor >> 8) & 0xff;
			blue  = (dwColor >> 0) & 0xff;
			dwColor = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pen_16 = dwColor;
			break;
		}
		case 32:
		{
			*pen_32 = dwColor;
			break;
		}
		default:
		{
			printf("can't surport %dbpp\n", g_DispBuff.iBpp);
			return -1;
			break;
		}
	}
	return 0;
}

/*绘制文字位图*/
void DrawFontBitMap(PFontBitMap ptFontBitMap, unsigned int dwColor)
{
	int i, j, p, q;
	int x = ptFontBitMap->tRegion.leftUpX;
	int y = ptFontBitMap->tRegion.leftUpY;
    int x_max = x + ptFontBitMap->tRegion.width;
    int y_max = y + ptFontBitMap->tRegion.height;
	unsigned char* buffer = ptFontBitMap->pucBuffer;
	int width = ptFontBitMap->tRegion.width;

    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
        for ( i = x, p = 0; i < x_max; i++, p++ )
        {
            if (i < 0 || j < 0 || i >= g_DispBuff.iXres || j >= g_DispBuff.iYres) continue;
            if(buffer[q * width + p]) PutPixel(i, j, dwColor);
        }
    }
}

// 把绘制好的像素刷到硬件上去
int FlushDisplayRegion(PRegion ptRegion, PDispBuff ptDispBuff)
{
	return g_DispDefault->FlushRegion(ptRegion, ptDispBuff);
}
