# uloop源码解析



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

struct uloop_timeout timeout;	//创建uloop_timeout全局变量

int frequency = 5; 				//每隔5秒超时一次

static void timeout_cb(struct uloop_timeout *t)
{
	printf("[%s] uloop test \n", __FUNCTION__);
	uloop_timeout_set(t, frequency * 1000);//设置下次的超时时间
}

int main()
{	
	uloop_init();			//使用库初始化	
	timeout.cb = timeout_cb;
	uloop_timeout_set(&timeout, frequency * 1000);//设置下次的超时时间
	uloop_run();
	uloop_done();
}
```

上述程序效果：每隔`5S`打印一次



## 核心数据结构

`uloop_timeout`定时器节点

```c
typedef void (*uloop_timeout_handler)(struct uloop_timeout *t);

struct uloop_timeout
{
	struct list_head list;		//指向下一个定时器结构的指针，用于链表操作
	bool pending;				//指向另一个定时器结构的指针，用于管理尚未处理的定时器

	uloop_timeout_handler cb;	//回调函数，用于处理定时器超时事件
	struct timeval time;		//用于存储超时时间
};
```



`uloop`链表，链表中的节点使用`uloop_timeout`

```c
struct list_head {
	struct list_head *next;
	struct list_head *prev;
};
```



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



`uloop_timeout_add`函数

**核心逻辑**

```c
int uloop_timeout_add(struct uloop_timeout *timeout)
{
	struct uloop_timeout *tmp;
	struct list_head *h = &timeouts;

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



### uloop运行

`uloop_run`函数

```c
int uloop_run_timeout(int timeout);

static inline int uloop_run(void)
{
	return uloop_run_timeout(-1);
}
```

### `uloop_run_timeout`函数

```c
int uloop_run_timeout(int timeout)
{
	int next_time = 0;

	uloop_run_depth++;

	uloop_status = 0;
	uloop_cancelled = false;
	do {
		uloop_process_timeouts();// 检测uloop链表中所有元素，将超时的时间从链表中删除

		if (do_sigchld)// 是否收到SIGCHLD信号，若收到则为true
			uloop_handle_processes();// 检查所有已结束的子进程，在链表中删除节点，并调用回调函数处理

		if (uloop_cancelled)
			break;

		next_time = uloop_get_next_timeout();// 获取下一个超时时间的超时时间
		if (timeout >= 0 && (next_time < 0 || timeout < next_time))
				next_time = timeout;
		uloop_run_events(next_time);
	} while (!uloop_cancelled && timeout < 0);

	--uloop_run_depth;

	return uloop_status;
}
```

**核心思想**

最外层记录循环的深度，内层使用一个大的`while`循环作为函数的核心逻辑



## 监听`epoll`事件

### `uloop_fetch_events`函数

```c
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
