#include "stdio.h"
#include "string.h"
#include "assert.h"
#include "stdlib.h"
#include "ctype.h"
#include "math.h"
#include "limits.h"
#include <sys/stat.h> // For struct stat
#include "syscall.h"

static char out_buffer[3][1 << 16];
static size_t out_pos[3];
static char in_buffer[1 << 16];
static size_t in_pos, in_size, in_file_size;
static char unget = EOF;

static void _cache_write(long fd, const char* str, register size_t len) {
	while (len--) {
		out_buffer[fd][out_pos[fd]++] = *str++;
		if (out_pos[fd] >> 16) {
			syscall(__NR_write, fd, (long)out_buffer[fd], 1 << 16);
			out_pos[fd] = 0;
		}
	}
}

static size_t _cache_read(long fd, char* str, register long len) {
	size_t i = 0;
	if (unget != EOF) {
		*str++ = unget;
		unget = EOF;
		--len;
		++i;
	}
	if (len > in_file_size + in_size - in_pos) {
		len = in_file_size + in_size - in_pos;
	}
	while (len > 0) {
		while (in_pos < in_size && len--) {
			*str++ = in_buffer[in_pos++];
			++i;
		}
		if (in_pos == in_size) {
			int l = in_file_size >> 16 ? (1 << 16) : in_file_size;
			syscall(__NR_read, fd, (long)in_buffer, l);
			in_size = l;
			in_pos = 0;
			in_file_size -= l;
		}
	}
	return i;
}

int fflush(struct FILE* stream) {
	long fd = ((long)stream);
	if (stream) {
		syscall(__NR_write, fd, (long)out_buffer[fd], out_pos[fd]);
		out_pos[fd] = 0;
	}
	return 0;
}

size_t fread(void* ptr, size_t size, size_t nmemb, struct FILE* stream) {
	size_t valid = (in_size - in_pos + (unget != EOF)) / size;
	if (nmemb > valid) {
		nmemb = valid;
	}
	_cache_read((long)stream, ptr, size * nmemb);
	return nmemb;
}

size_t fwrite(const void* ptr, size_t size, size_t nmemb, struct FILE* stream) {
	_cache_write((long)stream, ptr, size * nmemb);
	return nmemb;
}

static int scanf_nformat;
static char fscanf_last;
static struct FILE* fscanf_stream;
static const char* sscanf_ptr;

static char _scan_file() {
	return fscanf_last = getc(fscanf_stream);
}

static void _scan_unget_file() {
	ungetc(fscanf_last, fscanf_stream);
}

static char _scan_str() {
	return *sscanf_ptr++;
}

static void _scan_unget_str() {
	--sscanf_ptr;
}

static int _scanf(char (*fun_getc)(), void (*fun_unget)(), const char* format, va_list ap) {
	while (*format) {
		if (isspace(*format)) {
			while (isspace(fun_getc())) {}
			fun_unget();
			++format;
		} else if (*format == '%') {
			if (*++format == '%') {
				if (fun_getc() != '%') {
					return scanf_nformat;
				} else {
					++format;
				}
			} else {
				int ignore = 0;
				long maxi_read = LLONG_MAX;
				if (*format == '*') {
					ignore = 1;
					++format;
				}
				if (isdigit(*format)) {
					maxi_read = 0;
					while (isdigit(*format)) {
						maxi_read = ((maxi_read + (maxi_read << 2)) << 1) + (*format++ ^ '0');
					}
				}
				int length = 0;
				/* 0: int 1: long 2: long double 3: long long */
				if (*format == 'l') {
					++format;
					if (*format == 'l') {
						++format;
						length = 3;
					} else {
						length = 1;
					}
				} else if (*format == 'L') {
					++format;
					length = 2;
				}
				switch(*format++) {
				case 'i': {
					register long long num = 0;
					int f = 1;
					do {
						while (isspace(fun_getc())) {}
						fun_unget();
						char c = fun_getc();
						--maxi_read;
						if (c == '+' || c == '-') {
							if (maxi_read <= 0) {
								return scanf_nformat;
							}
							f = (c == '-' ? -1 : 1);
							c = fun_getc();
							--maxi_read;
						}
						if (!isdigit(c)) {
							return scanf_nformat;
						}
						if (c == '0') {
							if (maxi_read == 0) {
								break;
							}
							c = fun_getc();
							--maxi_read;
							if (toupper(c) == 'X') {
								if (maxi_read == 0) {
									break;
								}
								c = fun_getc();
								--maxi_read;
								if (!isxdigit(c)) {
									fun_unget();
									break;
								} else {
									num = (isdigit(c) ? c ^ '0' : (toupper(c) - 'A' + 10));
								}
								do {
									c = fun_getc();
									if (!maxi_read-- || !isxdigit(c)) {
										break;
									}
									num <<= 4;
									num += (isdigit(c) ? c ^ '0' : (toupper(c) - 'A' + 10));
								} while (1);
								fun_unget();
							} else if (isdigit(c)) {
								if ((c ^ '0') >= 8) {
									num = 0;
									break;
								} else {
									num = c ^ '0';
								}
								if (!maxi_read) {
									break;
								}
								do {
									c = fun_getc();
									if (!maxi_read-- || !(isdigit(c) && (c ^ '0') < 8)) {
										break;
									}
									num <<= 3;
									num += c ^ '0';
								} while (1);
								fun_unget();
							}
						} else {
							num = (c ^ '0');
							do {
								c = fun_getc();
								if (!maxi_read-- || !isdigit(c)) {
									break;
								}
								num = ((num + (num << 2)) << 1) + (c ^ '0');
							} while (1);
							fun_unget();
						}
					} while (0);
					++scanf_nformat;
					if (!ignore) {
						num *= f;
						switch(length) {
						case 0:
							*va_arg(ap, int*) = num;
							break;
						case 1:
							*va_arg(ap, long*) = num;
							break;
						case 3:
							*va_arg(ap, long long*) = num;
							break;
						}
					}
					break;
				}
				}
			}
		} else {
			if (fun_getc() != *format) {
				return scanf_nformat;
			} else {
				++format;
			}
		}
	}
	return scanf_nformat;
}

