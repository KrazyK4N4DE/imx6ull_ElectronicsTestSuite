#ifndef __UI_H__
#define __UI_H__

#include <common.h>
#include <disp_manager.h>
#include <input_manager.h>

#define BUTTON_DEFAULT_COLOR 0xff0000  // 默认颜色
#define BUTTON_PRESSED_COLOR 0x00ff00  // 按压变色
#define BUTTON_TEXT_COLOR    0x000000  // 文字颜色
#define BUTTON_PERCENT_COLOR 0x0000ff  // 数字颜色

struct Button;

typedef int (*ONDRAW_FUNC)(struct Button* ptButton, PDispBuff ptDispBuff);
typedef int (*ONPRESSED_FUNC)(struct Button* ptButton, PDispBuff ptDispBuff, PInputEvent ptInputEvent);
typedef struct Button
{
	char* name;
	int status;  // 状态标记位
	Region tRegion;  // 按钮的大小
	ONDRAW_FUNC OnDraw;  // 绘制按钮，struct Button* ptButton表示绘制按钮本身
	ONPRESSED_FUNC OnPressed;  // 使用按钮的方法
}Button, * PButton;

void ButtonInit(PButton ptButton, char* name, PRegion ptRegion, ONDRAW_FUNC OnDraw, ONPRESSED_FUNC OnPressed);

#endif
