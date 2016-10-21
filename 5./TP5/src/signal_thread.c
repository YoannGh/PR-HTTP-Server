#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <signal.h>



pthread_cond_t condAllCreated = PTHREAD_COND_INITIALIZER;
pthread_cond_t condWaitSignalMain = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex_number_thread_created = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_cond = PTHREAD_MUTEX_INITIALIZER;
int number_thread_created = 0;

void sig_hand(int sig)
{
}

void* chainedThread(void* arg)
{
	int* nb = (int*)arg;
	pthread_t tid = 0;
	pthread_mutex_lock(&mutex_number_thread_created);
	number_thread_created++;
	if(number_thread_created<(*nb))
	{

		if(pthread_create(&tid, NULL, chainedThread, arg) != 0) {
			perror("error p_create");
			pthread_exit(NULL);
		}
	}	
	else
	{
		pthread_cond_signal(&condAllCreated);
	}

	pthread_mutex_unlock(&mutex_number_thread_created);
	printf("Attente SIGINT du main\n");
	pthread_cond_wait(&condWaitSignalMain, &mutex_cond);
	pthread_mutex_unlock(&mutex_cond);

	if(tid)
	{	
		printf("inif\n");
		if(pthread_join(tid, NULL) != 0) {
				printf("error p_join");
				pthread_exit(NULL);
			}
			printf("Joined\n");
	}

	printf("out\n");
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {

	pthread_t tid;
	int nb_thread;
	sigset_t sig_proc, sig_set;
	struct sigaction action;

	if(argc != 2)
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	nb_thread = atoi(argv[1]);

	if( nb_thread <= 0 )
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}


	sigfillset(&sig_set);
	sigprocmask(SIG_BLOCK, &sig_set, NULL);
	

	if(pthread_create(&tid, NULL, chainedThread, &nb_thread) != 0) {
			perror("error p_create cons");
			return EXIT_FAILURE;
	}

	pthread_mutex_lock(&mutex_number_thread_created);
	while(nb_thread != number_thread_created)
	{	
		pthread_cond_wait(&condAllCreated, &mutex_number_thread_created);
	}
	pthread_mutex_unlock(&mutex_number_thread_created);

	sigemptyset(&sig_proc);
	action.sa_mask = sig_proc;
	action.sa_flags = 0;
	action.sa_handler = sig_hand;
	sigaction(SIGINT, &action, NULL);

	printf("Tous mes descendants sont crees\n");

	sigfillset(&sig_set);
	sigdelset(&sig_set, SIGINT);

	printf("Attente du sigint\n");
	sigsuspend(&sig_set);

	printf("SINGINT RECU\n");
	pthread_cond_broadcast(&condWaitSignalMain);

	if(pthread_join(tid, NULL) != 0) {
		printf("error p_join cons");
		return EXIT_FAILURE;
	}

	printf("Tous mes descendants se sont terminés\n");

	return EXIT_SUCCESS;
}