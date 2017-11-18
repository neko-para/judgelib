#include "stdlib.h"
#include "stdio.h"
#include "signal.h"
#include "string.h"
#include "ctype.h"
#include "math.h"
#include "syscall.h"

static void (*atexit_handler)(void);

static unsigned long next = 1;
int rand() {
	next = next * 1103515245 + 12345;
	return (unsigned)(next >> 16) & 32767;
}

void srand(unsigned seed) {
	next = seed;
}

static void* ptr;
void* _SBRK(size_t l) {
	ptr += l;
	return (void*)syscall(__NR_brk, (long)ptr, 0, 0);
}

typedef struct Splay {
	struct Splay* father;
	struct Splay* child[2];
	size_t len;
	size_t sum;
	size_t maxi;
	size_t use;
	size_t pad;
} Splay;

Splay* root;

static void Init(Splay* p, Splay* f, size_t l) {
	p->father = f;
	p->child[0] = p->child[1] = 0;
	p->len = p->sum = p->maxi = l;
	p->use = 0;
}

static int Which(Splay* p) {
	return p->father->child[1] == p;
}

static void Update(Splay* p) {
	if (p->use) {
		p->maxi = 0;
	} else {
		p->maxi = p->len;
	}
	p->sum = p->len;
	if (p->child[0]) {
		if (p->maxi < p->child[0]->maxi) {
			p->maxi = p->child[0]->maxi;
		}
		p->sum += p->child[0]->sum;
	}
	if (p->child[1]) {
		if (p->maxi < p->child[1]->maxi) {
			p->maxi = p->child[1]->maxi;
		}
		p->sum += p->child[1]->sum;
	}
}

static void Rotate(Splay* p) {
	Splay* f = p->father;
	int k = Which(p);
	if (f->father) {
		f->father->child[Which(f)] = p;
	} else {
		root = p;
	}
	p->father = f->father;
	f->father = p;
	f->child[k] = p->child[k ^ 1];
	p->child[k ^ 1] = f;
	if (f->child[k]) {
		f->child[k]->father = f;
	}
	Update(f);
	Update(p);
}

static void SplayTo(Splay* p, Splay** t) {
	while (*t != p) {
		if (p->father != *t) {
			if (Which(p) == Which(p->father)) {
				Rotate(p->father);
			} else {
				Rotate(p);
			}
		}
		Rotate(p);
	}
}

static Splay* Left(Splay* p) {
	while (p->child[0]) {
		p = p->child[0];
	}
	return p;
}

static Splay* Right(Splay* p) {
	while (p->child[1]) {
		p = p->child[1];
	}
	return p;
}

static Splay* CheckValid(Splay* p, Splay* ptr) {
	if (p == ptr) {
		return p;
	} else if (ptr < p) {
		if (p->child[0]) {
			return CheckValid(p->child[0], ptr);
		} else {
			return NULL;
		}
	} else {
		if (p->child[1]) {
			return CheckValid(p->child[1], ptr);
		} else {
			return NULL;
		}
	}
}

static Splay* FindFirstValid(Splay* p, size_t l) {
	if (p->child[0] && p->child[0]->maxi >= l) {
		return FindFirstValid(p->child[0], l);
	} else if (!p->use && p->len >= l) {
		return p;
	} else {
		return FindFirstValid(p->child[1], l);
	}
}

static void FixAlign(size_t* l) {
	if (!*l) {
		*l = 4;
	} else if (*l & 3) {
		*l += 4 - (*l & 3);
	}
}

void* malloc(size_t size) {
	FixAlign(&size);
	if (root->maxi < size) {
		SplayTo(Right(root), &root);
		if (root->use) {
			root->child[1] = _SBRK(sizeof(Splay) + size);
			Init(root->child[1], root, size);
			Update(root);
		} else { /* Increase the last block */
			_SBRK(size - root->len);
			root->len = size;
			Update(root);
		}
	}
	SplayTo(FindFirstValid(root, size), &root);
	root->use = 1;
	Update(root);
	return (void*)root + sizeof(Splay);
}

