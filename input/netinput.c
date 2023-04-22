#include <input_manager.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


#ifndef NULL
#define NULL (void* )0
#endif
#define SERVER_PORT 8888
static int g_iSocketServer;

static int NetInputGetInputEvent(PInputEvent ptInputEvent)
{
	struct sockaddr_in tSocketClientAddr;  // 客户端的地址信息
	unsigned int iAddrLen;
	char aRecvBuf[1000];
	int iRecvLen;  // 接收到的数据
	
	// ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
	iAddrLen = sizeof(struct sockaddr);
	iRecvLen = recvfrom(g_iSocketServer, aRecvBuf, 999, 0, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
	if(iRecvLen > 0)
	{
		aRecvBuf[iRecvLen] = '\0';
		ptInputEvent->iType = INPUT_TYPE_NET;
		// int gettimeofday(struct timeval *tv, struct timezone *tz);
		gettimeofday(&ptInputEvent->tTime, NULL);
		strncpy(ptInputEvent->str, aRecvBuf, 1000);
		ptInputEvent->str[999] = '\0';
		return 0;
	}
	else return -1;
}
static int NetInputDeviceInit()
{
	struct sockaddr_in tSocketServerAddr;
	int iRet;  // 临时变量iReturn

	// int socket(int domain, int type, int protocol);
	g_iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);  // AF_INET表示IPv4，SOCK_DGRAM表示UDP编程
	if (g_iSocketServer == -1)
	{
		printf("Socket Error\n");
		return -1;
	}

	tSocketServerAddr.sin_family = AF_INET;
	tSocketServerAddr.sin_port = htons(SERVER_PORT);
	tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;  // INADDR_ANY表示主机内所有IP
	memset(tSocketServerAddr.sin_zero, 0, 8);  // __pad就是sin_zero

	/*开始bind*/
	// int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	iRet = bind(g_iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr_in));
	if (iRet == -1)
	{
		printf("Bind Error\n");
		return -1;
	}
	return 0;
}
static int NetInputDeviceExit()
{
	close(g_iSocketServer);
	return 0;
}

static InputDevice g_tNetInputDev = 
{
	.name = "NetInput",
	.GetInputEvent = NetInputGetInputEvent,
	.DeviceInit = NetInputDeviceInit,
	.DeviceExit = NetInputDeviceExit
};

#if 1
int main(int argc, char * * argv)
{
	InputEvent event;
	int ret;

	g_tNetInputDev.DeviceInit();
	while(1)
	{
		ret = g_tNetInputDev.GetInputEvent(&event);
		if(ret)
		{
			printf("GetInputEvent Err!\n");
			return -1;
		}
		else
		{
			printf("Type       : %d\n", event.iType);
			printf("Str        : %s\n", event.str);
		}
	}
	
	return 0;
}
#endif
