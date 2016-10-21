#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "lib_upper.h"

void* upperDeleg(void* arg) {
	char* path = (char *) arg;
	int *ret = malloc(sizeof(int));
	*ret = upper(path);
	pthread_exit(ret);
}

int main(int argc, char* argv[]) {

	pthread_t* tids;
	int nb_thread;
	int i, errorCount;
	int *threadret;
	char* arg;

	errorCount = 0;

	if(argc < 2)
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	nb_thread = argc -1;
	tids = malloc(sizeof(pthread_t)*(nb_thread));

	for(i = 0; i < (nb_thread); i++) {

		arg = argv[i+1];
		if(pthread_create(&tids[i], NULL, upperDeleg, arg) != 0) {
			perror("error p_create");
			return EXIT_FAILURE;
		}
	}

	for(i = 0; i < (nb_thread); i++) {
		if((pthread_join(tids[i], (void**) &threadret) != 0) || !threadret) {
			printf("error p_join on file %s", argv[i+1]);
			errorCount++;
		}
	}

	free(tids);
	free(threadret);

	return errorCount;

}