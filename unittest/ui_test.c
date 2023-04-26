#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <font_manager.h>
#include <disp_manager.h>
#include <ui.h>

int main(int argc, char **argv)
{
	PDispBuff ptBuffer;
	Button tButton;
	Region tRegion;
	int error;

	if(argc != 2)
	{
		printf("Usage: %s <font_size>\n", argv[0]);
		return -1;
	}

	/*显示系统初始化*/
	DisplayInit();
	SelectDefaultDisplay("fb");
	InitDefaultDisplay();
	ptBuffer = GetDisplayBuffer();
	
	/*字体系统初始化*/
	FontsRegister();
	error = SelectAndInitFont("freetype", argv[1]);
	if(error)
	{
		printf("SelectAndInitFont error\n");
	}
	
	ButtonInit(&tButton, "test", &tRegion, NULL, NULL);
	tButton.OnDraw(&tButton, ptBuffer);
	/*模拟点击*/
	while(1)
	{
		tButton.OnPressed(&tButton, ptBuffer, NULL);
		sleep(2);  // 休眠2s
	}
	return 0;	
}

