#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

static long result=0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;	


struct Num
{
	int start, end;
};

void* add_func1(void *data)
{
	struct Num num1;
	int i;

	num1 = *((struct Num*)data);

	printf("num1.start = %d\n", num1.start);
	printf("num1.end = %d\n", num1.end);
	
	pthread_mutex_lock(&mutex); //잠금 생성

	for(i=num1.start; i<num1.end+1; i++)
	{
		result +=i;
	}
	pthread_mutex_unlock(&mutex); //잠금 해제	

	printf("result1 : %ld\n", result);
	return (void*)(result);

	//pthread_mutex_unlock(&mutex); //잠금 해제

}

void* add_func2(void *data)
{
	struct Num num2;
	int i;

	num2 = *((struct Num*)data);
	
	printf("num2.start = %d\n", num2.start);
	printf("num2.end = %d\n", num2.end);//

	pthread_mutex_lock(&mutex); //잠금 생성
	
	for(i=num2.start; i<=num2.end; i++)
	{
		result +=i;
	}
	
	pthread_mutex_unlock(&mutex); //잠금 해제

	printf("result2 : %ld\n", result);
	return (void*)(result);

	//pthread_mutex_unlock(&mutex); //잠금 해제
}	

int main()
{
	int pth_id;	
	pthread_t p_thread[2];
	struct Num num1, num2;
	num1.start = 1;	
	num2.end = 20000;
	num1.end = num2.end/2;
	num2.start = num1.end+1;
	
	long sum1,sum2;
	
	pth_id = pthread_create(&p_thread[0], NULL, add_func1, (void*)&num1);
	sleep(1);
	pth_id = pthread_create(&p_thread[1], NULL, add_func2, (void*)&num2);

	pthread_join(p_thread[0], (void**) &sum1);
	pthread_join(p_thread[1], (void**) &sum2);
	
	result = pthread_mutex_destroy(&mutex);
	printf("Sum = %ld\n", sum2);
	
	return 0;
}
	
	
