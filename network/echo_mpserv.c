#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 30
#define TRUE 1
#define FALSE 0

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
	pid = waitpid(-1, &status, WNOHANG);
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

	if(argc!=2)
	{	
		printf("USAGE : %s [PORT]\n", argv[0]);
		exit(1);
	}
	
	// 시그널 및 핸들러 등록 
	
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0); 
	/*int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact); 
	  성공하면 0, 실패하면 -1 반환한다 */

	// 1. socket()

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	/* int socket(int domain, int type, int protocol); int protocol은 특정 프로토콜을 사용하기 위해 지정하는 변수이며 보통 0으로 설정 */

	// 2. 서버 어드레스 설정
	memset(&serv_addr, 0, sizeof(serv_addr)); 
	/* void *memset(void *s, int c, size_t n); malloc()에서 할당받은 메모리를 int c 값으로 size_t n 만큼 초기화 함 */
 
	serv_addr.sin_family = AF_INET; //주소체계 정보 저장 
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	/* htonl 네트워크 byte order (big endian) 로 변경*/
	serv_addr.sin_port = htons(atoi(argv[1])); // 


	// 3-1. bind () error 방지를 위해 SO_REUSEADDR를 TRUE로 설정

	optlen = sizeof(option);
	option = TRUE;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, optlen);

	// 3. bind () 소켓에 IP주소와 포트번호 지정. 
	/* int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen); 
		성공하면 0, 실패하면 -1 반환 */
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("Bind () error");
	
	// 4. listen ()
	/* int listen(int s, int backlog); 소켓 디스크립터, 대기 메시지 큐의 개수. 
		성공하면 0, 실패하면 -1 */ 
	if(listen(serv_sock, 5)==-1)
		error_handling("Listen () error");
	
	
	while(1)
	{
		adr_sz = sizeof(clnt_addr);
		// 5. accept () 
		/* int accept(int s, struct sockaddr *addr, socklen_t *addrlen); 
		소켓 디스크립터, 클라이언트 주소 정보를 가지고 있는 포인터, 포인터가 가리키는 구조체의 크기.
		성공하면 -1 이외의 소켓 디스크립터, 실패하면 -1 */
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz);
		
		if(clnt_sock == -1)
		{
			continue; // while retry
		}
		else 
			printf("new client connected...\n");
	
		pid = fork(); 
	        
		// 자식 프로세스가 생성되지 않은 경우
		if(pid == -1)
		{	
			close(clnt_sock);
			continue; 
		}

		// 자식 프로세스의 경우(자식 프로세스는 생성)
		else if(pid == 0)
		{
			close(serv_sock);
			// 상대측에서 halfclose나 close를 하여 EOF 를 송신하면 while 문을 빠져나옴
			while((str_len = read(clnt_sock, buf, sizeof(buf)))!=0)
				write(clnt_sock, buf, str_len); 
			close(clnt_sock);
			printf("client disconnected... \n");
			return 0;
		}

		// 부모 프로세스인 경우
		else 
		{		
			close(clnt_sock);
			
		}

	}
	close(serv_sock);
	return 0;

}



	
	
