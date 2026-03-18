#include "common.h"
#include <assert.h>
char msg[] = "xyzzy xyzzy xyzzy\n";
int main(int argc, char **argv)
{
	int fd;
	int res = 0;
	struct bpf_test test;
	if (bpf_test_load("bpf_simple.o", &test) != 0)
		return -1;

	if ((fd = open("tst", O_RDWR | O_CREAT, 0644)) < 0) {
		fprintf(stderr, "Unable to open\n");
		res = -1;
		goto cleanup;
	}

	size_t size;
	off_t offset;
	assert(syscall(457, fd) == 0);
	do_write(fd, msg, strlen(msg), 0);
	char buf[20];
	do_read(fd, buf, strlen(msg), 0);
	
	int len = strlen(buf);
	assert(len == 18);
	assert(strcmp(buf, msg) == 0);
	int num_redactions = syscall(457,fd); 
	assert(num_redactions == 8);
	syscall(458, fd);
	assert(syscall(457, fd) == 0);

	close(fd);


cleanup:
    bpf_test_cleanup(&test);
	return res;
}
