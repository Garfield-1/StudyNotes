#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10

int main() {
    int epoll_fd, nfds, n;
    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];
    char buf[256];

    // 创建一个epoll实例
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // 添加标准输入文件描述符到epoll实例中
    event.events = EPOLLIN;
    event.data.fd = STDIN_FILENO;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &event) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // 等待事件发生
        nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }

        // 处理就绪的事件
        for (n = 0; n < nfds; ++n) {
            if (events[n].data.fd == STDIN_FILENO) {
                // 从标准输入中读取数据
                if (fgets(buf, sizeof(buf), stdin) == NULL) {
                    perror("fgets");
                    exit(EXIT_FAILURE);
                }
                printf("Received input: %s", buf);
                // 如果收到exit，则退出循环
                if (strcmp(buf, "exit\n") == 0) {
                    goto cleanup;
                }
            }
        }
    }

cleanup:
    close(epoll_fd);
    return 0;
}