#include <page_manager.h>
#include <config.h>
#include <ui.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define X_GAP 5
#define Y_GAP 5

static Button g_tButtons[ITEMCFG_MAX_NUM];
static int g_iButtonCount;

/*OnPressed函数*/
static int MainPageOnPressed(struct Button* ptButton, PDispBuff ptDispBuff, PInputEvent ptInputEvent)
{	
	unsigned int dwColor = BUTTON_DEFAULT_COLOR;
	char name[100];
	char status[100];
	char* strButton = ptButton->name;  // 按钮名字的通用变量
	char* command_status[3] = {"err", "ok", "percent"};
	int command_status_index = 0;
	char command[105];
	PItemCfg ptItemCfg;

	// 1.1 对于触摸屏事件
	if(ptInputEvent->iType == INPUT_TYPE_TOUCH)
	{
		// 1.2 分辨能否被点击
		if (GetItemCfgByName(ptButton->name)->bCanBeTouched == 0) return -1;
		// 1.3 修改颜色
		ptButton->status = !ptButton->status;  // 每次按压反转一下
		if(ptButton->status)
		{
			dwColor = BUTTON_PRESSED_COLOR;
			command_status_index = 1;
		}
	}
	// 2.1 对于网络事件
	else if(ptInputEvent->iType == INPUT_TYPE_NET)
	{
		// 2.2 根据传入的字符串修改颜色
		sscanf(ptInputEvent->str, "%s %s", name, status);
		if(strcmp(status, "ok") == 0)
		{
			dwColor = BUTTON_PRESSED_COLOR;
			command_status_index = 1;
		}
		else if(strcmp(status, "err") == 0)
		{
			dwColor = BUTTON_DEFAULT_COLOR;
			command_status_index = 0;
		}
		else if(status[0] >= '0' && status[0] <= '9')
		{
			dwColor = BUTTON_PERCENT_COLOR;
			strButton = status;
			command_status_index = 2;
		}
		else return -1;
	}
	else return -1;
	// 先把整个按钮底色绘制成默认颜色
	DrawRegion(&ptButton->tRegion, dwColor);  
	// 居中写文字
	DrawTextInRegionCentral(strButton, &ptButton->tRegion, BUTTON_TEXT_COLOR);
	// 刷到lcd/web上
	FlushDisplayRegion(&ptButton->tRegion, ptDispBuff);
	// 执行command
	ptItemCfg = GetItemCfgByName(ptButton->name);
	if(ptItemCfg->command[0] != '\0')
	{
		sprintf(command, "%s %s", ptItemCfg->command, command_status[command_status_index]);
		system(command);  // 调用command
	}

	return 0;
}

/*获取所有Button中最长的文字大小*/
static int GetFontSizeForAllButtons(void)
{
	int i, max_index = 0;
	int len, max_len = -1;
	RegionCartesian tRegionCar;
	float k, kx, ky;  // 缩放因子
	
	/*1. 找出name最长的Button*/
	for(i = 0; i < g_iButtonCount; ++i)
	{
		len = strlen(g_tButtons[i].name);
		if(len > max_len)
		{
			max_len = len;  // 最长文字长度更新
			max_index = i;  // 最大下标更新
		}
	}
	
	/*2. 以font_size = 100为例，算出它的外框*/
	SetFontSize(100);
	GetStringRegionCar(g_tButtons[max_index].name, &tRegionCar);
	
	/*3. 把文字的外框缩放为Button的外框*/
	kx = 1.0 * g_tButtons[max_index].tRegion.width / tRegionCar.width;
	ky = 1.0 * g_tButtons[max_index].tRegion.height / tRegionCar.height;
	k = kx  < ky ? kx : ky;  // 取kx、ky中最小因子，以防其中一边区域溢出
	
	/*4. 反算出font_size，只取0.80，避免文字过于接近边界*/
	return k * 100 * 0.8;
}

/*生成按钮，设定Button外框大小均匀分布*/
static void GenerateButtons(void)
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
	int iFontSize;
	
	/*算出单个按钮的width、height*/
	g_iButtonCount = n = GetItemCfgCount();  // 得到按钮总数量
	ptDispBuff = GetDisplayBuffer();  // 得到region的信息
	xRes = ptDispBuff->iXres;
	yRes = ptDispBuff->iYres;
	width = sqrt(1.0 / 0.618 * xRes * yRes / n);
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
			ptButton = &g_tButtons[i];
			ptButton->tRegion.leftUpX = pre_start_x + width;
			ptButton->tRegion.leftUpY = pre_start_y;
			ptButton->tRegion.width = width - X_GAP;
			ptButton->tRegion.height = height - Y_GAP;
			pre_start_x = ptButton->tRegion.leftUpX;

			/*ButtonInit*/
			ButtonInit(ptButton, GetItemCfgByIndex(i)->name, NULL, NULL, MainPageOnPressed);
			++i;
		}
	}

	iFontSize = GetFontSizeForAllButtons();
	
	/*OnDraw*/
	for(i = 0; i < n; ++i)
	{
		g_tButtons[i].iFontSize = iFontSize;
		g_tButtons[i].OnDraw(&g_tButtons[i], ptDispBuff);
	}
}

/*判断触摸屏输入的位置是否在按钮区域内*/
static int isTouchPointInRegion(int iX, int iY, PRegion ptRegion)
{
	// 判断X方向
	if(iX < ptRegion->leftUpX || iX >= ptRegion->leftUpX + ptRegion->width) return 0;
	// 判断Y方向
	if(iY < ptRegion->leftUpY || iY >= ptRegion->leftUpY + ptRegion->height) return 0;
	return 1;
}

/*通过网络输入的名字查找按钮*/
static PButton GetButtonByName(char* name)
{
	for(int i = 0; i < g_iButtonCount; ++i)
		if(strcmp(name, g_tButtons[i].name) == 0) return &g_tButtons[i];
	return NULL;
}


/*通过输入事件处理按钮*/
static PButton GetButtonByInputEvent(PInputEvent ptInputEvent)
{
	char name[100];
	if(ptInputEvent->iType == INPUT_TYPE_TOUCH)
	{
		for(int i = 0; i < g_iButtonCount; ++i)
			if(isTouchPointInRegion(ptInputEvent->iX, ptInputEvent->iY, &g_tButtons[i].tRegion)) return &g_tButtons[i];
	}
	else if(ptInputEvent->iType == INPUT_TYPE_NET)
	{
		sscanf(ptInputEvent->str, "%s", name);
		return GetButtonByName(name);
	}
	else return NULL;
	return NULL;
}

static void MainPageRun(void* pParams)
{
	int error;
	InputEvent tInputEvent;
	PButton ptButton;
	PDispBuff ptDispBuff = GetDisplayBuffer();
	
	/*读取配置文件*/
	error = ParseConfigFile();
	if(error)
	{
		printf("Parse config file error\n");
		return;
	}

	/*根据配置文件生成按钮、界面*/
	GenerateButtons();

	while(1)
	{
		/*读取输入事件*/		
		error = GetInputEvent(&tInputEvent);
		if(error) continue;

		/*根据输入事件找到按钮*/
		ptButton = GetButtonByInputEvent(&tInputEvent);
		if(!ptButton) continue;

		/*调用按钮的OnPressed函数*/
		ptButton->OnPressed(ptButton, ptDispBuff, &tInputEvent);
		
	}
}

static PageAction g_tMainPage = 
{
	.name = "main",
	.Run = MainPageRun
};

void MainPageRegister(void)
{
	RegisterPage(&g_tMainPage);
}
