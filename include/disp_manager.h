#ifndef __DISP_MANAGER_H__
#define __DISP_MANAGER_H__

#include <common.h>
#include <font_manager.h>

#define BACKGROUND_COLOR 0xff0000  // 背景颜色

// 用于存放buffer的分辨率信息
typedef struct DispBuff
{
	int iXres;
	int iYres;
	int iBpp;
	char* buff;
}DispBuff, * PDispBuff;

// 将显示管理器抽象出来的数据结构
typedef struct DispOpr
{
    char* name;  // 通过名字分辨不同的结构体(模块)
    int (*GetBuffer)(PDispBuff ptDispBuff);  // 获得一个buffer，在上面绘制图片，返回0表示get成功
    int (*FlushRegion)(PRegion ptRegion, PDispBuff ptDispBuff);  // 把绘制好的区域刷出来
    struct DispOpr* ptNext;  // 刷到FrameBuffer或者web端都行，用一个链表式的指针链接起来好管理
    int (*DeviceInit)(void);
	int (*DeviceExit)(void);
}DispOpr, * PDispOpr;

void DisplaySystemRegister(void);
void FramebufferInit(void);
void RegisterDisplay(PDispOpr ptDispOpr);
int SelectDefaultDisplay(char* name);
void LCD_Clear(PDispBuff ptDispBuff, unsigned char color);
int InitDefaultDisplay(void);
PDispBuff GetDisplayBuffer(void);
int PutPixel(int x, int y, unsigned int dwColor);
void DrawFontBitMap(PFontBitMap ptFontBitMap, unsigned int dwColor);
void DrawRegion(PRegion ptRegion, unsigned int dwColor);
void DrawTextInRegionCentral(char* name, PRegion ptRegion, unsigned int dwColor);
int FlushDisplayRegion(PRegion ptRegion, PDispBuff ptDispBuff);

#endif
