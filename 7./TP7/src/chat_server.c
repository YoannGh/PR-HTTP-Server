#include "chat_common.h"

void sendMessage(char* content, char* client)
{
	int sharedfd;
	struct myshm shm*;

	sharedfd = shm_open(argv[1], O_CREAT|O_RDWR, 0600);
	ftruncate(sharedfd, sizeof(struct myshm));
	shm = (sem_t*) mmap(NULL, sizeof(struct myshm), PROT_READ|PROT_WRITE, MAP_SHARED, sharedfd, 0);
}

int main(int argc, char* argv[]) {
	int sharedfd, i, j;
	struct myshm shm*;
	struct message msg*;
	char clients[MAX_USERS][1024];

	if(argc != 2) {
		printf("Bad args\n");
		return EXIT_FAILURE;
	}

	sharedfd = shm_open(argv[1], O_CREAT|O_RDWR, 0600);
	ftruncate(sharedfd, sizeof(struct myshm));
	shm = (sem_t*) mmap(NULL, sizeof(struct myshm), PROT_READ|PROT_WRITE, MAP_SHARED, sharedfd, 0);
	if (shm == NULL)
    {
        perror("In mmap()");
        return EXIT_FAILURE;
    }

    shm->read = 0;
    shm->write = 0;
    shm->nb = 0;
    sem_init(&shm->sem, 1, 1);

    nbclients = 0;

    while(1) {
    	sem_wait(&shm->sem);
    	for(i = 0; i<shm->write; i++)
    	{
    		msg = messages[i];
    		switch(msg->type)
    		{
	    		case 1:
	    			printf("Nouvelle connexion: %s\n", content);
	    			for(j = 0; j<MAX_USERS; j++)
	    				if(clients[j] == NULL)
	    					strcpy(clients[j], content);
	    			break;
	    		case 2:
	    			printf("Nouveau Message: %s\n", content);
	    			for(j = 0; j<MAX_USERS; j++)
	    				if(clients[j] != NULL)
	    					sendMessage(content, clients[j]);
	    			break;
	    		case 3:
	    			printf("Deconnexion: %s\n", content);
	    			for(j = 0; j<MAX_USERS; j++)
	    				if(strcmp(clients[j], content) == 0)
	    					clients[j] = NULL;

	    			break;
	    		default:
	    			printf("ERROR MESSAGE TYPE\n");
	    			break;
	    	}

    	}

    	sem_post(&shm->sem);
    }

    munmap(shm, sizeof(struct myshm));
	shm_unlink(argv[1]);

	return EXIT_SUCCESS;
}