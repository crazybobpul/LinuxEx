#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//SIGINT에 대응하는 핸들러
void sigHandler(int sig)
{	
	static int count=0;
	
	switch (sig)
	{
		case SIGINT: printf("OUCH! - Igot signal %d\n", sig);
			//(void)signal(SIGINT, SIG_DFL); //한번 실행 후 default로 
			break;
		
		case SIGQUIT: printf("roger! - quit quit quit %d\n", sig);
                        //(void)signal(SIGQUIT, SIG_DFL);
			break;	
	}
	count++;
	if(count==5)
		exit(0);

}

int main()
{
	signal(SIGINT, sigHandler);
	signal(SIGQUIT, sigHandler);

	while(1)
	{
		printf("Hello World!\n");
		sleep(1);
		fflush(stdout);
	}
return 0;
}


