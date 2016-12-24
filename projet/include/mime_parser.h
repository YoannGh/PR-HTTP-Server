#define MIME_PATH "/etc/mime.types"
#define ERR_BUFFER_SIZE 1024
#define MIME_CACHE_SIZE 16

typedef struct mime_cache_entry {
	char* ext;
	char* mime_type;
} mime_cache_entry;

typedef struct mime_parser {
	int fd;
	char* buffer;
	mime_cache_entry* cache;
	int cache_index;
} mime_parser;

int mime_parser_init(mime_parser* mp);

char* parse_file_ext(mime_parser* mp, char* file_ext);

int mime_parser_destroy(mime_parser* mp);