# ubus源码解析

> 笔者注：ubus项目内容繁杂相关解析文档的编写非一日之功，本文从ubus的官方demo实现进行切入，逐步分析ubus的实现原理

## 大纲（后续删除）

**本文档尚未完成，此大纲仅用于梳理思路**

* `ubus`底层使用`socket`建立通信连接
* `ubus`有一个发送消息队列

在`ubus`中，并没有单独的线程去维护请求队列。请求队列中的事件是在特定的函数调用时被处理的。具体来说，`ubus`使用事件循环 (`uloop`) 来处理请求队列中的事件

### 事件循环和请求处理

`ubus`使用 `uloop` 事件循环来处理请求和其他事件。当事件循环运行时，它会处理所有挂起的事件，包括请求的响应。以下是主要的处理流程：

1. **添加请求到队列**：
    - 当一个请求被发起时，例如通过 `ubus_complete_request_async` 函数，请求会被添加到上下文的请求队列中。
2. **事件循环处理**：
    - 事件循环 (`uloop_run`) 会处理所有挂起的事件，包括请求的响应。
    - 当有数据可读时，`ubus_handle_data` 函数会被调用，进而调用 `ubus_process_msg` 函数处理消息。
3. **处理挂起的消息**：
    - 如果在处理消息时，发现当前调用栈深度不为零（即 `ctx->stack_depth` 不为零），消息会被加入到挂起的消息队列中。
    - 当调用栈深度恢复到零时，会通过 `ubus_process_pending_msg` 函数处理挂起的消息。

### 关键代码

以下是相关的代码片段：

**添加请求到队列**

```c
void ubus_complete_request_async(struct ubus_context *ctx, struct ubus_request *req)
{
  if (!list_empty(&req->list))
     return;

  list_add(&req->list, &ctx->requests);
}
```

**事件循环处理**

```c
void ubus_handle_data(struct uloop_fd *u, unsigned int events)
{
  struct ubus_context *ctx = container_of(u, struct ubus_context, sock);
  struct ubus_msghdr_buf buf;
  int fd;

  while (ubus_recv_msg(ctx, &buf, &fd) > 0)
     ubus_process_msg(ctx, &buf, fd);
}
```

**处理挂起的消息**

```c
static void ubus_process_pending_msg(struct uloop_timeout *timeout)
{
  struct ubus_context *ctx = container_of(timeout, struct ubus_context, pending_timer);
  struct ubus_pending_msg *pending;
  while (!list_empty(&ctx->pending)) {
     if (ctx->stack_depth)
       break;

     pending = list_first_entry(&ctx->pending, struct ubus_pending_msg, list);
     list_del(&pending->list);

     ubus_process_msg(ctx, &pending->hdr, -1);
     free(pending);
  }
}
```

### 总结

`ubus`并没有单独的线程去维护请求队列。相反，它依赖于事件循环 (`uloop`) 来处理请求队列中的事件。当事件循环运行时，它会处理所有挂起的事件，包括请求的响应。因此，只有在事件循环运行时，程序才会处理请求队列中的事件。

----

## ubus简介

`ubus`是`OpenWrt`中的进程间通信机制，类似于桌面版`linux`的`dbus`，`Android`的`binder`。`ubus`基于`unix socket`实现，`socket`绑定到一个本地文件，具有较高的效率；

`unix socket`是`C/S`模型，建立一个`socket`连接，`server`端和`client`端分别要做如下步骤：

1. 建立一个`socket server`端，绑定到一个本地`socket`文件，监听`client`的连接
2. 建立一个或多个`socket client`端，连接到`server`端
3. `client`端和`server`端相互发送消息
4. `client`端或`server`端收到对方消息后，针对具体消息进行相应处理

![ubus架构图](.\img\ubus架构图.jpg)

`ubus`同样基于这套流程，其中`ubusd`实现`server`，其他进程实现`client`，例如`ubus(cli)`、`netifd`、`procd`；两个`client`通信需要通过`ubusd`转发

## 核心数据结构

### **struct ubus_context**

```c
struct ubus_context
{
	struct list_head requests;
	struct avl_tree objects;
	struct list_head pending;

	struct uloop_fd sock;
	struct uloop_timeout pending_timer;

	uint32_t local_id;
	uint16_t request_seq;
	bool cancel_poll;
	int stack_depth;

	void (*connection_lost)(struct ubus_context *ctx);
	void (*monitor_cb)(struct ubus_context *ctx, uint32_t seq, struct blob_attr *data);

	struct ubus_msghdr_buf msgbuf;
	uint32_t msgbuf_data_len;
	int msgbuf_reduction_counter;

	struct list_head auto_subscribers;
	struct ubus_event_handler auto_subscribe_event_handler;
};
```

**结构图**

<img src="./img/%E6%A0%B8%E5%BF%83%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84.jpg" alt="核心数据结构" style="zoom:200%;" />

其中的`AVL`树的部分，通过`ubus_add_object_cb`接口添加节点`ubus_remove_object_cb`接口删除节点，待后续完善
