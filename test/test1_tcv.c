#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 30 
#define TRUE	1
#define FALSE	0


void error_handling(char *message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}

void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid=waitpid(-1, &status, WNOHANG);
	printf("removed proc id : %d\n", pid);
}


int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	int option;

	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUFSIZE];
	socklen_t optlen;

	int i;
	int num1_start = 1;	
	int num1_end = 10000;
	int num2_start = num1.end+1;
	int num2_end = num1.end>>1;
	int result1, result2;
	long sum; 


	if(argc!=2)
	{
		printf("Usage : %s [port]\n", argv[0]);
		exit(1);
	}

	// 시그널 및 핸들러 등록
	act.sa_handler=read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	state = sigaction(SIGCHLD, &act, 0);

	// 1. socket()
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	// 2. 서버 어드레스 설정
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));


	// 3. bind()
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("bind() error");
	
	// 4. listen()
	if(listen(serv_sock,5)==-1)
		error_handling("listen() error");

	while(1)
	{
		adr_sz = sizeof(clnt_addr);	
		// 5. accpet()
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz);
		if(clnt_sock==-1)
		{
			continue;
		}
		else
			printf("new client connected...\n");

		pid = fork();

		// 자식프로세스가 생성되지 않은 경우
		if(pid==-1)
		{
			close(clnt_sock);
			continue;
		}
		// 자식 프로세스의 경우(자식 프로세스는 생성)
		else if(pid==0)
		{
			close(serv_sock);
			// 상대측에서 half-close나 close를 하여 EOF를 송신하면
			// while문을 빠져나온다.
					
			for(i=num1_start; i<num1_end; i++)
			{
				result1 +=i;
			}


			while((str_len = read(clnt_sock, buf, sizeof(buf)))!=0)
				write(clnt_sock, buf, str_len);
			close(clnt_sock);
			printf("client disconnected...\n");
			return 0;
		}
		// 부모 프로세스인 경우
		else
		{

			for(i=num2.start; i<num2.end; i++)
			{
				result2 +=i;	
			}
			sum = result1 + result2	
			close(clnt_sock);
		}
		printf("sum = %ld\n", sum); 
	}
	close(serv_sock);
	return 0;
}		


