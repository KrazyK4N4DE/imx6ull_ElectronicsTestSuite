#include <ui.h>	

static int DefaultOnDraw(struct Button* ptButton, PDispBuff ptDispBuff)
{
	// 先把整个按钮底色绘制成默认颜色
	DrawRegion(&ptButton->tRegion, BUTTON_DEFAULT_COLOR);  
	// 居中写文字
	DrawTextInRegionCentral(ptButton->name, &ptButton->tRegion, BUTTON_TEXT_COLOR);
	// 刷到lcd/web上
	FlushDisplayRegion(&ptButton->tRegion, ptDispBuff);
	return 0;
}
static int DefaultOnPressed(struct Button* ptButton, PDispBuff ptDispBuff, PInputEvent ptInputEvent)
{
	unsigned int dwColor = BUTTON_DEFAULT_COLOR;

	ptButton->status = !ptButton->status;  // 每次按压反转一下
	if(ptButton->status) dwColor = BUTTON_PRESSED_COLOR;
	// 先把整个按钮底色绘制成默认颜色
	DrawRegion(&ptButton->tRegion, dwColor);  
	// 居中写文字
	DrawTextInRegionCentral(ptButton->name, &ptButton->tRegion, BUTTON_TEXT_COLOR);
	// 刷到lcd/web上
	FlushDisplayRegion(&ptButton->tRegion, ptDispBuff);

	return 0;
}

void ButtonInit(PButton ptButton, char* name, PRegion ptRegion, ONDRAW_FUNC OnDraw, ONPRESSED_FUNC OnPressed)
{
	ptButton->status = 0;
	ptButton->name = name;
	if(ptRegion) ptButton->tRegion = *ptRegion;
	ptButton->OnDraw = OnDraw ? OnDraw : DefaultOnDraw;
	ptButton->OnPressed = OnPressed ? OnPressed : DefaultOnPressed;
}

