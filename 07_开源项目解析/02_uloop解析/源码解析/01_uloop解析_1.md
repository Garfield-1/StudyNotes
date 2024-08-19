# uloop源码解析

**参考链接**

[【openwrt】uloop_run过程分析_uloop 源代码分析-CSDN博客](https://blog.csdn.net/qq_24835087/article/details/122439506)

[openWrt libubox组件之uloop原理分析-CSDN博客](https://blog.csdn.net/weixin_30388677/article/details/98090433)

## 整体框架接口

- 初始化事件循环

　　`int uloop_init(void)`

　　创建一个`epoll`的句柄，最多监控`32`个文件描述符。

　　设置文件描述符属性，如`FD_CLOEXEC`。

- 事件循环主处理入口

　　`void uloop_run(void)`

- 销毁事件循环

　　`void uloop_done(void)`

　　关闭`epoll`句柄。

　　清空定时器链表中的所有的定时器。

　　清空进程处理事件链表中删除所有的进程事件节点。

## 定时器模块

## 使用方法

```c
#include <stdio.h>
#include <uloop.h>

struct uloop_timeout timeout;						// 创建uloop_timeout全局变量

int frequency = 5; 									// 每隔5秒超时一次

// 设置定时器回调
static void timeout_cb(struct uloop_timeout *t)
{
	printf("[%s] uloop test \n", __FUNCTION__);
	uloop_timeout_set(t, frequency * 1000);
}

int main()
{	
	uloop_init();									// 使用库初始化	
	timeout.cb = timeout_cb;						// 设置定时器超时回调
	uloop_timeout_set(&timeout, frequency * 1000);	// 设置下次的超时时间
	uloop_run();
	uloop_done();
}
```

上述程序效果：每隔`5S`打印一次



## 概述

`uloop`的核心数据结构围绕着三个链表进行展开，分别是定时器链表、子进程链表、信号管理链表

初始化阶段做的工作主要是，创建`epoll`、设置信号处理、设置定时器链表节点和超时回调函数

整体的架构被设计成，上层一个大循环，循环内部每一次检查超时事件链表和子进程链表，超时事件执行回调并删除链表中的节点，需要通过信号处理的事件也执行回调删除节点。底层使用`epoll`监听`socket`句柄





整体编码风格十分飘逸，使用大量全局变量控制关键流程且贯穿流程始终，内部多个链表互相影响互相耦合，但整体设计小巧精致少有冗余。可以明显作者在设计时，将整个`uloop.c`作为一个独立的单独模块。



## 核心数据结构

### 全局变量

```c
/* 管理事件循环的epoll机制的文件描述符 */    
static int poll_fd = -1;

/* 事件循环是否被取消,用于标记事件循环是否应终止执行 */
bool uloop_cancelled = false;

/* 是否需要处理SIGCHLD信号,当子进程终止时会发送到父进程。将其设置为true表示uloop应处理子进程的退出状态 */
bool uloop_handle_sigchld = true;

/* 存储事件循环的状态码 */
static int uloop_status = 0;

/* 指示是否需要处理SIGCHLD信号,用于内部控制信号处理逻辑 */
static bool do_sigchld = false;

/* 用于存储当前事件循环中所有注册的文件描述符的事件信息 */
static struct uloop_fd_event cur_fds[ULOOP_MAX_EVENTS];

/* 当前文件描述符的索引或标识符 */
static int cur_fd;

/* 当前文件描述符的数量 */
static int cur_nfds;

/* 记录事件循环的运行深度或层级 */
static int uloop_run_depth = 0;
```



### **`timeouts`链表**

定时器链表，记录定时器的状态

```c
/* 全局变量 */
static struct list_head timeouts = LIST_HEAD_INIT(timeouts);

/* 定时器链表节点 */
typedef void (*uloop_timeout_handler)(struct uloop_timeout *t);

struct uloop_timeout
{
	struct list_head list;		//链表头，用于链表操作
	bool pending;				//指向另一个定时器结构的指针，用于管理尚未处理的定时器

	uloop_timeout_handler cb;	//回调函数，用于处理定时器超时事件
	struct timeval time;		//用于存储超时时间
};
```



### `processes`链表

子进程管理链表,管理和处理子进程的状态和事件

```c
static struct list_head processes = LIST_HEAD_INIT(processes);

struct uloop_process
{
	struct list_head list;		//链表头，用于链表操作
	bool pending;				//进程的状态是否处于“待处理”状态

	uloop_process_handler cb;	//回调函数指针
	pid_t pid;					//管理的子进程的PID
};
```



### `signals`链表

信号管理链表,管理和处理系统信号

```c
static struct list_head signals = LIST_HEAD_INIT(signals);

typedef void (*uloop_signal_handler)(struct uloop_signal *s);

struct uloop_signal
{
	struct list_head list;			//链表头，用于链表操作
	struct sigaction orig;			//信号的原始处理器的设置
	bool pending;					//指示信号是否处于“待处理”状态

	uloop_signal_handler cb;		//回调函数指针
	int signo;						//存储了信号的编号
};
```



## 定时器链表管理

### `uloop_timeout_add`函数

新增定时器节点

**源代码如下**

> 笔者注：除注释外，所有代码均未删改

```c
int uloop_timeout_add(struct uloop_timeout *timeout)
{
	struct uloop_timeout *tmp;
	struct list_head *h = &timeouts;

	if (timeout->pending)
		return -1;

    /*
     * 遍历整个定时链表
     * 比较需要插入的节点的时间和当前链表元素的时间，将较大的值采用尾插法插入
     */
	list_for_each_entry(tmp, &timeouts, list) {
		if (tv_diff(&tmp->time, &timeout->time) > 0) {
			h = &tmp->list;
			break;
		}
	}

	list_add_tail(&timeout->list, h);
	timeout->pending = true;

	return 0;
}
```

**核心思想**

整个定时器链表以`time`排序遵循递增的顺序，将时间较大的元素插入尾部



### `uloop_clear_timeouts`函数

函数主要作用是，清空定时器链表

**源代码如下**

> 笔者注：除注释外，所有代码均未删改

```c
static void uloop_clear_timeouts(void)
{
	struct uloop_timeout *t, *tmp;
	
    /* 遍历定时器链表 */
	list_for_each_entry_safe(t, tmp, &timeouts, list)
		uloop_timeout_cancel(t);
}

int uloop_timeout_cancel(struct uloop_timeout *timeout)
{
	if (!timeout->pending)
		return -1;

	list_del(&timeout->list);
	timeout->pending = false;

	return 0;
}
```

**核心思想**

遍历链表，从链表中删除遍历到的每一个元素，并将进程状态`p->pending`置为`flase`



### `uloop_process_timeouts`函数

定时器链表管理**核心接口**。遍历整个链表执行已经到达时间的节点回调，并清除定时器

**源代码如下**

> 笔者注：除注释外，所有代码均未删改

```c
static void uloop_process_timeouts(void)
{
	struct uloop_timeout *t;
	struct timeval tv;

	if (list_empty(&timeouts))
		return;

	uloop_gettime(&tv);
	while (!list_empty(&timeouts)) {
        /* 获取定时器链表时间和当前时间比较 */
		t = list_first_entry(&timeouts, struct uloop_timeout, list);
		if (tv_diff(&t->time, &tv) > 0)
			break;

        /* 清除超时的定时器，并执行定时器回调 */
		uloop_timeout_cancel(t);
		if (t->cb)
			t->cb(t);
        
	}
}
```

**核心思想**

遍历整个定时器链表，将时间超过当前时间的节点清除，然后执行对应回调



## 子进程管理链表管理

### `uloop_process_add`函数

添加新节点

**源代码如下**

> 笔者注：除注释外，所有代码均未删改

```c
int uloop_process_add(struct uloop_process *p)
{
	struct uloop_process *tmp;
	struct list_head *h = &processes;

	if (p->pending)
		return -1;

	list_for_each_entry(tmp, &processes, list) {
		if (tmp->pid > p->pid) {
			h = &tmp->list;
			break;
		}
	}

	list_add_tail(&p->list, h);
	p->pending = true;

	return 0;
}
```

**核心思想**

按照升序排序将新节点插入链表中



### `uloop_clear_processes`函数

清空链表

**源代码如下**

> 笔者注：除注释外，所有代码均未删改

```c
static void uloop_clear_processes(void)
{
	struct uloop_process *p, *tmp;

	list_for_each_entry_safe(p, tmp, &processes, list)
		uloop_process_delete(p);
}

int uloop_process_delete(struct uloop_process *p)
{
	if (!p->pending)
		return -1;

	list_del(&p->list);
	p->pending = false;

	return 0;
}
```

**核心思想**

遍历链表，从链表中删除遍历到的每一个元素，并将进程状态`p->pending`置为`flase`



### `uloop_handle_processes`函数

子进程管理核心接口，

**源代码如下**

> 笔者注：除注释外，所有代码均未删改

```c
static void uloop_handle_processes(void)
{
	struct uloop_process *p, *tmp;
	pid_t pid;
	int ret;

	do_sigchld = false;

	while (1) {
		pid = waitpid(-1, &ret, WNOHANG);
		if (pid < 0 && errno == EINTR)
			continue;

		if (pid <= 0)
			return;
        /*
         * 遍历链表，直到找到与子进程匹配的pid
         * 删除节点并执行节点对应回调
         */
		list_for_each_entry_safe(p, tmp, &processes, list) {
			if (p->pid < pid)
				continue;

			if (p->pid > pid)
				break;

			uloop_process_delete(p);
			p->cb(p, ret);
		}
	}
}
```



## 信号管理链表管理

### `uloop_signal_add`函数

添加新的信号到信号管理链表中

**源代码如下**

> 笔者注：除注释外，所有代码均未删改

```c
int uloop_signal_add(struct uloop_signal *s)
{
	struct list_head *h = &signals;
	struct uloop_signal *tmp;
	struct sigaction sa;

	if (s->pending)
		return -1;

    /* 将传入的信号按照升序排列添加到链表中 */
	list_for_each_entry(tmp, &signals, list) {
		if (tmp->signo > s->signo) {
			h = &tmp->list;
			break;
		}
	}

	list_add_tail(&s->list, h);
	s->pending = true;

	sigaction(s->signo, NULL, &s->orig);

	if (s->orig.sa_handler != uloop_signal_wake) {
        /* 设置信号处理回调函数 uloop_signal_wake */
		sa.sa_handler = uloop_signal_wake;
		sa.sa_flags = 0;
		sigemptyset(&sa.sa_mask);
        /* 将sa.sa_handle绑定为信号s->signo的回调函数 */
		sigaction(s->signo, &sa, NULL);
	}

	return 0;
}
```

**核心思想**

信号管理链表的接口，与子进程和定时器管理链表的接口非常相似，先将节点添加到链表中然后设置回调。不同的是对于信号处理的回调函数不能直接进行绑定，需要通过`sigaction`系统调用来实现



### `uloop_signal_delete`函数

**源代码如下**

> 笔者注：除注释外，所有代码均未删改

```c
int uloop_signal_delete(struct uloop_signal *s)
{
	if (!s->pending)
		return -1;

	list_del(&s->list);
	s->pending = false;

	if (s->orig.sa_handler != uloop_signal_wake)
		sigaction(s->signo, &s->orig, NULL);

	return 0;
}
```

**核心思想**

将节点从链表中删除，并将回调函数重置



## 初始化定时器

`uloop_init`函数

```c
int uloop_init(void)
{
	if (uloop_init_pollfd() < 0)
		return -1;

	if (waker_init() < 0) {
		uloop_done();
		return -1;
	}

	uloop_setup_signals(true);

	return 0;
}
```

**核心思想**

本质是对`epoll`的一层封装，`uloop_init_pollfd`中创建`epoll`节点，后续在上层设置互斥锁防止出现死锁等问题。后续设置信号量监听回调函数



## 设置定时器超时时间

`uloop_timeout_set`函数

```c
int uloop_timeout_set(struct uloop_timeout *timeout, int msecs)
{
	struct timeval *time = &timeout->time;

	if (timeout->pending)
		uloop_timeout_cancel(timeout);

	uloop_gettime(time);

	time->tv_sec += msecs / 1000;
	time->tv_usec += (msecs % 1000) * 1000;

	if (time->tv_usec > 1000000) {
		time->tv_sec++;
		time->tv_usec -= 1000000;
	}

	return uloop_timeout_add(timeout);
}
```

**核心思想**

获取当前系统时间，然后在此基础上设置超时时间，将待设置的配置添加到`uloop`的链表中



### `uloop_run_timeout`函数

核心接口，对定时器链表和子进程链表中的

```c
int uloop_run_timeout(int timeout);

static inline int uloop_run(void)
{
	return uloop_run_timeout(-1);
}

int uloop_run_timeout(int timeout)
{
	int next_time = 0;

	uloop_run_depth++;

	uloop_status = 0;
	uloop_cancelled = false;
	do {
		uloop_process_timeouts();				// 检测uloop链表中所有元素，将超时的时间从链表中删除

		if (do_sigchld)							// 是否收到SIGCHLD信号，若收到则为true
			uloop_handle_processes();			// 检查所有已结束的子进程，在链表中删除节点，并调用回调函数处理

		if (uloop_cancelled)
			break;

		next_time = uloop_get_next_timeout();	// 计算下个事件超时离当前时间多久
		if (timeout >= 0 && (next_time < 0 || timeout < next_time))
				next_time = timeout;
		uloop_run_events(next_time);			//通过epoll监听文件句柄，超时时间设置为next_time
	} while (!uloop_cancelled && timeout < 0);

	--uloop_run_depth;

	return uloop_status;
}
```

**核心思想**

最外层记录循环的深度，内层使用一个大的`while`循环作为函数的核心逻辑，使用`uloop_cancelled`和`timeout`共同控制循环



## 监听`epoll`事件

### `uloop_fetch_events`函数

```c
static int cur_nfds;

static void uloop_run_events(int64_t timeout)
{
    ...
	cur_nfds = uloop_fetch_events(timeout);
    ...
}

static int uloop_fetch_events(int timeout)
{
	int n, nfds;

	nfds = epoll_wait(poll_fd, events, ARRAY_SIZE(events), timeout);
	for (n = 0; n < nfds; ++n) {
		struct uloop_fd_event *cur = &cur_fds[n];
		struct uloop_fd *u = events[n].data.ptr;
		unsigned int ev = 0;

		cur->fd = u;
		if (!u)
			continue;

		if (events[n].events & (EPOLLERR|EPOLLHUP)) {
			u->error = true;
			if (!(u->flags & ULOOP_ERROR_CB))
				uloop_fd_delete(u);
		}

		if(!(events[n].events & (EPOLLRDHUP|EPOLLIN|EPOLLOUT|EPOLLERR|EPOLLHUP))) {
			cur->fd = NULL;
			continue;
		}

		if(events[n].events & EPOLLRDHUP)
			u->eof = true;

		if(events[n].events & EPOLLIN)
			ev |= ULOOP_READ;

		if(events[n].events & EPOLLOUT)
			ev |= ULOOP_WRITE;

		cur->events = ev;
	}

	return nfds;
}
```

**核心思想**

本质是对`epoll_wait`的一层封装，函数内部的`cur_fds`为全局变量，循环中的主要操作实际上是对这个全局数组进行赋值的操作，和将`epoll`中的句柄取出来赋值的操作。
