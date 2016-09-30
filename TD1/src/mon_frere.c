#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv) {
 	int p1, p2, p3= 0;

 	if((p1 = fork()) == -1) {
 		perror("Erreur fork principal");
 	} 

	
 	else if (p1 != 0) {
 		if((p2 = fork()) == -1)
 			perror("p2");

 		if((p1 != 0) && (p2 == 0))
 			printf("PID GRAND FRERE: %d\n", p1);

 	}
	if( p1 == 0 || p2 == 0)
	{
 		if((p3 = fork()) == -1)
 			perror("petitfils");

 		printf("PID: %d PPID:%d\n", getpid(), getppid());
	}

	if(p3 != 0)
		wait(NULL);

	if(p1 != 0 && p2 != 0)
		wait(NULL);



	return EXIT_SUCCESS;

}