#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <linux/limits.h>

int main(int argc, char const *argv[])
{
	struct stat rep;
	struct stat stfile;
	DIR *d;
  	struct dirent *current_filename;
  	int current_fd;
  	char* filebuffer;
  	int cpt = 0;
  	char* path;

  	path = malloc(PATH_MAX);

	if (argc != 3)
	{
		printf("Error args\n");
		return EXIT_FAILURE;
	} 

	if(stat(argv[2], &rep) != 0) {
		printf("Impossible d'ouvrir le repertoire %s\n", argv[2]);
		perror("rep");
		return errno;
	}

	if(!(rep.st_mode & S_IFDIR))
	{
		printf("Vous devez specifier un repertoire\n");
		return EXIT_FAILURE;
	}

	
  	d = opendir(argv[2]);
  	if (d)
  	{
    	while ((current_filename = readdir(d)) != NULL)
    	{

    		path = malloc(PATH_MAX);
    		strcpy(path, argv[2]);
    		strcat(path, "/");
    		strcat(path, current_filename->d_name);

      		if(stat(path, &stfile) != 0) {
				printf("Impossible d'ouvrir le fichier %s\n", path);
				perror("file1");
				return errno;
			}

      		if(!(stfile.st_mode & S_IFDIR))
			{
				if ((current_fd = open(path, O_RDONLY)) == -1) {
      				perror("open fd");
      				return errno;
    			}
    		
				filebuffer = malloc(stfile.st_size);

				if (read(current_fd, filebuffer, stfile.st_size) == -1) {
            		perror("read fd");
            		return errno;
          		}

	          	if(strstr(filebuffer, argv[1]) != NULL) {
	          		cpt++;
	          		printf("Trouvé dans %s\n", path);
	          	}

	          	free(filebuffer);
	          	free(path);
	          	close(current_fd);

			}
		}
	}

   	closedir(d);

  	printf("%d fichiers trouves\n", cpt);

	return EXIT_SUCCESS;
}