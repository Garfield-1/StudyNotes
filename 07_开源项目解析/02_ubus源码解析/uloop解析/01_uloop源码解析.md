# uloop源码解析

> 参考文档 [openWrt libubox组件之uloop原理分析 - 老小白爱学习 - 博客园 (cnblogs.com)](https://www.cnblogs.com/laoxiaobaiup/p/9366880.html)

## uloop是什么

`uloop`是`libubox`下的一个模块，有三个功能：文件描述符触发事件的监控（监控指定的`fd`依靠`epoll`实现），`timeout`定时器处理， 当前进程的子进程的维护



## uloop整体结构

![01_uloop_run处理时序](.\img\01_uloop_run处理时序.png)

　`uloop_run`轮询处理定时器、进程、描述符事件。

- 遍历定时器`timeouts`链表判断是否有定时器超时，如果有则进行相应的回调处理，没有跳过。
- 判断是否有子进程退出`SIGCHLD`信号，有就会遍历`processes`进程处理的链表，调用相应的回调函数，没有跳过。
- 计算出距离下一个最近的定时器的时间，作为文件描述符事件`epoll`的超时时间。然后`epoll`进行事件监听，如果有文件描述符准备就绪(可读写时间)则调用相应的回调函数，或者有信号进行中断`epoll`返回停止监听，否则`epoll`阻塞直到超时时间完成。



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



## 定时器事件

### 定时器timeout结构

```c
struct uloop_timeout
{
    struct list_head list; //链表节点
    bool pending;      //添加一个新的timeout pending是true， false删除该节点timeout
    uloop_timeout_handler cb; //超时处理函数
    struct timeval time;   //超时时间
};
```

### 定时器使用接口

- 注册一个新定时器

  ```c
  int uloop_timeout_add(struct uloop_timeout *timeout)
  ```

　　用户不直接使用，内部接口，被接口`uloop_timeout_set`调用。

　　将定时器插入到timeouts链表中，该链表成员根据超时时间从小到大排列。

 ![03_定时器流程](E:\work\git_project\StuduNode\07_开源项目解析\02_ubus源码解析\uloop解析\img\03_定时器流程.png)

- 设置定时器超时时间(毫秒)，并添加

  ```c
  int uloop_timeout_set(struct uloop_timeout *timeout, int msecs)
  ```

　　如果`pending`为`true`,则从定时器链表中删除原先已存在的定时器。

　　设置定时器的超时时间点。

　　调用`uloop_timeout_add`接口将该定时器加入到定时器链表中。

- 销毁指定定时器

  ```c
  int uloop_timeout_cancel(struct uloop_timeout *timeout)
  ```

　　从定时器链表中删除指定定时器。

- 获取定时器还剩多长时间超时

  ```c
  int uloop_timeout_remaining(struct uloop_timeout *timeout)　　
  ```

　　这里`pending`标记可判断定时器是否处于生命周期，如果尚处在生命周期内，则返回离定时器超时还有多少时间，单位为毫秒。

### 定时器的使用

`uloop`提供了定时器接口

使用如下

```c
#include <stdio.h>
#include <uloop.h>

struct uloop_timeout timeout;	//创建uloop_timeout全局变量
 
int frequency = 5; //每隔5秒超时一次
 
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

上述程序允许效果：每隔5S打印一次

### 定时器功能流程

遍历定时器链表，如果有定时器已经超时，执行该定时器的回调函数。



## 描述符事件

### 文件描述符uloop结构

```c
struct uloop_fd
{
    uloop_fd_handler cb; /*文件描述符对应的处理函数 */
    int fd;       /*文件描述符*/
    bool eof;      /*EOF*/
    bool error;     /*出错*/
    bool registered;   /*是否已经添加到epoll的监控队列*/
    uint8_t flags;    /*ULOOP_READ | ULOOP_WRITE | ULOOP_BLOCKING等*/ 
};
```

### 描述符uloop使用接口

- 注册一个新描述符到事件处理循环

　　`int uloop_fd_add(struct uloop_fd *sock, unsigned int flags)`

　　`uloop`最多支持`10`个描述符事件。

- 从事件处理循环中销毁指定描述符

　　`int uloop_fd_delete(struct uloop_fd *sock)`

### 描述符事件流程

###  ![02_描述符事件流程](.\img\02_描述符事件流程.png)



## 进程事件

### 进程事件处理结构

```c
struct uloop_process
{
    struct list_head list;       
    bool pending;         
    uloop_process_handler cb; /** 文件描述符， 调用者初始化 */
    pid_t pid;         /** 文件描述符， 调用者初始化 */
};
```

### 进程事件使用接口

- 注册新进程到事件处理循环

  ```c
  int uloop_process_add(struct uloop_process *p)
  ```

　　将进程事件插入到进程事件链表中，链表根据`PID`从小到大排序。

　　其中`p->proc.pid`为注册到`uloop`监控的进程ID。

　　`P->cb`为进程退出的回调函数，类型为：

 ```c
  typedef void (*uloop_process_handler)(struct uloop_process *c, int ret)
 ```


- 从事件处理循环中销毁指定进程

  ```c
  int uloop_process_delete(struct uloop_process *p)
  ```

　　从进程事件处理链表中删除该进程事件。

### 进程事件处理流程

### ![04_进程事件处理流程](.\img\04_进程事件处理流程.png)
