#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm_com.h"


int main()
{
	int running = 1;
	void *shared_memory = (void*)0;
	struct shared_use_st *shared_stuff;
	int shmid;

	srand((unsigned int)getpid());

	shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	//key, 공유 메모리의 최소 크기 (존재하는 메모리라면 0), 접근 권한과 생성방식
	
	if(shmid == -1)
	{
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	shared_memory = shmat(shmid,(void*)0, 0); 
	// shmget을 통해 얻어낸 식별자 번호, 메모리가 붙을 주소(0은 커널이 명시함), 모드(지정하지 않으면 읽기 쓰기 가능 모드

	if(shared_memory == (void*)-1)
	{
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}

	printf("Memory attached at %p\n", (int*)shared_memory);

	shared_stuff = (struct shared_use_st*)shared_memory;
	shared_stuff -> written_by_you = 0;

	while(running) 
	{
		if(shared_stuff -> written_by_you)
		{
			printf("You wrote : %s", shared_stuff -> some_text);
			sleep(rand()%4); // ??
			shared_stuff -> written_by_you = 0;
			
			if(strncmp(shared_stuff -> some_text, "end", 3) == 0)
			{
				running = 0;
			}
		}
	}	

	if(shmdt(shared_memory) == -1)//shmid_ds (detatched)내용 업데이트. 아직 삭제된 것은 아님. 
	{
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}

	if(shmctl(shmid,IPC_RMID, 0) == -1)// 공유메모리 제어(모드 변경, 삭제, 잠금 설정 및 해제
	{
		fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_SUCCESS);
}

