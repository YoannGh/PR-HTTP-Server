#define _XOPEN_SOURCE 700
#define _REENTRANT
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "lib_upper.h"

void* upper(void *path)
{
    char* p = path;
    FILE* fp1, *fp2;
    int c = 1;

    fp1= fopen (p, "r");
    fp2= fopen (p, "r+");
    if ((fp1== NULL) || (fp2== NULL)) {
        perror (p);
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
