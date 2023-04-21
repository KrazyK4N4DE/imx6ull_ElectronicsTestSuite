#include <input_manager.h>

struct tsdev* g_ts;

static int TouchScreenGetInputEvent(PInputEvent ptInputEvent)
{
	struct ts_sample samp;
	in ret;
	
	ret = ts_read(g_ts, &samp, 1);
	if(ret != 1) return -1;

	// 将samp里面的值逐个赋给ptInputEvent，作转换
	ptInputEvent->iType = INPUT_TYPE_TOUCH;
	ptInputEvent->iX = samp.x;
	ptInputEvent->iY = samp.y;
	ptInputEvent->iPressure = samp.pressure;
	ptInputEvent->tTime = samp.tv;

	return 0;
}
static int TouchScreenDeviceInit()
{
	g_ts = ts_setup(NULL, 0);
	if (!g_ts)
	{
		printf("ts_setup err\n");
		return -1;
	}
	return 0;
}
static int TouchScreenDeviceExit()
{
	ts_close(g_ts);
	return 0;
}

static InputDevice g_tTouchScreenDev = 
{
	.name = "touchscreen",
	.GetInputEvent = TouchScreenGetInputEvent,
	.DeviceInit = TouchScreenDeviceInit,
	.DeviceExit = TouchScreenDeviceExit
};
