void* thread_func(void* arg) {
	
  int i, nb;
  int *param;
  int *lvl = (int*)arg;
  pthread_t *tid;
	
  nb = (*lvl)+1;
	
  if (*lvl < max) {
    param = (int*)malloc(sizeof(int));
    *param = nb;
    tid = calloc(nb, sizeof(pthread_t));
    printf("%d cree %d fils\n", (int)pthread_self(), nb);
    for (i = 0; i < nb; i++) {
      pthread_create((tid+i), 0, thread_func, param);
    }
    for (i = 0; i < nb; i++)
      pthread_join(tid[i], NULL);
  }

  if (*lvl > 1)
    pthread_exit ( (void*)0);
	
  return (void*)0;
}
