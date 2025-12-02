# syslog原理分析

[TOC]

## 概述

`syslog`是一个协议规范，需要有`server`端和`client`端，由`server`端收集日志并记录

* `Syslog`服务端通常是`syslogd`或更现代的 `rsyslog`、`syslog-ng` 或 `systemd-journald`）负责接收来自客户端的日志消息，存储它们，并可能根据配置转发到其他日志服务器或应用程序。服务端是`syslog`的核心，它处理日志的存储、转发和管理
* `Syslog` 客户端是指发送日志消息的应用程序、服务或内核。它将日志消息发送给`syslog`服务端（日志守护进程）。客户端通过 `syslog()`函数或者类似工具将日志消息发送到 `syslogd`或其他日志守护进程

## 日志信息的获取

`syslog`的获取主要通过以下的几种方式

<img src="./img/%E7%B3%BB%E7%BB%9F%E6%97%A5%E5%BF%97%E6%A6%82%E8%A7%88.png" alt="系统日志概览" style="zoom: 67%;" />

* `/dev/log`句柄

    `/dev/log`是一个特殊的`Unix`域套接字，用于在用户空间应用程序和系统日志守护进程之间传递日志消息。它是 `syslog` 系统的一部分，允许应用程序（包括内核和用户空间程序）将日志消息传递给日志守护进程（如 `rsyslog`、`syslog-ng` 或 `systemd-journald`），这些日志守护进程再将消息存储到日志文件中或转发到其他位置

    通常，任意进程都可以使用`syslog(3)`库函数来记录消息。这个函数会使用传入的参数以标准的格式构建一条消息，然后将这条消息写入`/dev/log socket`以供 `syslogd`读取

    `/dev/log`中的消息的另一个来源是`Kernel Log daemon klogd`，它会收集内核日志消息（内核使用 `printk()`函数生成的消息）。这些消息的收集可以通过两个等价的`Linux`特有的接口中的一个来完成（即`/proc/kmsg`文件和`syslog(2)`系统调用），然后使用`syslog(3)`库函数将它们写入`/dev/log`

    查询`Linux`源码可以在`devices.rst`文件中找到关于这个句柄的描述

    ``` bash
    Non-transient sockets and named pipes may exist in /dev.  Common entries are:
    
    =============== =============== ===============================================
    /dev/printer    socket        lpd local socket
    /dev/log    socket        syslog local socket
    /dev/gpmdata    socket        gpm mouse multiplexer
    =============== =============== ===============================================
    ```

* `UDP 514`端口

    `514`号端口是`syslog`使用的默认端口号。用于接收从其他计算机上发送的`syslog`日志消息

* `/dev/kmsg`句柄

    `/dev/kmsg`是一个可读写的字符设备文件，且允许多个进程进行读取，每个进程都能得到完整的数据流。 而且由于它是可读写的，因此你还可以插入消息到内核日志中

    查询`Linux`源码可以在`dev-kmsg`文件中找到关于这个句柄的描述，这里只摘抄一小段，详细描述需要查看源码

    ``` bash
    Description:    The /dev/kmsg character device node provides userspace access to the kernel's printk buffer.
    ```

* `/proc/kmsg`句柄

    `/proc/kmsg`是一个只读的伪`FIFO`文件，且里面的消息只能被读取一次。当多个进程同时尝试读取该文件时，每个进程将只能读取到一部分日志内容

## busybox实现

分析`busybox`源码可以在`syslogd.c`文件中看到，关于`syslog`的实现，在此仅列出关键的核心流程

### **函数调用栈**

```c
syslogd_main
    ->do_syslogd
        ->signal_no_SA_RESTART_empty_mask(SIGTERM, record_signo);
        ->signal_no_SA_RESTART_empty_mask(SIGINT, record_signo);
        ->create_socket()//创建一个socket绑定到/dev/log上，并通过xmove_fd函数将这个sockrt绑定到STDIN_FILENO上，用于后续读取
            ->strcpy(sunx.sun_path, _PATH_LOG);//define _PATH_LOG /dev/log
        ->xmove_fd(create_socket(), STDIN_FILENO)
        /*从STDIN_FILENO中读出内容，并将内容记录为syslog
          bb_got_signal信号被绑定到SIGTERM和SIGINT */
        ->while(!bb_got_signal) {
            read(STDIN_FILENO, recvbuf, MAX_READ - 1);
            split_escape_and_log(recvbuf, sz)
          }
```

### **关键函数**

**`xmove_fd`函数**

实际上是对`dup2`的一层封装

```c
void FAST_FUNC xdup2(int from, int to)
{
    if (dup2(from, to) != to)
        bb_simple_perror_msg_and_die("can't duplicate file descriptor");
}

void FAST_FUNC xmove_fd(int from, int to)
{
    if (from == to)
        return;
    xdup2(from, to);
    close(from);
}
```

**`do_syslogd`函数**

`syslogd`的核心函数，主要实现读取`/dev/log`句柄的内容并将其记录为`syslog`

> 笔者注：下文仅保留核心逻辑，删除部分不影响阅读的部分

```c
static void do_syslogd(void)
{
#define recvbuf (G.recvbuf)

    signal_no_SA_RESTART_empty_mask(SIGTERM, record_signo);
    signal_no_SA_RESTART_empty_mask(SIGINT, record_signo);
    signal(SIGHUP, SIG_IGN);
    xmove_fd(create_socket(), STDIN_FILENO);

    while (!bb_got_signal) {
        ssize_t sz;

 read_again:
        sz = read(STDIN_FILENO, recvbuf, MAX_READ - 1);

        while (1) {
            if (sz == 0)
                goto read_again;
            if (recvbuf[sz-1] != '\0' && recvbuf[sz-1] != '\n')
                break;
            sz--;
        }
        if (!ENABLE_FEATURE_REMOTE_LOG || (option_mask32 & OPT_locallog)) {
            recvbuf[sz] = '\0'; 
            // 将读取到的内容作为syslog记录
            split_escape_and_log(recvbuf, sz);
        }
    } /* while (!bb_got_signal) */

    timestamp_and_log_internal("syslogd exiting");
    remove_pidfile_std_path_and_ext("syslogd");
    kill_myself_with_sig(bb_got_signal);
}
```

**核心思想**

`do_syslogd`函数的核心流程非常简单

1. 配置信号量
2. 配置`socket`用于读取`/dev/log`，并将读取的结果作为`syslog`记录
3. 循环读取`/dev/log`的内容直到，有外部信号量使`syslogd`进程退出

## 参考文档

> [RFC 3164: The BSD Syslog Protocol (rfc-editor.org)](https://www.rfc-editor.org/rfc/rfc3164)
>
> [syslog协议介绍_syslog是什么协议-CSDN博客](https://blog.csdn.net/chdhust/article/details/50989785)
>
> [Busybox的syslogd认识与使用 - ArnoldLu - 博客园](https://www.cnblogs.com/arnoldlu/p/10583233.html)
>
> [sysklogd源码](https://github.com/troglobit/sysklogd)
>
> [BusyBox源码](https://www.busybox.net/)
