# Linux等待队列

> 参考文档：
>
> [Release v5.4 · torvalds/linux](https://github.com/torvalds/linux/releases/tag/v5.4)

## 概述

`Linux`内核的**等待队列（Wait Queue）**是重要的数据结构，与进程调度机制紧密相关联，可以用来同步对系统资源的访问、异步事件通知、跨进程通信等。
在`Linux`中，等待队列以循环链表为基础结构，包括两种数据结构：**等待队列头(wait queue head)**和**等待队列元素(wait queue)**，整个等待队列由等待队列头进行管理

## 核心原理

内核等待队列的实现依靠的是对于进程状态的管理和任务调度队列的管理来实现任务的唤醒和休眠

需要注意的是等待队列中的任务并不与线程一一对应，这个等待者可以对应多种类型，包括线程、进程、文件/poll/epoll回调型等待者

## 核心数据结构

内核中为等待队列定义了一个核心数据结构

```c
//linux-5.4/include/linux/wait.h
struct wait_queue_entry {
    unsigned int         flags;
    void                 *private;
    wait_queue_func_t    func;
    struct list_head     entry;
};

struct wait_queue_head {
    spinlock_t          lock;
    struct list_head    head;
};
typedef struct wait_queue_head wait_queue_head_t;
```

## 关键API

### 初始化

创建等待队列时会调用`DECLARE_WAITQUEUE`这个宏来创建并初始化一个等待队列节点

```cpp
#define __WAITQUEUE_INITIALIZER(name, tsk) {					\
	.private	= tsk,							\
	.func		= default_wake_function,				\
	.entry		= { NULL, NULL } }

#define DECLARE_WAITQUEUE(name, tsk)						\
	struct wait_queue_entry name = __WAITQUEUE_INITIALIZER(name, tsk)
```

这里的`default_wake_function`函数会在`__wake_up_common`函数中被调用执行

```c
static int __wake_up_common(struct wait_queue_head *wq_head, unsigned int mode,
			int nr_exclusive, int wake_flags, void *key,
			wait_queue_entry_t *bookmark)
{
	// ...
    
	list_for_each_entry_safe_from(curr, next, &wq_head->head, entry) {
		// ...
        ret = curr->func(curr, mode, wake_flags, key);
		// ...
    }
    
    // ...

	return nr_exclusive;
}
```

### 新增节点

调用`add_wait_queue`函数将创建的节点添加到等待队列中，在下一次时钟中断，进程切换时任务调度器会查看就绪队列中的进程

### 主动让出CPU

在将节点添加到等待队列后需要手动调用`schedule()`函数来让出`CPU`，因为只是将任务添加到等待中不会阻塞任务。这里主动让出`CPU`才会在下一次进程调度中将自身挂起

### 删除节点

调用`remove_wait_queue`函数将创建的节点从等待队列中删除
