#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    // 获取当前进程的 PID
    pid_t my_pid = getpid();
    printf("我的 PID 是：%d\n", my_pid);

    int fd;
    char filename[] = "test_file.txt";

    while (1) {
        // 使用 openat 系统调用打开文件
        // 传递 AT_FDCWD 作为 dirfd，表示在当前工作目录打开文件
        fd = openat(AT_FDCWD, filename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

        if (fd == -1) {
            perror("打开文件失败");
            break;
        } else {
            printf("成功打开文件 %s (fd: %d)\n", filename, fd);
        }

        // 关闭文件描述符
        close(fd);

        // 每隔一秒打开一次文件
        sleep(1);
    }

    return 0;
}
