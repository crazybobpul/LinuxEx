
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "test1.h"

#define TRUE 1
#define FALSE 0

// ./hello_server [PORT]
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;

	int option;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	socklen_t optlen;

	char message[]="Hello World!";
	char *ipaddr;
	

	struct cal test1;
	int result=0;
	int i;
	

	if(argc!=2)
	{
		printf("Usage : %s [port]\n", argv[0]);
		exit(1);
	}

	// 1. socket() 소켓을 생성한다.
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock ==-1)
		error_handling("socket() error");

	// 2. 서버의 주소값(ip, port)을 설정한다.
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);	// IP설정
	serv_addr.sin_port=htons(atoi(argv[1]));	// PORT설정


	// port 할당이 가능하도록 하여 bind error 가 생성되지 않음	
	optlen = sizeof(option);
	option = TRUE;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, optlen);




	// 3. 서버의 주소값을 bind한다.
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
		error_handling("bind() error");
	
	// 4. listen() 동시접속 최대허용수를 5로 설정한다.
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

	clnt_addr_size = sizeof(clnt_addr);

	// 5. accept() 
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
	if(clnt_sock==-1)
		error_handling("accept() error");

	read(clnt_sock, &test1, sizeof(test1));
	printf("calculate %d, %d, %d, %d, %d with %c\n", test1.oprd[0], test1.oprd[1], test1.oprd[2], test1.oprd[3], test1.oprd[4], test1.oprt);

	switch (test1.oprt)

	{
	case '+':
		for(i=0; i<test1.operandCount; i++)
		{
			result += test1.oprd[i];
			printf("result[%d]=%d\n",i,result);
		}
		//result = test1.oprd[0] + test1.oprd[1] + test1.oprd[2] + test1.oprd[3] + test1.oprd[4];
		printf("result with %c : %d/n", test1.oprt, result);
	break; 

	case '-':
		result=test1.oprd[0];
		for(i=1; i<test1.operandCount; i++)
			result-=test1.oprd[i];
			
		//result = test1.oprd[0] - test1.oprd[1] - test1.oprd[2] - test1.oprd[3] - test1.oprd[4];
		printf("result with %c : %d/n", test1.oprt, result);
        break;  
	
	case '*':
		for(i=0; i<test1.operandCount; i++)
                        result *= test1.oprd[i];

		//result = test1.oprd[0] * test1.oprd[1] * test1.oprd[2] * test1.oprd[3] * test1.oprd[4];
        	printf("result with %c : %d/n", test1.oprt, result);
        break;
        }
		
/*
	write(clnt_sock, message, sizeof(message));

	ipaddr = inet_ntoa(clnt_addr.sin_addr);
	printf("client ip addr : %s\n", ipaddr);
*/
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
