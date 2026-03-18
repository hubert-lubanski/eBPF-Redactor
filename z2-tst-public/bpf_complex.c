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
    return 1;
}

SEC("redactor/redact")
int bpf_prog1(struct redactor_ctx *ctx)
{

    const char repl[] = "xyzzy";
    const char pattern[] = "XYZZY";
    char buf[20];

    int changes = 0;

    int ret = bpf_copy_from_buffer(ctx, 0, buf, sizeof(buf) - 1);
    if (ret != 0)
        return -1;

    for (int i = 0; i < sizeof(buf) - 1; ++i)
    {
        int match = 0;
        for (int j = 0; j < sizeof(pattern) - 1; ++j)
        { 
            if (buf[i + j] == pattern[j])
            {
                ++match;
            }
	    else
	    {
                break;
            }
        }

        if (match == sizeof(pattern) - 1)
	{
            bpf_copy_to_buffer(ctx, i, repl, sizeof(repl) - 1);
	    changes += match;
	}
    }

    return changes;
}

char _license[] SEC("license") = "GPL";
