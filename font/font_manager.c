#include <font_manager.h>
#include <string.h>

static PFontOpr g_ptFonts= NULL;
static PFontOpr g_ptDefaultFonts= NULL;

/*注册单个字库*/
void RegisterFont(PFontOpr ptFontOpr)
{
	ptFontOpr->ptNext = g_ptFonts;
	g_ptFonts = ptFontOpr;
}
/*字库注册函数*/
void FontsRegister()
{
	extern void FreetypeRegister();
	FreetypeRegister();
}
/*选择想要使用的FontOpr并初始化*/
int SelectAndInitFont(char* aFontOprName, char* aFineName)
{
	PFontOpr tmp = g_ptFonts;
	while(tmp)
	{
		if(strcmp(tmp->name, aFontOprName) == 0) break;
		tmp = tmp->ptNext;
	}
	if(!tmp) return -1;
	g_ptDefaultFonts = tmp;
	return tmp->FontInit(aFineName);  // 返回顺便初始化
}
/*通用的设置字体大小*/
int SetFontSize(int iFontSize)
{
	return g_ptDefaultFonts->SetFontSize(iFontSize);
}
/*通用的获取FontBitMap*/
int GetFontBitMap(unsigned int dwCode, PFontBitMap ptFontBitMap)
{
	return g_ptDefaultFonts->GetFontBitMap(dwCode, ptFontBitMap);
}