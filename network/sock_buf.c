#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>



void error_handling(char *message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}




int main(int argc, char *argv[])
{
	int tcp_sock, udp_sock;
	int snd_buf, rcv_buf;
	socklen_t optlen;
	int state_snd, state_rcv;

	optlen_snd = sizeof(snd_buf);
	optlen_rcv = sizeof(rcv_buf);
	
	// TCP socket 생성
	tcp_sock = socket(PF_INET,SOCK_STREAM,0);


	// TCP getsockopt()
	state_snd = getsockopt(tcp_sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &optlen_snd);
	state_rcv = getsockopt(tcp_sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &optlen_rcv);
	
	if(state_snd)
		error_handling("TCP getsockopt() error");
	printf("Socket output buffer size : %d\n", snd_buf);

	if(state_rcv)
		error_handling("TCP getsockopt() error");
	printf("Socket output buffer size : %d\n", rcv_buf);
	
	return 0;
}
