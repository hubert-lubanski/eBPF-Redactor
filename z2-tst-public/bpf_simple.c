#include <linux/types.h>
#include <bpf_helpers.h>

struct redactor_ctx {
    union {
        struct {
            long long offset;
            unsigned int size;
        };
        struct {
            __u64 flags;
            __u64 mode;
        };
    };
};


SEC("redactor/decide")
int check(void *a)
{
    return 3;
}

SEC("redactor/redact")
int bpf_prog1(struct redactor_ctx *ctx)
{
    return 4;
}

char _license[] SEC("license") = "GPL";
