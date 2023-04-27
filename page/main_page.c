#include <page_manager.h>
#include <stdio.h>

static void MainPageRun(void* pParams)
{
	printf("FileName: %s\nFuncName: %s\nLine: %d\n", __FILE__, __FUNCTION__, __LINE__);  // 打印文件名、函数名、当前行数
}

static PageAction g_tMainPage = 
{
	.name = "main",
	.Run = MainPageRun
};

void MainPageRegister()
{
	RegisterPage(&g_tMainPage);
}
