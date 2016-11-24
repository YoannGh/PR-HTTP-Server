#define MAX_LENGTH 256

struct env_message {
	char type;
	char identificateur[MAX_LENGTH];
	char valeur[MAX_LENGTH];
};