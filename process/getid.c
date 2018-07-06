#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

void pr_exit(int status);

int main(void)
{
	int status;
	pid_t pid;	

	//자신의 PID값을 확인한다
	printf("process ID =%d\n", getpid());
	//
	printf("parent process ID=%d\n", getppid());
	sleep(10);

	pid = fork();

	//fork()를 실행했는데 정상적으로 자식프로세스가 생성되지 않은 경우
	if(pid<0)
	{
		printf("Error:fork()\n");
		return -1;	
	}
	//자식 프로세스인 경우 실행
	else if(pid==0)
	{
		printf("Child:Hello!!\n");
		raise(SIGINT);			
		//return 0;
	}
	//부모 프로세스인 경우 실행
	else
	{
		printf("Parent:Hi!!\n");
		printf("Child PID : %d\n",pid);
	}

	pid = wait(&status);
	printf("EXIT:Child pid=%d\n",pid);

	pr_exit(status);
	return 0;

}


void pr_exit(int status)
{
	//자식 프로세스가 정상적으로 종료되었다면,
	if(WIFEXITED(status))
		printf("normal termination, exit status=%d\n", WEXITSTATUS(status));
	

	//자식 프로세스가 시그널에 의해 종료되었다면,
	else if(WIFSIGNALED(status))
		printf("abnormal termination, signal number=%d%s\n", 
			WTERMSIG(status), WCOREDUMP(status) ? "(core file generated)" : "");
	
	//자식 프로세스가 정지된 상태라면,
	else if(WIFSTOPPED(status))
		printf("child stopped. signal number=%d\n", WSTOPSIG(status));
} 

