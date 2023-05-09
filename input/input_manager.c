#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <input_manager.h>

static PInputDevice g_InputDevs = NULL;  // 设备头结点
static pthread_mutex_t g_tMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  g_tConVar = PTHREAD_COND_INITIALIZER;

/*Start of 环形缓冲区实现*/
// 定义数组
#define BUFFLEN 20
static InputEvent g_atInputEvents[BUFFLEN];
// 双指针
static int iWrite = 0;
static int iRead = 0;

static int isInputBufferFull()
{
	return (iRead == (iWrite + 1) % BUFFLEN);
}
static int isInputBufferEmpty()
{
	return iRead == iWrite;
}
// 写入缓冲区
static void PutInputEventToBuffer(PInputEvent ptInputEvent)
{
	if(!isInputBufferFull())
	{
		g_atInputEvents[iWrite] = *ptInputEvent;
		iWrite = (iWrite + 1) % BUFFLEN;
	}
}
// 读取缓冲区
static int GetInputEventFromBuffer(PInputEvent ptInputEvent)
{
	if(!isInputBufferEmpty())
	{
		*ptInputEvent = g_atInputEvents[iRead];
		iRead = (iRead + 1) % BUFFLEN;
		return 1;
	}
	else return 0;
}
/*End of 环形缓冲区实现*/

/*注册函数*/
void RegisterInputDevice(PInputDevice ptInputDevice)
{
	ptInputDevice->ptNext = g_InputDevs;
	g_InputDevs = ptInputDevice;
}

/*注册各个设备*/
void InputSystemRegister(void)
{
	// register touchscreen
	extern void TouchScreenDeviceRegister(void);
	TouchScreenDeviceRegister();
	// register netinput
	extern void NetInputDeviceRegister(void);
	NetInputDeviceRegister();
}

/*对于每个设备，初始化、并创建线程*/
static void* input_recv_my_thread_func (void* data)
{
	PInputDevice ptDev = (PInputDevice)data;
	InputEvent tEvent;
	int ret;
	
	while (1)
	{
		// 读数据
		ret = ptDev->GetInputEvent(&tEvent);
		if(!ret)
		{
			// 保存数据
			pthread_mutex_lock(&g_tMutex);
			PutInputEventToBuffer(&tEvent);
			// 唤醒等待数据的线程
			pthread_cond_signal(&g_tConVar);  // 通知接收线程
			pthread_mutex_unlock(&g_tMutex);
		}
	}
	return NULL;
}
void InputDeviceInit(void)
{
	PInputDevice tmp = g_InputDevs;
	int ret;
	pthread_t tid;

	while(tmp)
	{
		ret = tmp->DeviceInit();
		if(!ret)
		{	
			ret = pthread_create(&tid, NULL, input_recv_my_thread_func, tmp);
		}
		tmp = tmp->ptNext;
	}
}

/*供上层APP调用，获取各个设备输入的数据，没有数据就休眠*/
int GetInputEvent(PInputEvent ptInputEvent)
{
	InputEvent tEvent;
	int ret;

	pthread_mutex_lock(&g_tMutex);
	if(GetInputEventFromBuffer(&tEvent))
	{
		*ptInputEvent = tEvent;
		pthread_mutex_unlock(&g_tMutex);
		return 0;
	}
	else
	{
		// 休眠等待
		pthread_cond_wait(&g_tConVar, &g_tMutex);
		if(GetInputEventFromBuffer(&tEvent))
		{
			*ptInputEvent = tEvent;
			ret = 0;
		}
		else ret = -1;
		pthread_mutex_unlock(&g_tMutex);
	}
	return ret;
}

