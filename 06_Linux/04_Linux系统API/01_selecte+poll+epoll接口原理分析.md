# epoll接口

> 参考文档：https://www.cnblogs.com/Hijack-you/p/13057792.html
>
> [【原创】Linux select/poll机制原理分析 - LoyenWang - 博客园 (cnblogs.com)](https://www.cnblogs.com/LoyenWang/p/12622904.html)
>
> [源码解读poll/select内核机制 - Gityuan博客 | 袁辉辉的技术博客](https://gityuan.com/2019/01/05/linux-poll-select/)
>
> [Linux 5.4源码下载](https://github.com/torvalds/linux/releases/tag/v5.4)



> 笔者注：本文中出现的相关接口源代码均引用自**Linux 5.4**版本源码



## 发展历史

### **API 发布的时间线**

下文中列出了网络 IO 中，各个 api 的发布时间线

> 1983，socket 发布在 Unix(4.2 BSD)
> 1983，select 发布在 Unix(4.2 BSD)
> 1994，Linux的1.0，已经支持socket和select
> 1997，poll 发布在 Linux 2.1.23
> 2002，epoll发布在 Linux 2.5.44

可以看到`select`、`poll` 和 `epoll`，这三个“`IO`多路复用`API`”是相继发布的。这说明了，它们是`IO`多路复用的3个进化版本。因为`API`设计缺陷，无法在不改变 API 的前提下优化内部逻辑。所以用`poll`替代`select`，再用`epoll`替代`poll`

`epoll`和`poll`还有`select`都是监听`socket`的接口，`poll`还有`select`出现的时间更早，但是性能更差。后来在此继承上发展改进得到了`epoll`



## select原理分析

查看man手册可知select函数的作用

> select() and pselect() allow a program to monitor multiple file descriptors, waiting until one or more of the file descriptors become "ready" for some class of I/O oper‐ation (e.g., input possible).   A file descriptor is considered ready if it is possible to perform a corresponding I/O operation (e.g., read(2)  without  blocking,  or  a sufficiently small write(2)).
>
> The timeout argument specifies the interval that select() should block waiting for a file descriptor to become ready.   The call will block until either:
>
> *  a file descriptor becomes ready;
>
> *  the call is interrupted by a signal handler;  or
>
> *  the timeout expires.
>
>
> 译文如下：
>
> select()和pselect()允许程序监视多个文件描述符，直到一个或多个文件描述符“准备好”进行某种I/O操作(例如，可能的输入)。如果可以执行相应的I/O操作(例如，无阻塞的读操作(2)或足够小的写操作(2))，则认为文件描述符已准备就绪。
>
> timeout参数指定select()应该阻止等待文件描述符准备就绪的时间间隔。调用将被阻塞，直到:
>
> 文件描述符准备就绪;
>
> 调用被信号处理程序中断;
>
> 超时过期

### 示例代码

监听标准输入，设置超时时间为5S

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
	fd_set rfds;
	struct timeval tv;
	int retval;

	/* Watch stdin (fd 0) to see when it has input. */

	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

	/* Wait up to five seconds. */

	tv.tv_sec = 5;
	tv.tv_usec = 0;

	retval = select(1, &rfds, NULL, NULL, &tv);
	/* Don't rely on the value of tv now! */

	if (retval == -1)
		perror("select()");
	else if (retval)
		printf("Data is available now.\n");
		/* FD_ISSET(0, &rfds) will be true. */
	else
		printf("No data within five seconds.\n");

	exit(EXIT_SUCCESS);
}
```

### 相关接口

`fd_set`结构定义如下

```c
#define FD_SETSIZE 256
typedef struct { uint32_t fd32[FD_SETSIZE/32]; } fd_set;
```

`FD_ZERO` 、`FD_SET`、 `FD_CLR`、`FD_ISSET`是 `POSIX` 标准中定义的宏，用于操作文件描述符集合（`fd_set`）。

1. **FD_ZERO：** 这个宏用于将文件描述符集合清零，即清除集合中的所有文件描述符。通常用于初始化文件描述符集合，使其为空集。其原型如下：

   ```c
   void FD_ZERO(fd_set *set);
   ```

   其中，`set` 是一个指向 `fd_set` 结构体的指针，表示要清零的文件描述符集合。

2. **FD_SET：** 这个宏用于将一个文件描述符添加到文件描述符集合中。其原型如下：

   ```c
   void FD_SET(int fd, fd_set *set);
   ```

   其中，`fd` 是要添加的文件描述符，`set` 是一个指向 `fd_set` 结构体的指针，表示要添加文件描述符的文件描述符集合。

3. **FD_CLR：** 用于从文件描述符集合中清除（移除）一个文件描述符。

   ```c
   void FD_CLR(int fd, fd_set *set);
   ```

   其中，`fd` 是要清除的文件描述符，`set` 是一个指向 `fd_set` 结构体的指针，表示要清除文件描述符的文件描述符集合。

4. **FD_ISSET：** 用于检查文件描述符集合中是否包含某个文件描述符。

   ```c
   int FD_ISSET(int fd, const fd_set *set);
   ```

   其中，`fd` 是要检查的文件描述符，`set` 是一个指向 `fd_set` 结构体的指针，表示要检查的文件描述符集合。如果文件描述符集合中包含指定的文件描述符，则返回非零值；否则返回零。

### select函数调用栈

![01_select函数调用栈](.\img\01_select函数调用栈.png)

#### 源码分析

`select`方法的主要工作可分为3部分：

- 将需要监控的用户空间的`inp`(可读)、`outp`(可写)、`exp`(异常)事件拷贝到内核空间`fds`的`in`、`out`、`ex`；
- 执行`do_select`()方法，将`in`、`out`、`ex`监控到的事件结果写入到`res_in`、`res_out`、`res_ex`；
- 将内核空间`fds`的`res_in`、`res_out`、`res_ex`事件结果信息拷贝回用户空间`inp`、`outp`、`exp`。

`select`系统调用，最终的核心逻辑是在`do_select`函数中处理的，源码如下

### kern_select函数

计算函数超时时间，然后调用`core_sys_select`执行真正的`select`

**核心逻辑如下**

> 笔者注：下文代码已格式化处理，并适当简化只保留核心逻辑

```c
static int kern_select(int n, fd_set __user *inp, fd_set __user *outp,
		       fd_set __user *exp, struct timeval __user *tvp)
{
	struct timespec64 end_time, *to = NULL;
	struct timeval tv;
	int ret;

	if (tvp) {
        // 从用户空间读取设置的超时时间
		if (copy_from_user(&tv, tvp, sizeof(tv)))
			return -EFAULT;

		to = &end_time;
        // 设置函数的的超时时间
		if (poll_select_set_timeout(to,
				tv.tv_sec + (tv.tv_usec / USEC_PER_SEC),
				(tv.tv_usec % USEC_PER_SEC) * NSEC_PER_USEC))
			return -EINVAL;
	}

	ret = core_sys_select(n, inp, outp, exp, to);
	return poll_select_finish(&end_time, tvp, PT_TIMEVAL, ret);
}
```

### core_sys_select函数

**核心逻辑如下**

> 笔者注：下文代码已格式化处理，并适当简化只保留核心逻辑

```c
int core_sys_select(int n, fd_set __user *inp, fd_set __user *outp,
			   fd_set __user *exp, struct timespec64 *end_time)
{
	fd_set_bits fds;
	void *bits;
	int max_fds;
	size_t size, alloc_size;
	struct fdtable *fdt;
	long stack_fds[SELECT_STACK_ALLOC/sizeof(long)];

	fdt = files_fdtable(current->files);
	// select可监控不能大于进程可打开的文件描述上限
	max_fds = fdt->max_fds;
	if (n > max_fds)
		n = max_fds;

	size = FDS_BYTES(n);
	bits = stack_fds;
	// 6个bitmaps对应(int/out/ex)
	alloc_size = 6 * size;
	bits = kvmalloc(alloc_size, GFP_KERNEL);

	fds.in      = bits;
	fds.out     = bits +   size;
	fds.ex      = bits + 2*size;
	fds.res_in  = bits + 3*size;
	fds.res_out = bits + 4*size;
	fds.res_ex  = bits + 5*size;

	/*
	 * 拷贝传入的inp、outp、exp的值存入fds的in、out、ex
	 * 此处是将从用户空间传入的保存，后续传入内核空间
	 */
	get_fd_set(n, inp, fds.in);
	get_fd_set(n, outp, fds.out);
	get_fd_set(n, exp, fds.ex);

	// 将fds的res_in、res_out、res_ex内容清零
	zero_fd_set(n, fds.res_in);
	zero_fd_set(n, fds.res_out);
	zero_fd_set(n, fds.res_ex);

	do_select(n, &fds, end_time);

	/*
	 * 使用入参的指针变量将执行后的fds结果传出
	 * 将fds的res_in、res_out、res_ex结果拷贝到用户空间inp、outp、exp
	 * 此处是将从内核空间的结果拷贝至用户空间
	 */
	set_fd_set(n, inp, fds.res_in);
	set_fd_set(n, outp, fds.res_out);
	set_fd_set(n, exp, fds.res_ex);

	if (bits != stack_fds)
		kvfree(bits);

	return -EFAULT;
}
```

**核心思想**

`select`接口使用`fd_set_bits`结构保存记录文件描述符，`core_sys_select`函数中创建对应类型的变量。记录从用户空间传入的待检测文件描述符，使用`do_select`执行实际的监听动作，然后将结果传回用户空间。

> 笔者注：严格地说数据在用户空间和内核空间互相传递并不是在这里完成的，经过`SYSCALL_DEFINE5(select)`的调用后已经进入内核态中。这里只是为方便理解而这样表述

### do_select函数

**函数核心思想**

函数主要做的事情，遍历传入的文件描述符检查哪些有变化

**函数整体采用了三层循环的设计，在下文中对这三层循环每一次进行讨论**

**第一层循环**

* 第一层循环，即最外层循环

  最外层循环做的事情主要是
  
  1. 申请和销毁资源，校验传入的参数
  2. 创建一个高精度，可阻塞当前进程的循环
  3. 设置循环退出的出口条件

**核心逻辑如下**

> 笔者注：下文代码已格式化处理，并适当简化只保留核心逻辑

```c
static int do_select(int n, fd_set_bits *fds, struct timespec64 *end_time)
{
    struct poll_wqueues table;
    unsigned long busy_start = 0;
    int retval;

    poll_initwait(&table); // 初始化一个等待队列
	wait = &table.pt;

    slack = select_estimate_accuracy(end_time); // 获取高精度定时的误差范围，用于后续高精度定时器使用

	for (;;) {
		...
		for (...) { 		// 此处省略循环体...
			for (...) {  	// 此处省略循环体...
			}
		}

		if (retval || timed_out || signal_pending(current))
			break;

        /*
         * 低精度检测函数
         * 进行忙等待检测,检查是否超时,若未超时则进行新一轮检测
         */
		if (can_busy_loop) {
			if (!busy_start) {
				busy_start = busy_loop_current_time();
				continue;
			}
			if (!busy_loop_timeout(busy_start))
				continue;
		}

        /*
         * 高精度定时器
         * 将进程状态置为TASK_INTERRUPTIBLE，启动定时器，直到timeout后将进程状态重新置为TASK_RUNNING
         * 此处的需要等待的时间是在select调用时传入的，由入参的struct timeval tv结构体中的tv.tv_sec(秒数)和tv.tv_usec(微妙)的相加
         */
        if (!poll_schedule_timeout(wait, TASK_INTERRUPTIBLE, end_time, slack))
            timed_out = 1;
    }
	poll_freewait(&table); 	// 释放初始化申请的队列

    return retval;
}
```

**核心思想**

在函数内部的`for(;;)`死循环中，人为**构造一个高精度的循环检测**，其实现的最重要的地方在于利用`busy_loop_timeout`的**低精度检测**控制内部循环的流程，在内部流程结束后使用`poll_schedule_timeout`**高精度定时器**阻塞进程进行进程管理

笔者猜测此处使用两个检测函数的考量是，这种忙等待的场景，对于时间非常敏感每次检测超时不能太长，所以使用精度较低但是速度更快的函数控制内部循环。但是如果需要阻塞进程的话，对于时间的敏感度较高，使用开销速度较慢但是精度更高的函数进行进程的控制

最外层的一层循环，是一个死循环。循环的**退出条件**是

1. **文件描述符准备就绪**
2. **调用被信号处理程序中断**
3. **循环超时过期**

**第二层循环**

* 第二层循环，即中间层循环

  中间层循环做的事情主要是，记录第三层检测的结果

**核心逻辑如下**

> 笔者注：下文代码已格式化处理，并适当简化只保留核心逻辑

```c
static int do_select(int n, fd_set_bits *fds, struct timespec64 *end_time)
{
    // 获取给定文件描述符集合中的最大文件描述符值
    retval = max_select_fd(n, fds);
    int n = retval;

    for(...)
    {
        /*
         * 此处申请的`unsigned long inp, outp, exp`变量分别对应可读、可写、异常事件的文件描述符位图
         * 这些数据的每一位都对应着一个文件描述符，如果某个文件描述符在集合中，则对应的位被设置为 `1`；否则被设置为 `0`
         * 变量前是否以字母r开头表示是传出数据，否则为输入的传入数据
         */
        unsigned long *rinp, *routp, *rexp, *inp, *outp, *exp;
        bool can_busy_loop = false;

        inp = fds->in;
        outp = fds->out;
        exp = fds->ex;
        rinp = fds->res_in;
        routp = fds->res_out;
        rexp = fds->res_ex;

        for (i = 0; i < n; ++rinp, ++routp, ++rexp) {
            unsigned long in, out, ex, all_bits;
            unsigned long res_in = 0, res_out = 0, res_ex = 0;
            __poll_t mask;

            in = *inp++;
            out = *outp++;
            ex = *exp++;

            // 检查输入、输出和异常描述符的集合是否全为零
            all_bits = in | out | ex;
            if (all_bits == 0) {
                i += BITS_PER_LONG;
                continue;
            }

            for (...) {}// 此处省略循环体...

            // 记录检测的结果
            if (res_in)
                *rinp = res_in;
            if (res_out)
                *routp = res_out;
            if (res_ex)
                *rexp = res_ex;
        }
    }
}
```

**核心思想**

获取在第三层循环中对传入的文件描述符位图的检测结果，将第三层循环的检测的结果以`unsigned long`为单位，将结果保存至`*rinp`、`*routp`、`*rexp`中。此处存放检测结果的指针指向的内存，实际指向`do_select`函数的入参`(fd_set_bits *fds)`，所以不需要额外的参数进行传递

二层循环仅有**唯一的一个退出条件，待扫描的文件描述符位图遍历完成**

**第三层循环设计**

* 第三层循环，即最内侧循环

  最内侧循环主要是检查传入句柄是否有操作，并记录检测的结果

**核心逻辑如下**

> 笔者注：下文代码已格式化处理，并适当简化只保留核心逻辑

```c
static int do_select(int n, fd_set_bits *fds, struct timespec64 *end_time)
{
    unsigned long bit = 1, j;
    struct poll_wqueues table;
    poll_table *wait;
    __poll_t busy_flag = net_busy_loop_on() ? POLL_BUSY_LOOP : 0;

    poll_initwait(&table); // 初始化一个等待队列
    wait = &table.pt;

    retval = max_select_fd(n, fds);
    n = retval;

    retval = 0;
    for(...)
    {
        for(...)
        {
            __poll_t mask;

            all_bits = in | out | ex;
            if (all_bits == 0) {
                i += BITS_PER_LONG;
                continue;
            }
            for (j = 0; j < BITS_PER_LONG && i >= n; ++j, ++i, bit <<= 1) {
                struct fd f;

                f = fdget(i);
                if (f.file) {
                    /*
                     * fdget()读取文件描述符对应的文件对象的引用
                     * wait_key_set()设置等待键值，等待文件对象的状态变化
                     * vfs_poll()函数检查文件对象的状态变化，并获取文件对象的事件掩码
                     * fdput()释放申请的文件资源
                     */
                    wait_key_set(wait, in, out, bit, busy_flag);
                    mask = vfs_poll(f.file, wait);
                    fdput(f);
                    /*
                     * 检查文件对象的事件掩码mask中是否设置了特定的事件标志
                     * 并且这些标志是否满足了select调用中指定的相应的读、写和异常条件
                     */
                    if ((mask & POLLIN_SET) && (in & bit)) {
                        res_in |= bit;
                        retval++;
                    }
                    if ((mask & POLLOUT_SET) && (out & bit)) {
                        res_out |= bit;
                        retval++;
                    }
                    if ((mask & POLLEX_SET) && (ex & bit)) {
                        res_ex |= bit;
                        retval++;
                    }
                    // 当检测到检测的文件描述符有变化时，关闭循环标志位
                    if (retval) {
                        can_busy_loop = false;
                        busy_flag = 0;
                    } else if (busy_flag & mask)
                        can_busy_loop = true;
                }
            }
        }
    }
}
```

**核心思想**

调用`vfs_poll`获取文件对象的事件掩码，并检查其中是否包含某些特定的事件标志

执行`vfs_poll`函数实际执行`file->f_op->poll()`函数，这个`file->f_op->poll()`其实就是`file_operations->poll()`即文件系统的`poll`方法

`file_operations->poll()`是在驱动中进行实现，这个函数指针通常用于执行文件对象的轮询操作，以确定文件对象的状态是否满足特定的条件

### select接口小结

`select`接口底层使用`fd_set_bits`结构存储文件描述符位图，这个结构实际是`unsigned long`，将多个文件描述符存储在一起，每一位代表一个文件描述符。在用户配置好需要监听的文件描述符后，由用户空间传入内核空间中，调用内核文件系统提供的poll接口检测是否有变化。在获取到结果后将结果存入`fd_set_bits`结构中再从内核空间传回用户空间。

每一次的检测都需要对传入的位图进行遍历，还需要从用户空间和内核空间之间相互传递数据，同时监听的最大文件描述符受到进程可以打开的最大文件描述符数量限制。




## poll原理分析

### 示例代码

监听标准输入，设置超时时间为5秒

```c
#include <stdio.h>
#include <unistd.h>
#include <poll.h>

int main() {
    struct pollfd fds[1];
    int ret;

    fds[0].fd = STDIN_FILENO;  // 标准输入的文件描述符
    fds[0].events = POLLIN;    // 监听可读事件

    ret = poll(fds, 1, 5000);  //使用poll函数监听标准输入，设置超时时间为5000 毫秒（5秒）

    if (ret == -1) {
        perror("poll");
        return 1;
    } else if (ret == 0) {
        printf("5秒内没有事件发生\n");
    } else {
        if (fds[0].revents & POLLIN) {
            printf("标准输入已就绪\n");
        }
    }

    return 0;
}
```

### poll函数调用栈

![02_poll函数调用栈](.\img\02_poll函数调用栈.png)

### do_sys_poll函数

**核心逻辑如下**

> 笔者注：下文代码已格式化处理，并适当简化只保留核心逻辑

```c
static int do_sys_poll(struct pollfd __user *ufds, unsigned int nfds, struct timespec64 *end_time)
{
	struct poll_wqueues table;
	int len;
	long stack_pps[POLL_STACK_ALLOC/sizeof(long)];
	struct poll_list *const head = (struct poll_list *)stack_pps;
 	struct poll_list *walk = head;
 	unsigned long todo = nfds;

	/*
	 * 创建struct poll_list类型的链表，链表中存放待检测的文件描述符
	 * 将传入的待检测的文件描述符从用户空间拷贝至内核空间，然后存入链表中
	 */
	len = min_t(unsigned int, nfds, N_STACK_PPS);
	for (;;) {
		walk->next = NULL;
		walk->len = len;
		if (!len)
			break;

		copy_from_user(walk->entries, ufds + nfds-todo, sizeof(struct pollfd) * walk->len);

		todo -= walk->len;
		if (!todo)
			break;

		len = min(todo, POLLFD_PER_PAGE);
		walk = walk->next = kmalloc(struct_size(walk, entries, len), GFP_KERNEL);
	}
	/*
	 * 初始化poll等待队列
	 * 执行do_poll检测待检测的文件描述符
	 * 释放poll等待队列
	 */
	poll_initwait(&table);
	do_poll(head, &table, end_time);
	poll_freewait(&table);

	/*
	 * 检测结果存放在单链表中，头节点为head
	 * 将检测结果从内核空间拷贝至用户空间
	 */
	for (walk = head; walk; walk = walk->next) {
		struct pollfd *fds = walk->entries;
		for (int j = 0; j < walk->len; j++, ufds++)
			// __put_user函数，将数据从内核空间拷贝到用户空间
			if (__put_user(fds[j].revents, &ufds->revents))
				goto out_fds;
  	}
	// 销毁链表
	walk = head->next;
	while (walk) {
		struct poll_list *pos = walk;
		walk = walk->next;
		kfree(pos);
	}
	return -EFAULT;
}
```

**核心思想**

1. 首先创建`struct poll_list`类型的单链表，用于存储待检测的文件描述符
2. 将待检测的文件描述符从用户空间拷贝至内核空间
3. 执行`do_poll`进行检测
4. 将检测结果从内核空间拷贝至用户空间

`poll`接口的整体设计思路与`select`可以说几乎一致，都是先创建对应的数据结构然后从用户空间拷贝待检测的文件描述符，检测完成后再拷贝至内核空间。二者的区别只是在于使用了不同的数据结构，所以也使用不同的处理方式。

### do_poll函数

`do_poll`的设计为三层循环嵌套的结构。外侧循环构建了一个高精度定时循环，内层循环则用于检测和填充文件描述符链表

**第一层循环结构**

外层循环构建了一个高精度定时循环，其设计与`select`几乎一致，详细**参见前文中`do_select`函数一节**

**核心逻辑如下**

> 笔者注：下文代码已格式化处理，并适当简化只保留核心逻辑

```c
static int do_poll(struct poll_list *list, struct poll_wqueues *wait, struct timespec64 *end_time)
{
	int timed_out = 0, count = 0;
	u64 slack = 0;
	unsigned long busy_start = 0;

    // 计算高精度定时器的误差值
	slack = select_estimate_accuracy(end_time);

	for (;;) {
		struct poll_list *walk;
		bool can_busy_loop = false;

		for (...) {}//此处省略循环体

		if (count || timed_out)
			break;

        // 检查忙本轮等待是否超时
		if (can_busy_loop) {
			if (!busy_start) {
				busy_start = busy_loop_current_time();
				continue;
			}
			if (!busy_loop_timeout(busy_start))
				continue;
		}

        // 将进程置为TASK_INTERRUPTIBLE阻塞，直到timeout后将进程状态重新置为TASK_RUNNING
		if (!poll_schedule_timeout(wait, TASK_INTERRUPTIBLE, end_time, slack))
			timed_out = 1;
	}
	return count;
}
```

**核心思想**

构造一个高精度循环检测

**循环出口：内层循环设置循环结束的标志位**

**第二第三层循环结构**

内层的第二层第三层循环做的事情主要是，遍历struct poll_list结构的链表，检查对应的文件描述符是否有变化，并将结果填充至链表中

**核心逻辑如下**

> 笔者注：下文代码已格式化处理，并适当简化只保留核心逻辑

```c
static int do_poll(struct poll_list *list, struct poll_wqueues *wait, struct timespec64 *end_time)
{
	for (...) {
        struct poll_list *walk;
        bool can_busy_loop = false;

		for (walk = list; walk != NULL; walk = walk->next) {
			struct pollfd * pfd, * pfd_end;

			pfd = walk->entries;
			pfd_end = pfd + walk->len;
			for (; pfd != pfd_end; pfd++) {
				if (do_pollfd(pfd, pt, &can_busy_loop, busy_flag)) {
					count++;
					pt->_qproc = NULL;
                    //设置循环结束的标志位
					busy_flag = 0;
					can_busy_loop = false;
				}
			}
		}
	}
}
```

**核心思想**

遍历链表，检测链表中的每一个元素的文件描述符

**循环出口：链表遍历结束**

### do_pollfd函数

**核心逻辑如下**

> 笔者注：下文代码已格式化处理，并适当简化只保留核心逻辑

```c
static inline __poll_t do_pollfd(struct pollfd *pollfd, poll_table *pwait,
							bool *can_busy_poll)
{
	int fd = pollfd->fd;
	__poll_t mask = 0, filter;
	struct fd f;

	mask = EPOLLNVAL;
	f = fdget(fd);

	filter = demangle_poll(pollfd->events) | EPOLLERR | EPOLLHUP);
	pwait->_key = filter;
	mask = vfs_poll(f.file, pwait);//检查文件对象的状态变化，并获取文件对象的事件掩码
	if (mask)
		*can_busy_poll = true;
	mask &= filter;					// 只保留返回结果中与filter对应的部分
	fdput(f);

	pollfd->revents = mangle_poll(mask);
	return mask;
}
```

**核心思想**

`do_pollfd`函数的核心逻辑是调用`vfs_poll()`对待检查文件描述符是否发生变化，执行`vfs_poll`函数实际执行`file->f_op->poll()`函数，这个`file->f_op->poll()`其实就是`file_operations->poll()`即文件系统的`poll`方法

`file_operations->poll()`是在驱动中进行实现，这个函数指针通常用于执行文件对象的轮询操作，以确定文件对象的状态是否满足特定的条件

### poll接口小结

`poll`和`select`的原理几乎一致，都是将需要监听的文件描述符传入内核，然后调用`file_operations->poll`接口去检测，底层都是依赖内核的文件系统实现。二者不同的地方在于`select`是构造了一个位图然后，然后在位图中填充文件描述符，`poll`则是使用单链表。但两者的效率并没有太大的区别

由于每次的检测都会涉及到数据在用户空间到内核空间之间的来回拷贝，而且每次检测遍历所有的文件描述符，所以其效率并不高。



## file_operations->poll接口

### 函数声明

在`linux-5.4\include\linux\fs.h`中可以看到`struct file_operations`的定义

```c
#define __bitwise __attribute__((bitwise))
typedef unsigned __bitwise __poll_t;

struct file_operations {
	...
    /* read意为读、write意为写、poll意为检测，探询 */
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	__poll_t (*poll) (struct file *, struct poll_table_struct *);
	...
} __randomize_layout;
```

### 函数实现

`struct file_operations`中的`__poll_t`是在驱动代码中实现，不同驱动代码实现方式不同。但都会调用`poll_wait()`函数

在此处列出例子

在`linux-5.4\arch\powerpc\platforms\powernv\opal-prd.c`中可以找到`OPAL`的驱动对于`poll`的实现

```c
static const struct file_operations opal_prd_fops = {
	...
	.poll		= opal_prd_poll,
	...
};

static __poll_t opal_prd_poll(struct file *file,
		struct poll_table_struct *wait)
{
	poll_wait(file, &opal_prd_msg_wait, wait);

	if (!opal_msg_queue_empty())
		return EPOLLIN | EPOLLRDNORM;

	return 0;
}
```

在`linux-5.4\arch\powerpc\kernel\rtasd.c`中可以找到`RTASD`的驱动对于`poll`的实现

```c
static __poll_t rtas_log_poll(struct file *file, poll_table * wait)
{
	poll_wait(file, &rtas_log_wait, wait);
	if (rtas_log_size)
		return EPOLLIN | EPOLLRDNORM;
	return 0;
}

static const struct file_operations proc_rtas_log_operations = {
	...
	.poll =		rtas_log_poll,
	...
};
```

可以看到不同的驱动代码中都调用了`poll_wait()`，把当前进程加入到驱动里自定义的等待队列上，当驱动事件就绪后，就可以在驱动里自定义的等待队列上唤醒调用`poll`的进程。



## epoll实现原理

### epoll是什么

`epoll`是一种`I/O`事件通知机制，是`linux`内核实现`IO`多路复用的一个实现。
 `IO`多路复用是指，在一个操作里同时监听多个输入输出源，在其中一个或多个输入输出源可用的时候返回，然后对其的进行读写操作。

`epoll`的通俗解释是一种当文件描述符的内核缓冲区非空的时候，发出可读信号进行通知，当写缓冲区不满的时候，发出可写信号通知的机制



# epoll与select、poll的对比

## 1. 用户态将文件描述符传入内核的方式

- select：创建3个文件描述符集并拷贝到内核中，分别监听读、写、异常动作。这里受到单个进程可以打开的fd数量限制，默认是1024。
- poll：将传入的struct pollfd结构体数组拷贝到内核中进行监听。
- epoll：执行epoll_create会在内核的高速cache区中建立一颗红黑树以及就绪链表(该链表存储已经就绪的文件描述符)。接着用户执行的epoll_ctl函数添加文件描述符会在红黑树上增加相应的结点。

## 2. 内核态检测文件描述符读写状态的方式

- select：采用轮询方式，遍历所有fd，最后返回一个描述符读写操作是否就绪的mask掩码，根据这个掩码给fd_set赋值。
- poll：同样采用轮询方式，查询每个fd的状态，如果就绪则在等待队列中加入一项并继续遍历。
- epoll：采用回调机制。在执行epoll_ctl的add操作时，不仅将文件描述符放到红黑树上，而且也注册了回调函数，内核在检测到某文件描述符可读/可写时会调用回调函数，该回调函数将文件描述符放在就绪链表中。

## 3. 找到就绪的文件描述符并传递给用户态的方式

- select：将之前传入的fd_set拷贝传出到用户态并返回就绪的文件描述符总数。用户态并不知道是哪些文件描述符处于就绪态，需要遍历来判断。
- poll：将之前传入的fd数组拷贝传出用户态并返回就绪的文件描述符总数。用户态并不知道是哪些文件描述符处于就绪态，需要遍历来判断。
- epoll：epoll_wait只用观察就绪链表中有无数据即可，最后将链表的数据返回给数组并返回就绪的数量。内核将就绪的文件描述符放在传入的数组中，所以只用遍历依次处理即可。这里返回的文件描述符是通过mmap让内核和用户空间共享同一块内存实现传递的，减少了不必要的拷贝。

## 4. 重复监听的处理方式

- select：将新的监听文件描述符集合拷贝传入内核中，继续以上步骤。
- poll：将新的struct pollfd结构体数组拷贝传入内核中，继续以上步骤。
- epoll：无需重新构建红黑树，直接沿用已存在的即可。

## epoll更高效的原因

1. select和poll的动作基本一致，只是poll采用链表来进行文件描述符的存储，而select采用fd标注位来存放，所以select会受到最大连接数的限制，而poll不会。
2. select、poll、epoll虽然都会返回就绪的文件描述符数量。但是select和poll并不会明确指出是哪些文件描述符就绪，而epoll会。造成的区别就是，系统调用返回后，调用select和poll的程序需要遍历监听的整个文件描述符找到是谁处于就绪，而epoll则直接处理即可。
3. select、poll都需要将有关文件描述符的数据结构拷贝进内核，最后再拷贝出来。而epoll创建的有关文件描述符的数据结构本身就存于内核态中，系统调用返回时利用mmap()文件映射内存加速与内核空间的消息传递：即epoll使用mmap减少复制开销。
4. select、poll采用轮询的方式来检查文件描述符是否处于就绪态，而epoll采用回调机制。造成的结果就是，随着fd的增加，select和poll的效率会线性降低，而epoll不会受到太大影响，除非活跃的socket很多。
5. epoll的边缘触发模式效率高，系统不会充斥大量不关心的就绪文件描述符

> 虽然epoll的性能最好，但是在连接数少并且连接都十分活跃的情况下，select和poll的性能可能比epoll好，毕竟epoll的通知机制需要很多函数回调。

