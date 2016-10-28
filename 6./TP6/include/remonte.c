int main(int argc, char *argv[]){

  int id, n, i, total, res;
  int *ptr = NULL;
  int *pids;

  n = (argc < 2) ? 0 : strtol(argv[1], NULL, 10);
  if (n <= 0) {
    fprintf(stderr, "Usage: %s nombre\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* Demonde un descripteur de ressource partagee, eventuellement deja creee. */
  DESC_SHM(id, argv[0]);

  /* Controle du retour du descripteur */
  if (CTRL_DESC(id, n)) {
    perror("Echec de l'allocation du descripteur\n");
    exit(errno);
  }

  /* Allocation */
  ALLOC_SHM(ptr, id, n);

  /* Controle du retour de l'allocation */
  if (CTRL_SHM(ptr)) {
    fprintf(stderr, "Echec de l'allocation de la ressource partagee\n");
    exit(errno);
  }

  pids = malloc(n * sizeof(int));

  for(i=0; i<n; i++){
    int pid = fork();
    if (pid == -1){
      perror("fork");
      return -1;
    } else if (pid) {
      pids[i] = pid;
    } else {
      srand(time(NULL)*i);
      /* Ecriture dans la ressource partagee */
      SET_SHM(ptr, id, i, (int) (10*(float)rand()/ RAND_MAX));
      exit(EXIT_SUCCESS);
    }
  }
  for(i=0; i<n; i++){
    int status;
    waitpid(pids[i], &status, 0);
  }

  total = 0;

  for(i=0; i<n; i++){
    /* Lecture dans la ressource partagee */
    GET_SHM(res, ptr,i, id);
    printf("pid %d envoie %d\n", pids[i], res);
    total += res;
  }

  free(pids);

  printf("total: %d\n", total);

  /* Liberation de la ressource */
  if (FREE_SHM(id, ptr, n, argv[0])) {
    fprintf(stderr, "Ressource partagee mal rendue\n");
    exit(EXIT_FAILURE);
  }
  return EXIT_SUCCESS;
}