void free(void* ptr) {
	Splay* p = CheckValid(root, ptr - sizeof(Splay));
	if (!p) {
		return;
	}
	SplayTo(p, &root);
	root->use = 0;
	Update(root);
	SplayTo(Right(p->child[0]), root->child);
	if (!root->child[0]->use) {
		root->child[0]->len += root->len + sizeof(Splay);
		root->child[0]->child[1] = root->child[1];
		if (root->child[1]) {
			root->child[1]->father = root->child[0];
		}
		root->child[0]->father = NULL;
		root = root->child[0];
		Update(root);
	}
	if (root->child[1]) {
		SplayTo(Left(root->child[1]), root->child + 1);
		if (!root->child[1]->use) {
			root->len += root->child[1]->len + sizeof(Splay);
			root->child[1] = root->child[1]->child[1];
			if (root->child[1]) {
				root->child[1]->father = root;
			}
			Update(root);
		}
	}
}

void* realloc(void* ptr, size_t size) {
	Splay* p = CheckValid(root, ptr - sizeof(Splay));
	if (!p) {
		return NULL;
	}
	FixAlign(&size);
	SplayTo(p, &root);
	if (size == root->len) {
		return ptr;
	} else if (size < root->len) {
		if (root->child[1]) {
			SplayTo(Left(root->child[1]), root->child + 1);
			if (!root->child[1]->use) {
				root->child[1]->len += root->len - size;
				root->len = size;
				Update(root->child[1]);
				Update(root);
				return ptr;
			} else {
				if (root->len - size >= sizeof(Splay)) {
					Splay* pr = root->child[1];
					root->child[1] = (Splay*)(((void*)root) + sizeof(Splay) + size);
					Init(root->child[1], root, root->len - size - sizeof(Splay));
					root->len = size;
					root->child[1]->child[1] = pr;
					if (pr) {
						pr->father = root->child[1];
					}
					Update(root->child[1]);
					Update(root);
					return ptr;
				} else {
					return ptr; /* Just drop the memory */
				}
			}
		} else {
			if (root->len - size >= sizeof(Splay)) {
				root->child[1] = (Splay*)(((void*)root) + sizeof(Splay) + size);
				Init(root->child[1], root, root->len - size - sizeof(Splay));
				root->len = size;
				Update(root);
				return ptr;
			} else {
				return ptr; /* Just drop the memory */
			}
		}
	} else {
		if (root->child[1]) {
			SplayTo(Left(root->child[1]), root->child + 1);
			if (!root->child[1]->use && root->len + sizeof(Splay) + root->child[1]->len >= size) {
				if (root->len + root->child[1]->len >= size) {
					root->child[1]->len -= size - root->len;
					root->len = size;
					Update(root->child[1]);
					Update(root);
					return ptr;
				} else {
					root->len += sizeof(Splay) + root->child[1]->len;
					root->child[1] = root->child[1]->child[1];
					if (root->child[1]) {
						root->child[1]->father = root;
					}
					Update(root);
					return ptr;
				}
			} else {
				unsigned* pn = malloc(size);
				unsigned* p = pn;
				unsigned* pr = (unsigned*)ptr;
				size_t l = root->len >> 2;
				while (l--) {
					*p++ = *pr++;
				}
				free(ptr);
				return pn;
			}
		} else {
			_SBRK(size - root->len);
			root->len = size;
			return ptr;
		}
	}
}

void abort() {
	raise(SIGABRT);
	exit(0);
}

int atexit(void (*func)(void)) {
	atexit_handler = func;
	return 0;
}

void exit(int status) {
	atexit_handler ? atexit_handler() : (void)0;
	fflush(stdout);
	fflush(stderr);
	_exit(status);
}

void _exit(int status) {
	syscall(__NR_exit, status, 0, 0);
}

int abs(int n) {
	return n < 0 ? -n : n;
}

long labs(long n) {
	return n < 0 ? -n : n;
}

long long llabs(long long n) {
	return n < 0 ? -n : n;
}

div_t div(int numerator, int denominator) {
	return (div_t){numerator / denominator, numerator % denominator};
}

ldiv_t ldiv(long numerator, long denominator) {
	return (ldiv_t){numerator / denominator, numerator % denominator};
}

lldiv_t lldiv(long long numerator, long long denominator) {
	return (lldiv_t){numerator / denominator, numerator % denominator};
}

void* bsearch(const void* key, const void* base, size_t num, size_t size, int (*compar)(const void*, const void*)) {
	while (num) {
		size_t half = (num >> 1) * size;
		int ret = compar(base + half, key);
		if (!ret) {
			return (void*)(base + half);
		} else if (ret > 0) {
			num >>= 1;
		} else {
			base = base + half + size;
			num -= num >> 1;
			--num;
		}
	}
	return 0;
}

