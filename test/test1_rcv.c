#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 50

void error_handling(char *message);

int main(int argc, char *argc[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[BUFSIZE];
	int srt_len;

	if(argc!=3)
	{
		printf("Usage ;%s [IP][PORT]\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM_0);
	if(sock == -1)
	{
		error_handling("socket() error");
	}
	
	memset(&serv_addr, 0, sizeof(serv_addr);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if(connect(sock, (struc sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("connect() error");
	}
	
***************************************************************	
	str_len = read(sock, message, sizeof(message);
***************************************************************

	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


