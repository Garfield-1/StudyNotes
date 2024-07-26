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



## 初始化定时器

`uloop_init`函数

```c
static int uloop_init_pollfd(void)
{
	if (poll_fd >= 0)
		return 0;

	poll_fd = epoll_create(32);
	if (poll_fd < 0)
		return -1;

	fcntl(poll_fd, F_SETFD, fcntl(poll_fd, F_GETFD) | FD_CLOEXEC);
	return 0;
}
```

核心思想

