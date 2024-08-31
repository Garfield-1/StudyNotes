#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

void handle_timeout(int signum) {
    return;
}

int execute_command_with_timeout(const char *command)
{
    pid_t pid;
    int status;
    struct sigaction sa;
    // 设置信号处理函数以捕获超时信号
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &handle_timeout;
    sigaction(SIGALRM, &sa, NULL);

    pid = fork();
    if (pid == -1) {
        printf("[%s][%d] fork fail\n",__FUNCTION__,__LINE__);
        return -1;
    } else if (pid == 0) {
        int ret;
        ret = system(command);
        exit(WEXITSTATUS(ret));
    } else {
        // 设置定时器以在10秒后发送SIGALRM信号
        alarm(10);
        if (waitpid(pid, &status, 0) == -1) {
            printf("[%s][%d] waitpid errno = [%d]\n",__FUNCTION__,__LINE__, errno);
            return -1;
        }
        alarm(0);
        if (WIFEXITED(status)) {
            if(WEXITSTATUS(status) != 0) {
                printf("[%s] [%d] fail ID = [%d]\n", __FUNCTION__, __LINE__, WEXITSTATUS(status));
                return -1;
            }
        } else {
            printf("[%s] [%d] system operation fail\n", __FUNCTION__, __LINE__);
        }
    }
    return 0;
}

int main()
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s", "./b.o");
    execute_command_with_timeout(cmd);

    return 0;
}