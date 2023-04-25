#ifndef __FONT_MANAGER_H__
#define __FONT_MANAGER_H__

#include <common.h>

/*抽象出数据结构来描述文字，支持固定点阵的字库和freetype字库*/
typedef struct FontBitMap
{
	Region tRegion;
	unsigned char* pucBuffer;	// 位图
	// 对于freetype，需要选定一个基点，由此可以知道下一个字符基点坐标
	int iCurOriginX;
	int iCurOriginY;
	int iNextOriginX;
	int iNextOriginY;
}FontBitMap, * PFontBitMap;
/*抽象出数据结构来描述字体文件的操作方法，获得字符对应的FontBitMap*/
typedef struct FontOpr
{
	char* name;
	int (*FontInit)(char* aFineName);  // aFineName为字体文件
	int (*SetFontSize)(int iFontSize);  // 设置字体大小
	int (*GetFontBitMap)(unsigned int dwCode, PFontBitMap ptFontBitMap);  // 根据需要的字符的ascii值，获得数据保存到ptFontBitMap中
	struct FontOpr* ptNext;  // 为支持多个字库文件，使用链表连接
}FontOpr, * PFontOpr;

void RegisterFont(PFontOpr ptFontOpr);
void FontsRegister();
int SelectAndInitFont(char* aFontOprName, char* aFineName);
int SetFontSize(int iFontSize);
int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap);

#endif
