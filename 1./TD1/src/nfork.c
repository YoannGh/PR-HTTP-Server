#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int nfork(int n) {
	int i;
	int ret;
	int cpt = 0;
	for(i = 0; i < n; i++) {
		ret = fork();
		if(ret == -1) {
			perror("Erreur fork");
			break;
		}
		else if(ret == 0) {
			//printf("fils : ");
			return 0;
		}
		else
			cpt++;
	}
	//printf("pere : ");
	if(cpt == 0)
		return -1;
	return cpt;
}

// int main(int argc, char **argv) {
// 	printf("%d\n", nfork(atoi(argv[1])));
// }
