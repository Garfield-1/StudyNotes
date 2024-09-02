#include <stdio.h>
#include <unistd.h>
#include <poll.h>

int main() {
    struct pollfd fds[1];
    int ret;

    fds[0].fd = STDIN_FILENO;  // 标准输入的文件描述符
    fds[0].events = POLLIN;    // 监听可读事件

    ret = poll(fds, 1, 5000);  //使用poll函数监听标准输入，设置超时时间为5000 毫秒（5秒）

    if (ret == -1) {
        perror("poll");
        return 1;
    } else if (ret == 0) {
        printf("5秒内没有事件发生\n");
    } else {
        if (fds[0].revents & POLLIN) {
            printf("标准输入已就绪\n");
        }
    }

    return 0;
}