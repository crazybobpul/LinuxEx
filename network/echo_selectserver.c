#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define BUFSIZE 30


void error_handling(char *message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}


int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock, i;
	struct sockaddr_in serv_addr, clnt_addr;
	struct timeval timeout;
	fd_set reads, cpy_reads;	

	socklen_t adr_sz;
	int fd_max, fd_num, str_len, state;
	char buf[BUFSIZE];
 	

	if(argc!=2)
	{	
		printf("USAGE : %s [PORT]\n", argv[0]);
		exit(1);
	}
	
	
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


	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	fd_max=serv_sock;
	
	
	while(1)
	{
		cpy_reads=reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout);
		//select 함수가 정상적으로 호출되지 않은 경우		
		if(fd_num==-1)
		{
			break;
		}

		//timeout
		else if(fd_num==0)
		{
			continue;
		}
		
		for(i=3; i<fd_max+1; i++)
		{
			//connection request 
			if(FD_ISSET(i, &cpy_reads))
			{
				if(i==serv_sock)
				{				
					adr_sz = sizeof(clnt_addr);
					clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &adr_sz);
					FD_SET(clnt_sock, &reads);
					if(fd_max<clnt_sock)
						fd_max=clnt_sock;
					printf("Connected client : %d\n", clnt_sock);
				}	
			
				else  // read message
				{
					str_len = read(i, buf, BUFSIZE);
					// 수신된 데이터가 0인 경우				
					if(str_len==0)
					{	
						FD_CLR(i, &reads);
					close(i);	
					printf("closed client:%d\n", i);
					}
					// 정상적으로 데이터를 수신한 경우
					else 
					{
						write(i, buf, str_len);
					}	
				}
			}		

		}
	
	}
	close(serv_sock);
	return 0;

}



	
	
