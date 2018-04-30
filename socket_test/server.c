#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT	7777

int main() {
	int s;

	struct sockaddr_in6 routerAddr, localAddr;
	struct sockaddr_in serverAddr;
	int routerLen = sizeof(routerAddr);

	char buf[255];

	// 소켓 생성
	if((s = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
		perror("socket error : ");
		exit(0);
	}

	memset(&localAddr, 0x00, sizeof(localAddr));
	localAddr.sin6_family = AF_INET6;
	localAddr.sin6_addr = in6addr_any;
	localAddr.sin6_port = htons(PORT);

	memset(&serverAddr, 0x00, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("13.209.8.64");
	serverAddr.sin_port = htons(PORT);


	// 바인딩 (소켓에 이름 연결)
	if(bind(s, (struct sockaddr*)&localAddr, sizeof(localAddr)) == -1) {
		perror("bind error : "); 
		exit(0);
	}

	/* 출력문 넣어서 이 위치에서 하는 오퍼레이션 확인 */
	while(1) {
		memset(buf, '\0', 255);

		/* 클라이언트가 입력한 내용을 버퍼에 담는다 read는 fd에서 읽어들인 바이트수를 반환 */
		recvfrom(s, buf, 255, 0, (struct sockaddr*)&routerAddr, &routerLen);
		printf("Data: %s\n", buf);

		sendto(s, buf, strlen(buf) + 1, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	}
	printf("a client disconnected\n");
	close(s);
}