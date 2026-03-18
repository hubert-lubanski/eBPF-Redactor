#include "common.h"


void do_write(int fd, char *buf, int size, int pos) {
	if (lseek(fd, pos, SEEK_SET) != pos)
		syserr("lseek");
	const int split = size / 3;

	if (write(fd, buf, split) != split)
		syserr("Write error 1");

	if (write(fd, buf + split, size - split) != size - split)
		syserr("Write error 2");
}

void do_read(int fd, char *buf, int size, int pos) {
	if (lseek(fd, pos, SEEK_SET) != pos)
		syserr("lseek");
	const int split = 2;

	if (read(fd, buf, split) != split)
		syserr("Read error 1");

	if (read(fd, buf + split, size - split) != size - split)
		syserr("Read error 2");
}
