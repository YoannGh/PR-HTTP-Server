#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(void) {
  pid_t pid;
  int  j=0; int i = 0;
  while (i < 2) {
    i ++;
    if ((pid = fork ()) == -1) {
      perror("fork");
      return errno;
    }
    else if (pid == 0) j=i;
  }
  if (j == 2) {
    char *const args[] = {"sleep", "2", NULL};
    execv("/bin/sleep", args);
    printf ("sans fils \n");
  }
  else {
    printf ("%d fils\n", (i-j) );
    while (j < i) {
      j++;
      wait (NULL);
    }
  }
  return EXIT_SUCCESS;
}  
