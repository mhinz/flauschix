#pragma once

#include <stdint.h>

char  *strcat(char *dest, const char *src);
char  *strchr(char *s, const int c);
char  *strcpy(char *dest, const char *src);
char  *strdup(const char *s);
char  *strncat(char *dest, const char *src, size_t n);
char  *strncpy(char *dest, const char *src, size_t n);
char  *strrchr(char *s, const int c);
int    strcmp(const char *s1, const char *s2);
int    strncmp(const char *s1, const char *s2, size_t n);
size_t strlen(char *s);
