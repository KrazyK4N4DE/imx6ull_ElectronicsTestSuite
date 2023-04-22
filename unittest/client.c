#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

/*
* socket()
* connect()			用该函数建立到服务器的连接，在udp中可用可不用
* 用connect()的话，用send()
* 不用connect()的话，用sendto()，其包括了目的地
*/

#define SERVER_PORT 8888

int main(int argc, char** argv)
{
	int iSocketClient;
	struct sockaddr_in tSocketServerAddr;
	int iRet;
	int iAddrLen;
	int iSendLen;

	/*命令行中没有输入要连接的服务器地址时*/
	if(argc != 3)
	{
		printf("Usage: \n");
		printf("%s <server_ip> <str>\n", argv[0]);
		return -1;
	}

	/*开始socket()*/
	iSocketClient = socket(AF_INET, SOCK_DGRAM, 0);

	tSocketServerAddr.sin_family = AF_INET;
	tSocketServerAddr.sin_port = htons(SERVER_PORT);
	if(inet_aton(argv[1], &tSocketServerAddr.sin_addr) == 0)
	{
		printf("Invalid server_ip\n");
		return -1;
	}
	memset(tSocketServerAddr.sin_zero, 0, 8);  // __pad就是sin_zero
	
#if 0
	// int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	iRet = connect(iSocketClient, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if(iRet == -1)
	{
		printf("Connect Error\n");
		return -1;
	}
#endif

	/*连接成功了就可以send了*/

	// ssize_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen);
	iAddrLen = sizeof(struct sockaddr);
	iSendLen = sendto(iSocketClient, argv[2], strlen(argv[2]), 0, (const struct sockaddr *)&tSocketServerAddr, iAddrLen);
	close(iSocketClient);

	return 0;
}

