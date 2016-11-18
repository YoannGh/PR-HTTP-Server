#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int process(int id, int NB_PCS) {
	sem_t* map;
	int sharedfd;
	sharedfd = shm_open("blabla", O_CREAT|O_RDWR, 0600);
	ftruncate(sharedfd, sizeof(sem_t));
	map = (sem_t*) mmap(NULL, sizeof(sem_t), PROT_READ|PROT_WRITE, MAP_SHARED, sharedfd, 0);
	if (map == NULL)
    {
        perror("In mmap()");
        return EXIT_FAILURE;
    }
	
    sleep(NB_PCS % (id+1));
    if(id > 0)
    	sem_wait(&map[id-1]);
   

    printf("reveil %d, pid: %d\n", id, getpid());

    if(id < NB_PCS)
    	sem_post(&map[id]);

	exit(0);
}

int main(int argc, char* argv[]) {
	int NB_PCS;
	int i, pid, sharedfd;
	sem_t mutex;
	sem_t* map;

	if(argc != 2) {
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	NB_PCS = atoi(argv[1]);

	sharedfd = shm_open("blabla", O_CREAT|O_RDWR, 0600);
	ftruncate(sharedfd, sizeof(sem_t)*(NB_PCS-1));
	map = (sem_t*) mmap(NULL, sizeof(sem_t)*(NB_PCS-1), PROT_READ|PROT_WRITE, MAP_SHARED, sharedfd, 0);
	if (map == NULL)
    {
        perror("In mmap()");
        return EXIT_FAILURE;
    }

	for(i = 0; i < NB_PCS-1; i++) {
		sem_init(&map[i], 1, 0);
	}

	for(i = 0; i < NB_PCS; i++) {
		if((pid = fork()) == -1) {
			perror("fork");
			return EXIT_FAILURE;
		}
		else if (pid == 0) {
			process(i, NB_PCS);
			break;
		}
	}


	for(i = 0; i < NB_PCS; i++) {
		wait(NULL);
	}

	puts("fin main");

	munmap(map, sizeof(sem_t));
	shm_unlink("blabla");
	sem_destroy(&mutex);

	return EXIT_SUCCESS;
}