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

int main(int argc, char **argv)
{
	PDispBuff ptBuffer;
	int error;

	/*若命令行第二个参数没有传入字库*/
	if(argc != 2)
	{
		printf("Usage: %s <font_file>\n", argv[0]);
		return -1;
	}
	
	/*初始化显示系统*/
	DisplayInit();
	SelectDefaultDisplay("fb");
	InitDefaultDisplay();
	ptBuffer = GetDisplayBuffer();
	
	/*初始化输入系统*/
	InputRegister();
	InputDeviceInit();

	/*初始化文字系统：需传入字库*/
	FontsRegister();
	error = SelectAndInitFont("freetype", argv[1]);
	if(error)
	{
		printf("SelectAndInitFont error\n");
		return -1;
	}

	/*初始化页面系统*/
	PagesRegister();

	/*运行业务系统的主页面*/
	Page("main")->Run(NULL);
	
	return 0;	
}

