#include <stdio.h>

char* readable_filesize(double size, char *buf) {
    int i = 0;
    const char* units[] = {"o", "Ko", "Mo", "Go", "To", "Po", "Eo", "Zo", "Yo"};
    while (size > 1024) {
        size /= 1024;
        i++;
    }
    sprintf(buf, "%.*f%s", i, size, units[i]);
    return buf;
}