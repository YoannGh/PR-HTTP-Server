#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <unistd.h>
#include <errno.h>
#include <regex.h>

#include "mime_parser.h"
#include "util.h"

int parse_file_ext(char* file_ext, char** type) {
	char* pattern;
	regex_t preg;
	char err_buf[ERR_BUFFER_SIZE];
	size_t maxGroups;
	regmatch_t *match;
	int res;
	char* mime_type_str;
	int len = 0;
	char* line;
	int fd;

	if ((fd = open (MIME_PATH, O_RDONLY, 0444)) < 0) {
#ifdef DEBUG
		perror("error opening mimes.types");
#endif
		return -1;
    }

	if(file_ext == NULL) {
		len = strlen(DEFAULT_MIME_TYPE);
		mime_type_str = (char *) malloc(len + 1);
		strcpy(mime_type_str, DEFAULT_MIME_TYPE);
		mime_type_str[len] = '\0';
		*type = mime_type_str;
		return 0;
	}

	if(file_ext[0] == '.') 
		file_ext++;

	/*
		(\S+)(.*)(\s)(html) => Match 4 groupes, match[1] = 1er groupe(mime_type)
	*/
	
	len = 14 + strlen(file_ext) + 1 + 1;
	pattern = (char *) malloc(len * sizeof(char));
	strcpy(pattern, "(\\S+)(.*)(\\s)(");
	strcat(pattern, file_ext);
	strcat(pattern, ")");
	pattern[len] = '\0';
#ifdef DEBUG
	printf("Pattern: %s\n", pattern);
#endif


	if( (res = regcomp(&preg, pattern, REG_ICASE|REG_EXTENDED|REG_NEWLINE)) != 0) {
#ifdef DEBUG
		regerror(res, &preg, err_buf, ERR_BUFFER_SIZE);
		printf("regcomp error: %s\n", err_buf);
#endif
		free(pattern);
		return -1;
	}

	maxGroups = preg.re_nsub + 1; /* Nb de groupe regex + 1 */
	match = (regmatch_t *) malloc (sizeof(regmatch_t) * maxGroups);

	res = REG_NOMATCH;
	while((res == REG_NOMATCH) && (readline(fd, &line) >= 0)) {

		if(line && (line[0] == '#' || line[0] == ' ' || line[0] == '\0')) {
			free(line);
			continue;
		}

		res = regexec(&preg, line, maxGroups, match, 0);

		if (res == 0) {
			len = match[1].rm_eo - match[1].rm_so;
	   		mime_type_str = (char *) malloc(len + 1);
	   		memcpy(mime_type_str, line + match[1].rm_so, len);
	   		mime_type_str[len] = '\0';
	   		*type = mime_type_str;
		}

		free(line);
	}

	if(res == REG_NOMATCH) { /* Si on sort du while sans avoir trouvé */

		len = strlen(DEFAULT_MIME_TYPE);
		mime_type_str = (char *) malloc(len + 1);
		strcpy(mime_type_str, DEFAULT_MIME_TYPE);
		mime_type_str[len] = '\0';
		*type = mime_type_str;
	}

	regfree(&preg);
	free(pattern);
	free(match);

	close(fd);

	return 0;

}

/* int main(int argc, char* argv[]) {
	mime_parser mp;
	char * mime_type_str;
	char* exts[] = {"html", "css", "js", "png", "jpeg", "mp3", "mp4", "webm", "gif", "doc", "html", "css", "js", "png", "jpeg", "mp3", "mp4", "webm", "gif", "doc", "html", "css", "js", "png", "jpeg", "mp3", "mp4", "webm", "gif", "doc", "html", "css", "js", "png", "jpeg", "mp3", "mp4", "webm", "gif", "doc", "html", "css", "js", "png", "jpeg", "mp3", "mp4", "webm", "gif", "doc", "html", "css", "js", "png", "jpeg", "mp3", "mp4", "webm", "gif", "doc"};
	int i;

	if(argc != 2) {
		printf("usage: %s <file_ext>\n", argv[0]);
		return 0;
	}

	mime_parser_init(&mp);

	for(i = 0; i < 60; i++) {
		parse_file_ext(&mp, exts[i], &mime_type_str);
		printf("%d: %s Mime_type: '%s'\n", i, exts[i], mime_type_str);
		free(mime_type_str);
	}

	mime_parser_destroy(&mp);

	return 0;
} */