#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <sys/time.h>

#define INPUT_TYPE_TOUCH 1  // 触摸屏事件
#define INPUT_TYPE_NET 2  // 网络事件

// 抽象出输入系统的结构体，可以触摸屏输入、网络输入等等
typedef struct InputEvent
{
	struct timeval tTime;  // 表示时间
	int iType;  // 输入类型
	int iX;  // 坐标X
	int iY;  // 坐标Y
	int iPressure;  // 按压压力
	char str[1024];  // 对于网络事件，传输字符串来输入
}InputEvent, * PInputEvent;
// 抽象出第二个结构体，表示输入设备
typedef struct InputDevice
{
	char* name;  // 名称
	int (*GetInputEvent)(PInputEvent ptInputEvent);
	int (*DeviceInit)();
	int (*DeviceExit)();
	struct InputDevice* ptNext;
}InputDevice, * PInputDevice;

void RegisterInputDevice(PInputDevice ptInputDevice);
void InputRegister();
void InputDeviceInit();
int GetInputEvent(PInputEvent ptInputEvent);

#endif
