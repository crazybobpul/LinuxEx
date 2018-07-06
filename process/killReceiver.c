#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void sigHandler(int sig)
{
	static int count=0;
	
	printf("killTest:I got signal%d\n",sig);
	
	count++;
	if(count==5)
	{
		(void)signal(SIGINT, SIG_DFL);
		exit(0);
	}
}

int main(void)
{
	signal(SIGINT,sigHandler);
	while(1)
	{
		printf("Hello World!\n");
		sleep(1);
	}
}


