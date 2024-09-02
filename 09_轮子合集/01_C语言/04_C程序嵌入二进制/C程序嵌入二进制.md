# C程序嵌入二进制

将待执行程序转换成二进制

例如当前存在`shell`文件如下

```shell
#!/bin/sh

echo 1 > ./demo
```

使用`xxd`工具执行命令`xxd -i demo.sh > demo.h`即可获得`demo.h`如下

```c
unsigned char demo_sh[] = {
  0x23, 0x21, 0x2f, 0x62, 0x69, 0x6e, 0x2f, 0x73, 0x68, 0x0a, 0x0a, 0x65,
  0x63, 0x68, 0x6f, 0x20, 0x31, 0x20, 0x3e, 0x20, 0x2e, 0x2f, 0x64, 0x65,
  0x6d, 0x6f
};
unsigned int demo_sh_len = 26;
```

创建`demo.c`内容如下

```c
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
```

**程序解析**

首先利用`xxd`工具将`shell`文件转换成`16进制`存在头文件中，在`C`文件中将头文件中记录的`16进制`数据还原后写入临时文件，然后再执行