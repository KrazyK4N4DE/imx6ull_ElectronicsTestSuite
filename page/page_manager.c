#include <common.h>  // 用到NULL
#include <page_manager.h>
#include <string.h>

static PPageAction g_ptPages = NULL;

/*注册单个页面*/
void RegisterPage(PPageAction ptPageAction)
{
	ptPageAction->ptNext = g_ptPages;
	g_ptPages = ptPageAction;
}

/*注册多个页面*/
void PageSystemRegister(void)
{
	extern void MainPageRegister(void);
	MainPageRegister();
}

/*获取某个页面*/
PPageAction Page(char* name)
{
	PPageAction tmp = g_ptPages;
	while(tmp)
	{
		if(strcmp(tmp->name, name) == 0) return tmp;
		tmp = tmp->ptNext;
	}
	return NULL;
}
