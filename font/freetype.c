#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <wchar.h>
#include <sys/ioctl.h>
#include <font_manager.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

static FT_Face g_face;
static int g_iDefaultFontSize = 12;

static int FreetypeFontInit(char* aFineName)
{
	FT_Library library;
    int error;
	error = FT_Init_FreeType(&library);  // 初始化freetype字库
	if(error)
	{
		printf("FT_Init_FreeType error\n");
		return -1;
	}
    error = (library, aFineName, 0, &g_face);  /* create face object */
	if(error)
	{
		printf("FT_New_Face error\n");
		return -1;
	}
	FT_Set_Pixel_Sizes(g_face, g_iDefaultFontSize, 0);  // 设置默认字体大小
	return 0;
}
static int FreetypeSetFontSize(int iFontSize)
{
	FT_Set_Pixel_Sizes(g_face, iFontSize, 0);  // 设置字体大小
	return 0;
}
static int FreetypeGetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap)
{
	int error;
	FT_Vector pen;  // pen就是freetype中的origin点
    FT_Glyph  glyph;
    FT_GlyphSlot slot = g_face->glyph;

	// 给基准点坐标赋值
	pen.x = ptFontBitMap->iCurOriginX * 64;  // 单位: 1/64像素
    pen.y = ptFontBitMap->iCurOriginY * 64;  // 单位: 1/64像素
	// 转换：transformation
	FT_Set_Transform(g_face, 0, &pen);
	// 加载位图: load glyph image into the slot (erase previous one)
    error = FT_Load_Char(g_face, dwCode, FT_LOAD_RENDER);
    if (error)
    {
        printf("FT_Load_Char error\n");
        return -1;
    }

	ptFontBitMap->pucBuffer = slot->bitmap.buffer;  // 获得点阵
	ptFontBitMap->iLeftUpX = slot->bitmap_left;
	ptFontBitMap->iLeftUpY = 2 * ptFontBitMap->iCurOriginY -  slot->bitmap_top;  // 笛卡尔坐标到LCD坐标的转换
	ptFontBitMap->iWidth = slot->bitmap.width;
	ptFontBitMap->iRows = slot->bitmap.rows;
	ptFontBitMap->iCNextOriginX = ptFontBitMap->iCurOriginX + slot->advance.x / 64;
	ptFontBitMap->iCNexyOriginY = ptFontBitMap->iCurOriginY + slot->advance.y / 64;

	return 0;
}

static FontOpr g_tFreetypeOpr = 
{
	.name = "Freetype",
	.FontInit = FreetypeFontInit,
	.SetFontSize = FreetypeSetFontSize,
	.GetFontBitMap = FreetypeGetFontBitMap
};


