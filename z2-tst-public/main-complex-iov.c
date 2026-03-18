#include "common.h"
#include <assert.h>

char msg[15] = "xxXYZZYqqQWERT";

#define IOVEC_NUM 2

#define repeat(n) for (unsigned long ___i = 0; ___i < (n); ++___i)

int main(int argc, char **argv)
{
	int fd;
	int res = 0;
	struct bpf_test test;
	if (bpf_test_load("bpf_complex.o", &test) != 0)
		return -1;

	if ((fd = open("tst", O_WRONLY | O_CREAT, 0644)) < 0) {
		fprintf(stderr, "Unable to open\n");
		res = -1;
		goto cleanup;
	}

	
	assert(syscall(457, fd) == 0);
	
    repeat(IOVEC_NUM) {
        write(fd, msg, sizeof(msg));
    }
    
    close(fd);
    if ((fd = open("tst", O_RDONLY)) < 0) {
		fprintf(stderr, "Unable to open\n");
		res = -1;
		goto cleanup;
	}
    
    char buf[IOVEC_NUM][sizeof(msg)];
    struct iovec read_vec[IOVEC_NUM];

    for (int i = 0; i < IOVEC_NUM; ++i){
        read_vec[i].iov_base = buf[i];
        read_vec[i].iov_len = sizeof(msg);
    }

	ssize_t ret = readv(fd, read_vec, IOVEC_NUM);

    printf("readv returned %ld.\n", ret);

    for (int i = 0; i < IOVEC_NUM; ++i){
        printf("read: %.*s\n", (int)sizeof(msg), (char *)read_vec[i].iov_base);
    }
	
    int syscall_ret = syscall(457, fd);

    printf("Syscall returned %d.\n", syscall_ret);

	assert(syscall_ret == 5 * IOVEC_NUM);
    for (int i = 0; i < IOVEC_NUM; ++i) {
	    assert(strlen(buf[i]) > 0 && strcmp(buf[i], "xxxyzzyqqQWERT") == 0);
    }
	close(fd);

cleanup:
	bpf_test_cleanup(&test);
	return res;
}
