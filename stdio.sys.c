#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "sys.h"

int _sys_fflush(int fd) {
	return fsync(fd);
}

size_t _sys_fread(void* ptr, size_t size, size_t nmemb, int fd) {
	size_t i = 0;
	for (i = 0; i < nmemb; ++i) {
		if (size != read(fd, ptr, size)) {
			break;
		} else {
			ptr += size;
		}
	}
	return i;
}

size_t _sys_fwrite(const void* ptr, size_t size, size_t nmemb, int fd) {
	size_t i = 0;
	for (i = 0; i < nmemb; ++i) {
		if (size != write(fd, ptr, size)) {
			break;
		} else {
			ptr += size;
		}
	}
	return i;
}