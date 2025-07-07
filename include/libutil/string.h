#ifndef OS_RTA_LIBUTIL_STRING_H
#define OS_RTA_LIBUTIL_STRING_H

#include <types.h>

void *memset(void *s, int c, word n);
void *memcpy(void *dest, const void *src, word n);
void *memmove(void *dest, const void *src, word n);
word  strlen(const char *s);
int   strcmp(const char *s1, const char *s2);
int   strncmp(const char *s1, const char *s2, word n);

#endif
