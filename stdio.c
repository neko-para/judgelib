#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "ctype.h"
#include "math.h"
#include "syscall.h"

int fflush(struct FILE* stream) {
	return syscall(__NR_fsync, (long)stream, 0, 0);
}

size_t fread(void* ptr, size_t size, size_t nmemb, struct FILE* stream) {
	size_t i = 0;
	for (i = 0; i < nmemb; ++i) {
		if (size != syscall(__NR_read, (long)stream, (long)ptr, size)) {
			break;
		} else {
			ptr += size;
		}
	}
	return i;
}

size_t fwrite(const void* ptr, size_t size, size_t nmemb, struct FILE* stream) {
	size_t i = 0;
	for (i = 0; i < nmemb; ++i) {
		if (size != syscall(__NR_write, (long)stream, (long)ptr, size)) {
			break;
		} else {
			ptr += size;
		}
	}
	return i;
}

static int printf_nformat;
static struct FILE* fprintf_stream;
static char* sprintf_ptr;

static void _print_file(char c) {
	fputc(c, fprintf_stream);
	++printf_nformat;
}

static void _print_str(char c) {
	*sprintf_ptr++ = c;
	++printf_nformat;
}

static void _reverse(char* beg, char* end) {
	--end;
	while (beg < end) {
		char k = *beg;
		*beg++ = *end;
		*end-- = k;
	}
}

static int _printf(void (*func)(char), const char* format, va_list ap) {
	int i;
	printf_nformat = 0;
	while (*format) {
		switch(*format) {
		case '%':
			++format;
			if (*format == '%') {
				++format;
				func('%');
				break;
			}
			do {
				int left_justify = 0;
				int sign_type = 0; /* 0: default 1: always 2: blank */
				int prefix_suffix = 0;
				char fill = ' ';
				int width = 0;
				int preci = -1;
				int length = 0;
				/* 0: int 1: short 2: long 3: long double 4: long long */
				/* Yes, we support long long, because it's very useful. */
				int step = 0;
				while (step == 0) {
					switch(*format++) {
					case '-':
						left_justify = 1;
						break;
					case '+':
						sign_type = 1;
						break;
					case ' ':
						sign_type = 2;
						break;
					case '#':
						prefix_suffix = 1;
						break;
					case '0':
						if (isdigit(*format) || *format == '*') {
							fill = '0';
							break;
						}
					default:
						--format;
						step = 1;
					}
				}
				if (isdigit(*format)) {
					width = *format++ ^ '0';
				} else if (*format == '*') {
					width = va_arg(ap, int);
					++format;
				}
				if (*format == '.') {
					++format;
					if (isdigit(*format)) {
						preci = *format ^ '0';
						++format;
					} else if (*format == '*') {
						preci = va_arg(ap, int);
						++format;
					}
				}
				if (*format == 'h') {
					++format;
					length = 1;
				} else if (*format == 'l') {
					++format;
					if (*format == 'l') {
						++format;
						length = 4;
					} else {
						length = 2;
					}
				} else if (*format == 'L') {
					++format;
					length = 3;
				}
				switch(*format++) {
				case 'c':
					func((char)va_arg(ap, int));
					break;
				case 's': {
					const char* str = va_arg(ap, const char*);
					while (*str && preci-- > 0) {
						func(*str++);
					}
					break;
				}
				case 'd':
				case 'i': {
					char buf[30];
					int pos = 0;
					long long val;
					int signw = 1;
					switch(length) {
					case 0:
						val = va_arg(ap, int);
						break;
					case 1:
						val = (short)va_arg(ap, int);
						break;
					case 2:
						val = va_arg(ap, long);
						break;
					case 3:
						val = va_arg(ap, long long);
						break;
					}
					if (val < 0) {
						buf[pos++] = '-';
						/* We cannot simply make val to -val. */
						/* When val is 0x80000000, -val is also 0x80000000 */
						while (val) {
							buf[pos++] = -(val % 10) | '0';
							val /= 10;
						}
					} else {
						switch(sign_type) {
						case 0:
							signw = 0;
							break;
						case 1:
							buf[pos++] = '+';
							break;
						case 2:
							buf[pos++] = ' ';
							break;
						}
						do { /* Use do to make sure 0 can be formatted. */
							buf[pos++] = (val % 10) | '0';
							val /= 10;
						} while (val);
					}
					_reverse(buf + signw, buf + pos);
					i = 0;
					if (fill == '0' && !isdigit(buf[0])) {
						func(buf[0]);
						++i;
					}
					if (left_justify) {
						for (; i < pos; ++i) {
							func(buf[i]);
						}
						while (width-- > pos) {
							func(fill);
						}
					} else {
						while (width-- > pos) {
							func(fill);
						}
						for (; i < pos; ++i) {
							func(buf[i]);
						}
					}
					break;
				}
				case 'p': {
					char buf[30];
					int pos = 0;
					unsigned long val = (unsigned long)va_arg(ap, void*);
					do { /* Use do to make sure 0 can be formatted. */
						if ((val & 15) < 10) {
							buf[pos++] = (val & 15) | '0';
						} else {
							buf[pos++] = (val & 15) + ('a' - 10);
						}
						val >>= 4;
					} while (val);
					while (pos < preci) {
						buf[pos++] = '0';
					}
					_reverse(buf, buf + pos);
					width -= 2;
					if (fill == ' ') {
						while (width-- > pos) {
							func(' ');
						}
					}
					func('0');
					func('x');
					for (i = 0; i < pos; ++i) {
						func(buf[i]);
					}
					break;

				}
				case 'u':
				case 'o':
				case 'x':
				case 'X': {
					char buf[30];
					int pos = 0;
					unsigned long long val;
					int radix = 10;
					int upp = 0;
					switch(format[-1]) {
					case 'o':
						radix = 8;
						break;
					case 'X':
						upp = 1;
					case 'x':
						radix = 16;
						break;
					}
					switch(length) {
					case 0:
						val = va_arg(ap, unsigned);
						break;
					case 1:
						val = (unsigned short)va_arg(ap, unsigned);
						break;
					case 2:
						val = va_arg(ap, unsigned long);
						break;
					case 3:
						val = va_arg(ap, unsigned long long);
						break;
					}
					if (radix != 16) {
						do { /* Use do to make sure 0 can be formatted. */
							buf[pos++] = (val % radix) | '0';
							val /= radix;
						} while (val);
					} else {
						do { /* Use do to make sure 0 can be formatted. */
							if ((val & 15) < 10) {
								buf[pos++] = (val & 15) | '0';
							} else {
								buf[pos++] = (val & 15) + ((upp ? 'A' : 'a') - 10);
							}
							val >>= 4;
						} while (val);
						while (pos < preci) {
							buf[pos++] = '0';
						}
					}
					_reverse(buf, buf + pos);
					switch(radix) {
					case 8:
						width -= 1;
						break;
					case 16:
						width -= 2;
						break;
					}
					if (fill == ' ' && !left_justify) {
						while (width-- > pos) {
							func(' ');
						}
					}
					switch(radix) {
					case 8:
						func('0');
						break;
					case 16:
						func('0');
						func(upp ? 'X' : 'x');
						break;
					}
					if (fill == '0' && !left_justify) {
						while (width-- > pos) {
							func('0');
						}
					}
					for (i = 0; i < pos; ++i) {
						func(buf[i]);
					}
					if (left_justify) {
						while (width-- > pos) {
							func(fill);
						}
					}
					break;
				}
				case 'f': {
					long double value;
					if (length == 3) {
						value = va_arg(ap, long double);
					} else {
						value = va_arg(ap, double);
					}
					if (preci == -1) {
						preci = 6;
					}
					long double itp, flp;
					flp = modfl(value, &itp);
					int bit = (itp == 0 ? 1 : ceil(log10(itp))) + preci + 5;
					char* temp = (char*)malloc(bit);
					int ip = 0;
					do {
						temp[ip++] = (int)roundl(fmodl(itp, 10)) ^ '0';
						itp = floor(itp / 10);
					} while (itp >= 1 - 1e-4);
					_reverse(temp, temp + ip);
					if (preci || prefix_suffix) {
						temp[ip++] = '.';
					}
					int bs = ip;
					flp *= pow(10, preci);
					if (modfl(flp, &flp) >= 0.5) {
						flp += 1;
					}
					for (i = 0; i < preci; ++i) {
						temp[ip++] = (int)roundl(fmodl(flp, 10)) ^ '0';
						flp = floor(flp / 10);
					}
					_reverse(temp + bs, temp + bs + preci);
					for (i = 0; i < ip; ++i) {
						func(temp[i]);
					}
					free(temp);
					break;
				}
				/* TODO: Add float e, E, g, G support. */
				}
				break;
			} while (0);
			break;
		default:
			++printf_nformat;
			func(*format++);
		}
	}
	return printf_nformat;
}

