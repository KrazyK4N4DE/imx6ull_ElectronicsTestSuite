#ifndef __FONT_MANAGER_H__
#define __FONT_MANAGER_H__

#ifndef NULL
#define NULL (void* )0;
#endif

/*抽象出数据结构来描述文字，支持固定点阵的字库和freetype字库*/
typedef struct FontBitMap
{
	int iLeftUpX;				// 描述位置的左上角X坐标
	int iLeftUpY;				// 描述位置的左上角Y坐标
	int iWidth;					// 有多宽
	int iRows;					// 有多少行(多高)
	unsigned char* pucBuffer;	// 位图
	// 对于freetype，需要选定一个基点，由此可以知道下一个字符基点坐标
	int iCurOriginX;
	int iCurOriginY;
	int iCNextOriginX;
	int iCNexyOriginY;
}FontBitMap, * PFontBitMap;
/*抽象出数据结构来描述字体文件的操作方法，获得字符对应的FontBitMap*/
typedef struct FontOpr
{
	char* name;
	int (*FontInit)(char* aFineName);  // aFineName为字体文件
	int (*SetFontSize)(int iFontSize);
	int (*GetFontBitMap)(unsigned int dwCode, PFontBitMap ptFontBitMap);  // 根据需要的字符的ascii值，获得数据保存到ptFontBitMap中
	struct FontOpr* ptNext;  // 为支持多个字库文件，使用链表连接
}FontOpr, * PFontOpr;

#endif
