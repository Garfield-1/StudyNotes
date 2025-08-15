# hostapad event loop模块梳理

`event loop`模块（即`eloop`模块）的主要作用是提供一个事件驱动的机制，用于管理和处理异步事件。它是`hostapd`的核心组件之一，负责协调各种事件的调度和执行。底层依靠`epoll`、`select`、`poll`、`kqueue`这些`IO`复用接口来实现

> 笔者注：**本文只关注epoll实现**



## 核心数据结构

定义了一个全局变量`static struct eloop_data eloop`，存放创建的`epoll`节点和待监听的句柄

<img src="./img/struct eloop_data.jpg" alt="struct eloop_data"  />



## 核心流程

整体流程与`uloop`高度相似，都是自身维护一个链表记录要处理的事件。同时在一个大循环中不停的检测直到所有事件都被处理，循环之间的间隔等待依靠`epoll_wait`函数来阻塞进程，事件则是计算链表节点的时间和当前的系统时间得到

`eloop`与`uloop`的一点区别在于对于外部信号的处理不同，功能侧重不同。`uloop`侧重定时循环，`eloop`则是监听句柄是否活跃

核心的循环在`eloop_run`函数中实现，