int fprintf(struct FILE* stream, const char* format, ...) {
	va_list ap;
	int ret;
	va_start(ap, format);
	ret = vfprintf(stream, format, ap);
	va_end(ap);
	return ret;
}

int vfprintf(struct FILE* stream, const char* format, va_list ap) {
	fprintf_stream = stream;
	return _printf(_print_file, format, ap);
}

int printf(const char* format, ...) {
	va_list ap;
	int ret;
	va_start(ap, format);
	ret = vprintf(format, ap);
	va_end(ap);
	return ret;
}

int vprintf(const char* format, va_list ap) {
	return vfprintf(stdout, format, ap);
}

int sprintf(char* str, const char* format, ...) {
	va_list ap;
	int ret;
	va_start(ap, format);
	ret = vsprintf(str, format, ap);
	va_end(ap);
	return ret;
}

int vsprintf(char* str, const char* format, va_list ap) {
	sprintf_ptr = str;
	return _printf(_print_str, format, ap);
}

int fgetc(struct FILE* stream) {
	unsigned char ch;
	return fread(&ch, 1, 1, stream) == 1 ? ch : EOF;
}

char* fgets(char* s, int size, struct FILE* stream) {
	char* t = s;
	--size;
	while (size) {
		*s = fgetc(stream);
		if (*s == EOF) {
			*s = 0;
			return t == s ? 0 : t;
		}
		if (*s == '\n') {
			*++s = 0;
			return t;
		}
		++s;
	}
	*s = 0;
	return t;
}

char* gets(char* s) {
	char* p = s;
	for (;;) {
		*s = fgetc(stdin);
		if (*s == EOF) {
			*s = 0;
			return p == s ? NULL : p;
		}
		if (*s == '\n') {
			*s = 0;
			return p;
		}
		++s;
	}
}

int fputc(int c, struct FILE* stream) {
	return fwrite(&c, 1, 1, stream) == 1 ? c : EOF;
}

int fputs(const char* s, struct FILE* stream) {
	size_t l = strlen(s);
	return fwrite(s, 1, l, stream) == l ? 0 : EOF;
}

int puts(const char* s) {
	return fputs(s, stdout) == EOF ? EOF : fputc('\n', stdout);
}