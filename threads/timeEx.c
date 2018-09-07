#include <stdio.h>
#include <time.h>
#include <sys/time.h>    //gettimeofday()함수에서 사용
#include <stdlib.h>


int main(int argc, char *argv[])
{
	time_t UTCtime;
	struct tm *tm;
	char buf[BUFSIZ];
	struct timeval UTCtime_u;

	//1. time()
	time(&UTCtime);
	printf("time : %u\n", (unsigned)UTCtime); //UTC 현재시간 출력

	tm = localtime(&UTCtime);
	printf("year : %d\n",tm->tm_year);

	printf("asctime : %s\n", asctime(tm));// 현재 시간을 tm구조체로 읽어옴
	//printf("time : %d

	
	//2. gettimeofday()
	gettimeofday(&UTCtime_u, NULL); //UTC 현재 시간(마이크로단위)
	printf("gettimeofday : %ld/%ld\n", UTCtime_u.tv_sec, UTCtime_u.tv_usec);

return 0;
}
