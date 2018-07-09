#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/my_fifo"

int main(int argc, char *argv[])
{
	int res;
	int open_mode=0;
	int i;

	if(argc<2)
	{
		fprintf(stderr, "Usage:%s<some combination of, O_RDONLY | O_WRONLY | O_NONBLOCK>\n", *argv);
		exit(EXIT_FAILURE);
	}
	
	for(i=1;i<argc;i++)
	{
		if(strncmp(*++argv,"O_RDONLY",8)==0) //*argv로 입력받은 문자와 비교.
			open_mode |=O_RDONLY;
		if(strncmp(*argv,"O_WRONLY",8)==0)
			open_mode |=O_WRONLY;
		if(strncmp(*argv,"O_NONBLOCK",10)==0)
			open_mode |=O_NONBLOCK;
	}
	
	if(access(FIFO_NAME,F_OK)==-1) //파일이 존재 하는지 여부. 파일이 없다면,
	{
		res=mkfifo(FIFO_NAME,0777); //파일을 만들고
 
		if(res!=0) // 파일이 제대로 생성되지 않으면,
		{
			fprintf(stderr,"Could not create fifo %s\n",FIFO_NAME);
			exit(EXIT_FAILURE);
		}
	}
	
	printf("Process %d opening FIFO\n",getpid()); 
	res=open(FIFO_NAME, open_mode);//위에서 입력된 오픈모드로 파일 오픈, 새 fd 넣어줌. 자식 생김.
	
	printf("Process %d result %d\n", getpid(),res);//
	sleep(5);

	if(res!=-1)
		(void)close(res);

	printf("Process %d finishied\n",getpid());
	exit(EXIT_SUCCESS);
}



/*mkfifo : pathname 이름을 가지며 mode 의 권한을 가지는 FIFO 파일을 만들어낸다. 주로 IPC 용도로 사용된다. FIFO 파일은 pipe 와 달리 이름있는 파일을 통해서 통신을 할수 있게 함. 일단 FIFO 파일이 만들어지면 open, write, read 등의 표준 함수를 이용해서 보통의 파일처럼 접근이 가능. FIFO 는 First In First Out 먼저들어온 데이타가 먼저 나가는 queue 방식의 입/출력을 지원한다.성공할 경우 0, 실패할 경우에는 -1 리턴. */
