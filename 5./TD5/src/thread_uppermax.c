#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "lib_upper.h"


int main(int argc, char* argv[]) {

	pthread_t* tids;
	int nb_thread, nb_file;
	int i, errorCount;
	char* arg;

	errorCount = 0;

	if(argc < 3)
	{
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	nb_thread = atoi(argv[1]);
	nb_file = argc -2;

	if( (nb_thread >= nb_file) || (nb_thread <= 0) )
	{
		printf("Wrong number of threads asked\n");
		return EXIT_FAILURE;
	}

	tids = malloc(sizeof(pthread_t)*(nb_thread));

	for(i = 0; i < (nb_thread); i++) 
	{
		arg = argv[i+2];
		if(pthread_create(&tids[i], NULL, upper, arg) != 0) {
			perror("error p_create");
			return EXIT_FAILURE;
		}
	}

	for(i = 0; i < nb_file; i++) 
	{
		if(pthread_join(tids[i%nb_thread], NULL) != 0) 
		{
			printf("error p_join on file %s", argv[i+1]);
			errorCount++;
		}
		else if(i + nb_thread < nb_file)
		{
			arg = argv[i+2+nb_thread];
			if(pthread_create(&tids[i%nb_thread], NULL, upper, arg) != 0) 
			{
				perror("error p_create");
				return EXIT_FAILURE;
			}			
		}
	}

	free(tids);

	return errorCount;

}