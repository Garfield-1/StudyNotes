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
