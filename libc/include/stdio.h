#ifndef _STDIO_H
#define _STDIO_H 1

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char *, ...);
int putchar(int);
int puts(const char *);
int getchar();
char *gets(char *s);

#ifdef __cplusplus
}
#endif

#endif
