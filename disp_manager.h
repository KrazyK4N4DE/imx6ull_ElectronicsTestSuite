#ifndef __DISP_MANAGER_H__
#define __DISP_MANAGER_H__

// 块区域大小分辨率
typedef struct Region
{
    int leftUpX, leftUpY;
    int width, height;
}Region, * PRegion;

// 将显示管理器抽象出来的数据结构
typedef struct DisOpr
{
    char* name;  // 通过名字分辨不同的结构体(模块)
    char* GetBuffer(int* pXRes, int* pYRes, int* pBpp);  // 获得一个buffer，在上面绘制图片
    int FlushRegion(PRegion ptRegion, char* buffer);  // 把绘制好的区域刷出来
    struct DisOpr* ptNext;  // 刷到FrameBuffer或者web端都行，用一个链表式的指针链接起来好管理
}DisOpr, * PDisOpr;

#endif
