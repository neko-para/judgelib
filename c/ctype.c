#include "ctype.h"

#define __RANGE__(a, b, c) ((unsigned)(a - b) <= (c - b))

int iscntrl(int ch) {
	return __RANGE__(ch, 0, 0x1F) || ch == 0x7F;
}

int isspace(int ch) {
	return __RANGE__(ch, '\t', '\r') || ch == ' ';
}

int isupper(int ch) {
	return __RANGE__(ch, 'A', 'Z');
}

int islower(int ch) {
	return __RANGE__(ch, 'a', 'z');
}

int isalpha(int ch) {
	return isupper(ch) || islower(ch);
}

int isdigit(int ch) {
	return __RANGE__(ch, '0', '9');
}

int isxdigit(int ch) {
	return isdigit(ch) || __RANGE__(ch, 'a', 'f') || __RANGE__(ch, 'A', 'F');
}

int isalnum(int ch) {
	return isalpha(ch) || isdigit(ch);
}

int ispunct(int ch) {
	return __RANGE__(ch, '!', '/') || __RANGE__(ch, ':', '@') || __RANGE__(ch, '[', '`') || __RANGE__(ch, '{', '~');
}

int isgraph(int ch) {
	return __RANGE__(ch, '!', '~');
}

int isprint(int ch) {
	return __RANGE__(ch, ' ', '~');
}

int tolower(int ch) {
	return isupper(ch) ? ch + ('a' - 'A') : ch;
}

int toupper(int ch) {
	return islower(ch) ? ch + ('A' - 'a') : ch;
}