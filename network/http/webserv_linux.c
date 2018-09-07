#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void* request_handler(void* arg);

void send_data(FILE* fp, char* ct, char* file_name);

char* content_type(char* file);

void send_error(FILE* fp);

void error_handling(char* message);



int main(int argc, char *argv[])
{

	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_size;
	char buf[BUF_SIZE];
	pthread_t t_id;	

	if(argc!=2) 
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// 1. socket() 소켓 생성	

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);

	// 2. 서버의 주소값 (IP, PORT) 설정

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY); // IP 설정
	serv_adr.sin_port = htons(atoi(argv[1]));   // PORT 설정

	// 3. 서버의 주소값 bind()

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)

		error_handling("bind() error");

	// 4. listen() 동시접속 최대 허용수를 20으로 설정

	if(listen(serv_sock, 20)==-1)

		error_handling("listen() error");
	

	while(1)

	{

		clnt_adr_size=sizeof(clnt_adr);
		
		// 5. accept()

		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_size);

		printf("Connection Request : %s:%d\n", 

			inet_ntoa(clnt_adr.sin_addr), ntohs(clnt_adr.sin_port));

		// pthread 생성(thread식별자, thread특성, 분기시킬 thread 함수, 앞함수의 매개변수)	
		pthread_create(&t_id, NULL, request_handler, &clnt_sock);
		
		// 성공 0, 실패 0 이외값. thread종료되는 즉시 자원을 모두 되돌려 줄 것을 보증. 
		pthread_detach(t_id);

	}

	close(serv_sock);

	return 0;

}



void* request_handler(void *arg)
{
	int clnt_sock=*((int*)arg);
	char req_line[SMALL_BUF];
	FILE* clnt_read;
	FILE* clnt_write;

	char method[10];
	char ct[15];
	char file_name[30];

	// 스트림 분리 	

	clnt_read=fdopen(clnt_sock, "r");  // read fp 값 생성
	clnt_write=fdopen(dup(clnt_sock), "w");  // 복사해서 write fp 값 생성 
	fgets(req_line, SMALL_BUF, clnt_read);	// 열린 파일 스트림으로 부터 문자열 입력 받음. EOF or "\n" 만나면 읽기 멈춤. buf 마지막 다음 문자를 \0 으로 변경.

	if(strstr(req_line, "HTTP/")==NULL) // 문자열에서 임의의 문자열이 시작하는 위치를 포인터로 반
	{
		send_error(clnt_write);
		fclose(clnt_read);
		fclose(clnt_write);
		return;
	}

	printf("req_line:%s\n", req_line);
	strcpy(method, strtok(req_line, " /")); // " /"기준으로 자르고 method에 복사 
	
	print("method : %s\n", method);
	strcpy(file_name, strtok(NULL, " /")); // " /"기준으로 자르고 file_name에 복사

	printf("file_name : %s\n",file_name); 

	strcpy(ct, content_type(file_name)); // text/html(파일종류/포맷형식)

	if(strcmp(method, "GET")!=0)

	{

		send_error(clnt_write);

		fclose(clnt_read);

		fclose(clnt_write);

		return;

	 }

	fclose(clnt_read);

	send_data(clnt_write, ct, file_name); 

	printf("test\n");

}


//응답

void send_data(FILE* fp, char* ct, char* file_name)

{

	char protocol[]="HTTP/1.0 200 OK\r\n";

	char server[]="Server:Linux Web Server \r\n";

	char cnt_len[]="Content-length:2048\r\n";

	char cnt_type[SMALL_BUF];

	char buf[BUF_SIZE];

	FILE* send_file;

	

	sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);

	send_file=fopen(file_name, "r");

	if(send_file==NULL)
	{
		send_error(fp); 
		return;
	}


	/* 헤더 정보 전송 */

	fputs(protocol, fp);

	fputs(server, fp);

	fputs(cnt_len, fp);

	fputs(cnt_type, fp);



	/* 요청 데이터 전송 */

	while(fgets(buf, BUF_SIZE, send_file)!=NULL) 
	{
		fputs(buf, fp);

		fflush(fp);
	}

	fflush(fp);

	fclose(fp);

}



char* content_type(char* file)
{

	char extension[SMALL_BUF];

	char file_name[SMALL_BUF];

	strcpy(file_name, file);

	strtok(file_name, ".");

	strcpy(extension, strtok(NULL, "."));

	

	if(!strcmp(extension, "html")||!strcmp(extension, "htm")) 

		return "text/html";

	else

		return "text/plain";

}



void send_error(FILE* fp)

{	

	char protocol[]="HTTP/1.0 400 Bad Request\r\n";

	char server[]="Server:Linux Web Server \r\n";

	char cnt_len[]="Content-length:2048\r\n";

	char cnt_type[]="Content-type:text/html\r\n\r\n";

	char content[]="<html><head><title>NETWORK</title></head>"

	       "<body><font size=+5><br>오류 발생! 요청 파일명 및 요청 방식 확인!"

		   "</font></body></html>";



	fputs(protocol, fp);

	fputs(server, fp);

	fputs(cnt_len, fp);

	fputs(cnt_type, fp);

	fflush(fp);

}



void error_handling(char* message)

{

	fputs(message, stderr);

	fputc('\n', stderr);

	exit(1);

}