int fscanf(struct FILE* stream, const char* format, ...) {
	va_list lst;
	va_start(lst, format);
	int ret = vfscanf(stream, format, lst);
	va_end(lst);
	return ret;
}

int vfscanf(struct FILE* stream, const char* format, va_list ap) {
	fscanf_stream = stream;
	return _scanf(_scan_file, _scan_unget_file, format, ap);
}

int sscanf(const char* str, const char* format, ...) {
	va_list lst;
	va_start(lst, format);
	int ret = vsscanf(str, format, lst);
	va_end(lst);
	return ret;
}

int vsscanf(const char* str, const char* format, va_list ap) {
	sscanf_ptr = str;
	return _scanf(_scan_str, _scan_unget_str, format, ap);
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

static void _reverse(register char* beg, register char* end) {
	--end;
	while (beg < end) {
		char k = *beg;
		*beg++ = *end;
		*end-- = k;
	}
}

static int _printf(void (*fun_putc)(char), const char* format, va_list ap) {
	int i;
	printf_nformat = 0;
	while (*format) {
		switch(*format) {
		case '%':
			++format;
			if (*format == '%') {
				++format;
				fun_putc('%');
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
					fun_putc((char)va_arg(ap, int));
					break;
				case 's': {
					const char* str = va_arg(ap, const char*);
					preci &= INT_MAX; /* Make -1 to INT_MAX */
					while (*str && preci-- > 0) {
						fun_putc(*str++);
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
						fun_putc(buf[0]);
						++i;
					}
					if (left_justify) {
						for (; i < pos; ++i) {
							fun_putc(buf[i]);
						}
						while (width-- > pos) {
							fun_putc(fill);
						}
					} else {
						while (width-- > pos) {
							fun_putc(fill);
						}
						for (; i < pos; ++i) {
							fun_putc(buf[i]);
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
							fun_putc(' ');
						}
					}
					fun_putc('0');
					fun_putc('x');
					for (i = 0; i < pos; ++i) {
						fun_putc(buf[i]);
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
							fun_putc(' ');
						}
					}
					switch(radix) {
					case 8:
						fun_putc('0');
						break;
					case 16:
						fun_putc('0');
						fun_putc(upp ? 'X' : 'x');
						break;
					}
					if (fill == '0' && !left_justify) {
						while (width-- > pos) {
							fun_putc('0');
						}
					}
					for (i = 0; i < pos; ++i) {
						fun_putc(buf[i]);
					}
					if (left_justify) {
						while (width-- > pos) {
							fun_putc(fill);
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
					width -= ip;
					if (sign_type) {
						--width;
					}
					if (!left_justify && fill == ' ') {
						while (width--) {
							fun_putc(' ');
						}
					}
					if (value < 0) {
						fun_putc('-');
					} else if (sign_type) {
						fun_putc(sign_type == 1 ? '+' : ' ');
					}
					if (!left_justify && fill == '0') {
						while (width--) {
							fun_putc('0');
						}
					}
					for (i = 0; i < ip; ++i) {
						fun_putc(temp[i]);
					}
					if (left_justify) {
						while (width--) {
							fun_putc(' ');
						}
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
			fun_putc(*format++);
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

int sprintf(char* str, const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	int ret = vsprintf(str, format, ap);
	va_end(ap);
	return ret;
}

int vsprintf(char* str, const char* format, va_list ap) {
	sprintf_ptr = str;
	int ret = _printf(_print_str, format, ap);
	*sprintf_ptr = 0;
	return ret;
}

int fgetc(struct FILE* stream) {
	char ch = EOF;
	return _cache_read((long)stream, &ch, 1) == 1 ? ch : EOF;
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

int ungetc(int c, struct FILE* stream) {
	unget = c;
	return c;
}

void __judge_lib_init_stdio() {
	struct stat st;
	syscall(__NR_fstat, 0, (long)&st, 0);
	in_file_size = st.st_size;
}