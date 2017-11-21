#ifndef _CTYPE_H_
#define _CTYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

int iscntrl(int ch);
int isspace(int ch);
int isupper(int ch);
int islower(int ch);
int isalpha(int ch);
int isdigit(int ch);
int isxdigit(int ch);
int isalnum(int ch);
int ispunct(int ch);
int isgraph(int ch);
int isprint(int ch);

int tolower(int ch);
int toupper(int ch);

#ifdef __cplusplus
}
#endif

#endif