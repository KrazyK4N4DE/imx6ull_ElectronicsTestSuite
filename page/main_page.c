#include <page_manager.h>
#include <config.h>
#include <stdio.h>
#include <math.h>

#define X_GAP 5
#define Y_GAP 5

static Button g_tButtons[ITEMCFG_MAX_NUM];

static void GenerateButtons()
{
	int width, height;
	int n_per_line;  // 每行有多少个按钮
	int n, xRes, yRes;
	PDispBuff ptDispBuff;
	int start_x, start_y;  // 第一个按钮的左上角位置
	int pre_start_x, pre_start_y;
	int col, row, rows;
	int i = 0;
	PButton ptButton;
	
	/*算出单个按钮的width、height*/
	n = GetItemCfgCount();  // 得到按钮总数量
	ptDispBuff = GetDisplayBuffer();  // 得到region的信息
	xRes = ptDispBuff->iXres;
	yRes = ptDispBuff->iYres;
	width = sqrt(1.0 / 0.618 * xRes * yRes * n);
	n_per_line = xRes / width + 1;  // 为了不让width越界，要处理width使其小一点
	width  = xRes / n_per_line;
	height = 0.618 * width;  // 设定height与width为黄金比例

	/*居中显示：计算每个按钮的Region*/
	start_x = (xRes - width * n_per_line) / 2;
	rows = n / n_per_line;
	if(rows * n_per_line < n) ++rows;
	start_y = (yRes - rows * height) / 2;
	// 循环计算每个按钮的region
	for(row = 0; (row < rows) && (i < n); ++row)
	{
		pre_start_y = start_y + row * height;
		pre_start_x = start_x - width;
		for(col = 0; (col < n_per_line) && (i < n); ++col)
		{
			if(i == n) break;
			ptButton = &g_tButtons[i];
			ptButton->tRegion.leftUpX = pre_start_x + width;
			ptButton->tRegion.leftUpY = pre_start_y;
			ptButton->tRegion.width = width - X_GAP;
			ptButton->tRegion.height = height - Y_GAP;
			pre_start_x = ptButton->tRegion.leftUpX;

			/*ButtonInit*/
			ButtonInit(ptButton, GetItemCfgByIndex(i)->name, NUll, NUll, NULL);
			++i;
		}
	}
	
	/*OnDraw*/
	for(i = 0; i < n; ++i) g_tButtons[i].OnDraw(&g_tButtons[i], ptDispBuff);
}

static void MainPageRun(void* pParams)
{
	int error;
	
	/*读取配置文件*/
	error = ParseConfigFile();
	if(error)
	{
		printf("Parse config file error\n");
		return error;
	}

	/*根据配置文件生成按钮、界面*/

	while(1)
	{
		/*读取输入事件*/

		/*根据输入事件找到按钮*/

		/*调用按钮的OnPressed函数*/
		
	}
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
