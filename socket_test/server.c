#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	int server_sockfd, client_sockfd;
	int state, client_len;
	int pid;

	FILE* fp;
	struct sockaddr_in clientaddr, serveraddr;

	char buf[255];
	char line[255];

	if(argc != 2) {
		printf("argument error");
		exit(0);
	}

	memset(line, '0', 255);
	state = 0;

	// 주소 파일을 읽어들인다.
	client_len = sizeof(clientaddr);
	if((fp = fopen("test.txt", "r")) == NULL) {
		perror("file open error : ");
		exit(0);
	}

	// 소켓 생성
	if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error : ");
		exit(0);
	}

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(atoi(argv[1]));

	// 바인딩 (소켓에 이름 연결)
	if(bind(server_sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
		perror("bind error : "); 
		exit(0);
	}

	if(listen(server_sockfd, 5) == -1) {
		perror("listen error : ");
		exit(0);
	}

	while(1) {
		client_sockfd = accept(server_sockfd, (struct sockaddr*)&clientaddr, &client_len);

		if(client_sockfd == -1) {
			perror("Accept error : ");
			exit(0);
		}
		// printf("test");

		/* 출력문 넣어서 이 위치에서 하는 오퍼레이션 확인 */
		while(1) {
			/* rewind -> 파일 읽기/쓰기 포인터 위치를 처음으로 초기화 */
			rewind(fp);
			memset(buf, '0', 255);

			/* 클라이언트가 입력한 내용을 버퍼에 담는다 read는 fd에서 읽어들인 바이트수를 반환 */
			if(read(client_sockfd, buf, 255) <= 0) {
				close(client_sockfd);
				break;
			}
			printf("클라이언트 입력내용 : %s\n", buf);

			/* 클라이언트가 입력한 내용이 quit이면 break */
			if(strncmp(buf, "quit", 4) == 0) {
				write(client_sockfd, "bye bye", 0);
				close(client_sockfd);
				break;
			}


			while(fgets(line, 255, fp) != NULL) {
				/* 클라이언트가 입력한 문자열이 포함된 fp내의 내용을 반복문 돌며 전부 출력 */
				if(strstr(line, buf) != NULL) {
					write(client_sockfd, line, 255);
				}
				memset(line, '0', 255);
			}

			write(client_sockfd, "end", 255);
			printf("send end\n");
		}
	}
	close(client_sockfd);
}