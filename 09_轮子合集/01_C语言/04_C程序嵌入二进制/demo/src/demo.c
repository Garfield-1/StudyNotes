#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "demo.h"

extern unsigned char demo_sh[];
extern unsigned int demo_sh_len;

#define filename "./tmpfile"

int main() {
    int status;
    int fd;
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // 写入二进制数据到临时文件
    if (write(fd, demo_sh, demo_sh_len) != demo_sh_len) {
        perror("write");
        close(fd);
        unlink(filename);
        return 1;
    }
    close(fd);

    // 使用 fork 执行二进制文件
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        unlink(filename);
        return 1;
    } else if (pid == 0) {
        // 子进程：执行二进制文件
        execl(filename, filename, (char *)NULL);
        perror("execl");
        unlink(filename);
        exit(1);
    } else{
        if(waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            unlink(filename);
            return 1;
        }
    }

    unlink(filename);

    return 0;  
}