void qsort(void* base, size_t num, size_t size, int (*compar)(const void*, const void*)) {
	if (num < 2) {
		return;
	}
	size_t i;
	void* temp = malloc(size);
	--num;
	const void* key = base + num * size;
	void* now = base;
	void* low = base;
	void* same = base;
	for (i = 0; i < num; ++i) {
		int ret = compar(now, key);
		if (ret == 0) {
			memcpy(temp, now, size);
			memcpy(now, same, size);
			memcpy(same, temp, size);
			same += size;
		} else if (ret < 0) {
			memcpy(temp, now, size);
			memcpy(now, same, size);
			memcpy(same, low, size);
			memcpy(low, temp, size);
			low += size;
			same += size;
		}
		now += size;
	}
	free(temp);
	memcpy(temp, now, size);
	memcpy(now, same, size);
	memcpy(same, temp, size);
	same += size;
	qsort(base, (low - base) / size, size, compar);
	qsort(same, num + 1 - (same - base) / size, size, compar);
}

long double strtold(const char* str, char** endptr) {
	register long double ret = 0;
	register int f = 1;
	while (isspace(*str)) {
		++str;
	}
	if (*str == '+' || *str == '-') {
		f = (*str++ == '-') ? -1 : 1;
	}
	while ( isdigit(*str)) {
		ret = ret * 10 + (*str++ ^ '0');
	}
	if (*str == '.') {
		long double flt = 0;
		const char* p = ++str;
		while (isdigit(*str)) {
			flt += flt * 10 + (*str++ ^ '0');
		}
		ret += flt * powl(10, -(long)(str - p));
	}
	if (*str == 'e' || *str == 'E') {
		++str;
		register long i = 0;
		while (isdigit(*str)) {
			i = ((i + (i << 2)) << 1) + (*str++ ^ '0');
		}
		ret *= powl(10, i);
	}
	if (endptr) {
		*endptr = (char*)str;
	}
	return ret * f;
}

long long atoll(const char* str) {
	register long long i = 0;
	register int f = 1;
	while (isspace(*str)) {
		++str;
	}
	if (*str == '+' || *str == '-') {
		f = (*str++ == '-') ? -1 : 1;
	}
	while (isdigit(*str)) {
		i = ((i + (i << 2)) << 1) + (*str++ ^ '0');
	}
	return i * f;
}

long long strtoll(const char* str, char** endptr, int base) {
	register long long i = 0;
	register int f = 1;
	while (isspace(*str)) {
		++str;
	}
	if (*str == '+' || *str == '-') {
		f = (*str++ == '-') ? -1 : 1;
	}
	if (base == 0 || base == 16) {
		if (str[0] == '0' && tolower(str[1]) == 'x') {
			str += 2;
			base = 16;
		}
	}
	if (base == 0 || base == 8) {
		if (str[0] == '0' && isdigit(str[1])) {
			++str;
			base = 8;
		}
	}
	if (base <= 10) {
		while (isdigit(*str) && (*str ^ '0') < base) {
			i = i * base + (*str++ ^ '0');
		}
	} else {
		while (isalnum(*str) && toupper(*str) - 'A' + 10 < base) {
			i = i * base + (isdigit(*str) ? (*str++ ^ '0') : (*str - 'A' + 10));
		}
	}
	if (endptr) {
		*endptr = (char*)str;
	}
	return i * f;
}

unsigned long long strtoull(const char* str, char** endptr, int base) {
	register unsigned long long i = 0;
	while (isspace(*str)) {
		++str;
	}
	if (base == 0 || base == 16) {
		if (str[0] == '0' && tolower(str[1]) == 'x') {
			str += 2;
			base = 16;
		}
	}
	if (base == 0 || base == 8) {
		if (str[0] == '0' && isdigit(str[1])) {
			++str;
			base = 8;
		}
	}
	if (base <= 10) {
		while (isdigit(*str) && (*str ^ '0') < base) {
			i = i * base + (*str++ ^ '0');
		}
	} else {
		while (isalnum(*str) && toupper(*str) - 'A' + 10 < base) {
			i = i * base + (isdigit(*str) ? (*str++ ^ '0') : (*str - 'A' + 10));
		}
	}
	if (endptr) {
		*endptr = (char*)str;
	}
	return i;
}

void __judge_lib_init_stdlib() {
	ptr = (void*)syscall(__NR_brk, 0, 0, 0);
	root = _SBRK(sizeof(Splay));
	Init(root, NULL, 0);
	root->use = 1;
}