#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define  BUFF_SIZE   1024

int   main( int argc, char **argv)
{
   int   sock;
   int   server_addr_size;

   struct sockaddr_in   server_addr;

   char   buff_rcv[BUFF_SIZE+5];


   sock  = socket( PF_INET, SOCK_DGRAM, 0);
   
   if( -1 == sock)
   {
      printf( "socket 생성 실패n");
      exit( 1);
   }

   memset( &server_addr, 0, sizeof( server_addr));
   server_addr.sin_family     = AF_INET;
   server_addr.sin_port       = htons(7777);
   server_addr.sin_addr.s_addr= inet_addr( "127.0.0.1");

   sendto( sock, argv[1], strlen( argv[1])+1, 0,    // +1: NULL까지 포함해서 전송
            ( struct sockaddr*)&server_addr, sizeof( server_addr));          

   server_addr_size  = sizeof( server_addr);
   recvfrom( sock, buff_rcv, BUFF_SIZE, 0 , 
            ( struct sockaddr*)&server_addr, &server_addr_size);
   printf( "receive: %sn", buff_rcv);
   close( sock);
   
   return 0;
}



// #include <sys/types.h>
// #include <sys/stat.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

// int main(int argc, char **argv)
// {
//     int send_len;

//     int client_len;
//     int client_sockfd;

//     char buf_in[255];
//     char buf_get[255];

//     struct sockaddr_in clientaddr;

//     if (argc != 2)
//     {
//         perror("argument error : ");
//         exit(0);
//     }


//     client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     clientaddr.sin_family = AF_INET;
//     // clientaddr.sin_addr.s_addr = inet_addr("13.209.8.64");
//     clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//     clientaddr.sin_port = htons(atoi(argv[1]));

//     client_len = sizeof(clientaddr);

//     if (connect(client_sockfd, (struct sockaddr *)&clientaddr, client_len) < 0)
//     {
//         perror("Connect error: ");
//         exit(0);
//     }
//     while(1)
//     {
//         printf("입력 : ");
//         fgets(buf_in, 255, stdin);

//         buf_in[strlen(buf_in) - 1] = '\0';
//         /* for C server */
//         // write(client_sockfd, buf_in, 255);

//         /* for node server */
//         send_len = strlen(buf_in);
//         write(client_sockfd, buf_in, send_len);

//         if (strncmp(buf_in, "quit", 4) == 0)
//         {
//             close(client_sockfd);
//             exit(0);
//         }

//         read(client_sockfd, buf_get, 255);
//         printf("%s\n", buf_get);
//     }

//     close(client_sockfd);
//     exit(0);
// }