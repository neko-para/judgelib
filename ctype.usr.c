#include "ctype.h"

int isdigit(int ch) {
	return (unsigned)(ch - '0') < 10;
}