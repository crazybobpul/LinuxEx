#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>



void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid=waitpid(-1, &status, WNOHANG);
	printf("removed proc id : %d\n", pid);
}


int main(int argc, char *argv[])
{
	int shmid;
	pid_t pid;
	int state;

	int *shmaddr;
	void *shared_memory = (void*)0;
		

	struct sigaction act;

	int i;
	int num1_start = 1;
	int num2_end = 20000;	
	int num1_end = (num2_end>>1);
	int num2_start = (num1_end+1);
	
	
	//long sum; 


	// 시그널 및 핸들러 등록
	act.sa_handler=read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	state = sigaction(SIGCHLD, &act, 0);
	
	pid = fork();


	if(pid==-1)
	printf("error!\n");

		
	// 자식 프로세스의 경우(자식 프로세스는 생성)
	


	else if(pid==0)
	{
		int result1;

		shmid = shmget((key_t)1234, sizeof(int)*100, 0666|IPC_CREAT);

		shared_memory = shmat(shmid, (void *)0, 0);

		shmaddr = (int*)shared_memory;
          
		for(i=num1_start; i<=num1_end; i++)
		{
			result1=result1+i;
		}
		printf("result = %d\n",result1);
		
		*shmaddr = result1; 

		shmdt(shared_memory);

		exit(0);
	}
	
	// 부모 프로세스인 경우
	else
	{
		sleep(2);

		int result2;
  
		shmid = shmget((key_t)1234, sizeof(int)*100, 0666|IPC_CREAT);

		shared_memory = shmat(shmid, (void *)0, 0);
		
		shmaddr = (int*)shared_memory;
		
		//printf("child sum = %ld\n", ); 
		
		result2=*shmaddr;

		for(i=num2_start; i<=num2_end; i++)
		{
			result2=result2+i;	
		}
		
		printf("sum = %d\n", result2);


		//step4.shmdt
        	if(shmdt(shared_memory)==-1)
        	{
        	        fprintf(stderr,"shmdt failed\n");
        	        exit(EXIT_FAILURE);
        	}
        	//step5.shmclt
        	if(shmctl(shmid,IPC_RMID,0)==-1)
        	{
        	        fprintf(stderr,"shmctl(IPC_RMID) failed\n");
        	        exit(EXIT_FAILURE);
        	}
 
	}
	
	
	return 0;
}		


