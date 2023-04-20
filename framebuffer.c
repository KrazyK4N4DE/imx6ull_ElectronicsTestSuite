#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "disp_manager.h"

static int fd_fb;
static struct fb_var_screeninfo var;	/* Current var */
static int screen_size;
static unsigned char *fb_base;
static unsigned int line_width;
static unsigned int pixel_width;

static int FbDeviceInit()
{
	fd_fb = open("/dev/fb0", O_RDWR);
	if (fd_fb < 0)
	{
		printf("can't open /dev/fb0\n");
		return -1;
	}
	if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
	{
		printf("can't get var\n");
		return -1;
	}

	line_width  = var.xres * var.bits_per_pixel / 8;
	pixel_width = var.bits_per_pixel / 8;
	screen_size = var.xres * var.yres * var.bits_per_pixel / 8;
	fb_base = (unsigned char *)mmap(NULL , screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
	if (fb_base == (unsigned char *)-1)
	{
		printf("can't mmap\n");
		return -1;
	}
	return 0;
}
static int FbDeviceExit()
{
	munmap(fd_fb, screen_size);
	close(fd_fb);
	return 0;
}

/*
* 可以返回LCD的Framebuffer，上层APP可直接操作LCD，这时FbFlushRegion可写为空
* 也可以malloc返回一块无关的buffer，要使用FbFlushRegion
*/
static int FbGetBuffer(PDispBuff ptDispBuff)  // 获得一个buffer，在上面绘制图片
{
	ptDispBuff->iXres = var.xres;
	ptDispBuff->iYres = var.yres;
	ptDispBuff->iBpp = var.bits_per_pixel;
	ptDispBuff->buff = fb_base;
	return 0;
}
static int FbFlushRegion(PRegion ptRegion, PDispBuff ptDispBuff)  // 把绘制好的区域刷出来
{
	return 0;
}

static DisOpr g_tFramebufferOpr = 
{
	.name = "fb",
	.GetBuffer = FbGetBuffer,
	.FlushRegion = FbFlushRegion,
	.DeviceInit = FbDeviceInit,
	.DeviceExit = FbDeviceExit
};
// 将结构体注册进去
void FramebufferInit()
{
	RegisterDisplay(&g_tFramebufferOpr);
}

