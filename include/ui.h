#idndef __UI_H__
#define __UI_H__

#include <common.h>
#include <disp_manager.h>
#include <input_manager.h>

typedef struct Button
{
	char* name;
	Region tRegion;  // 按钮的大小
	int (*OnDraw)(struct Button* ptButton, PDispBuff ptDispBuff);  // 绘制按钮，struct Button* ptButton表示绘制按钮本身
	int (*OnPressed)(struct Button* ptButton, PDispBuff ptDispBuff, PInputEvent ptInputEvent);  // 使用按钮的方法
}Button, * PButton;

#endif