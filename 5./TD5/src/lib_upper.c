#define _XOPEN_SOURCE 700
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "lib_upper.h"

int upper(char *path)
{
    FILE* fp1, *fp2;
    int c = 1;

    fp1= fopen (path, "r");
    fp2= fopen (path, "r+");
    if ((fp1== NULL) || (fp2== NULL)) {
        perror (path);
	   return errno;
    }

    while (c !=EOF) {
        c=fgetc(fp1);
        if (c!=EOF)
        fputc(toupper(c),fp2);
    }

    fclose (fp1);
    fclose (fp2);

    return EXIT_SUCCESS;
}
