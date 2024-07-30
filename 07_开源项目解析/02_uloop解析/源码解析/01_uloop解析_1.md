# uloop源码解析

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

`uloop_run_timeout`函数

```c
int uloop_run_timeout(int timeout)
{
	int next_time = 0;

	uloop_run_depth++;

	uloop_status = 0;
	uloop_cancelled = false;
	do {
		uloop_process_timeouts();

		if (do_sigchld)
			uloop_handle_processes();

		if (uloop_cancelled)
			break;

		next_time = uloop_get_next_timeout();
		if (timeout >= 0 && (next_time < 0 || timeout < next_time))
				next_time = timeout;
		uloop_run_events(next_time);
	} while (!uloop_cancelled && timeout < 0);

	--uloop_run_depth;

	return uloop_status;
}
```

