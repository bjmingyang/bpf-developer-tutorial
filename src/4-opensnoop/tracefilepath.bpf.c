#include <vmlinux.h>
#include <bpf/bpf_helpers.h>

/// @description "Process ID to trace"
const volatile int pid_target = 0;

SEC("tracepoint/syscalls/sys_enter_openat")
int tracepoint__syscalls__sys_enter_openat(struct trace_event_raw_sys_enter *ctx)
{
    u64 id = bpf_get_current_pid_tgid();
    u32 pid = id >> 32;

    // Filter by PID if pid_target is set
    if (pid_target && pid_target != pid) {
        return 0;
    }

    // Read the pathname (second argument of sys_openat, ctx->args[1])
    char pathname[256]; // Buffer to store the file path
    long ret = bpf_probe_read_user_str(pathname, sizeof(pathname), (void *)ctx->args[1]);
    if (ret < 0) {
        bpf_printk("Failed to read pathname for PID %d\n", pid);
        return 0;
    }

    // Print PID and pathname
    bpf_printk("PID: %d opened file: %s\n", pid, pathname);
    return 0;
}

char LICENSE[] SEC("license") = "GPL";
