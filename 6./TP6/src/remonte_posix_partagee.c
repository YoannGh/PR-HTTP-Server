#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>


/* Demonde un descripteur de ressource partagee, eventuellement deja creee. */
#define DESC_SHM(id, path) id = shm_open(path, O_RDWR|O_CREAT, 0600)

/* Controle du retour du descripteur */
#define CTRL_DESC(id,n) printf("d:%d %d\n", ftruncate(id, n*sizeof(int)), id)

/* Allocation */
#define ALLOC_SHM(ptr, id, n) ptr = mmap(NULL, n*sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED, id, 0)


/* Controle du retour de l'allocation */
#define CTRL_SHM(ptr) ptr == NULL
/* Ecriture dans la ressource partagee */
#define SET_SHM(ptr, id, i, rand) *(ptr+i) = rand

/* Lecture dans la ressource partagee */
#define GET_SHM(res, ptr, i, id) res = *(ptr+i)

/* Liberation de la ressource */
#define FREE_SHM(id, ptr, n, path) !(munmap(ptr, n*sizeof(int)) || shm_unlink(path))

#include "remonte.c"