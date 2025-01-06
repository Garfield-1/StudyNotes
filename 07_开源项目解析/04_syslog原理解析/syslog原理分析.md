# `syslog`原理分析

参考文档

[RFC 3164: The BSD Syslog Protocol (rfc-editor.org)](https://www.rfc-editor.org/rfc/rfc3164)

[syslog协议介绍_syslog是什么协议-CSDN博客](https://blog.csdn.net/chdhust/article/details/50989785)

[sysklogd源码](https://github.com/troglobit/sysklogd)

[BusyBox源码](https://www.busybox.net/)



## 概述

`syslog`是一个协议规范，需要有`server`端和`client`端，由`server`端收集日志并记录，通常使用`busybox`所提供的`server`端服务，底层会使用`sysklogd`库提供的接口



## `busybox`实现

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

### **`/dev/log`句柄**

`/dev/log`是一个专门用于`syslog`服务的特殊句柄，查询`Linux`源码可以在`devices.rst`中找到关于这个句柄的描述

```rst
Non-transient sockets and named pipes may exist in /dev.  Common entries are:

=============== =============== ===============================================
/dev/printer	socket		lpd local socket
/dev/log	socket		syslog local socket
/dev/gpmdata	socket		gpm mouse multiplexer
=============== =============== ===============================================
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

