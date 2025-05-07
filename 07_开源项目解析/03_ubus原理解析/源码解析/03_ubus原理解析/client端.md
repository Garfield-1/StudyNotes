# ubus client端原理分析

> 笔者注：本文分析的是ubus源码中的官方demo，源码见：ubus/examples/client.c



## 整体流程

整体流程上，`ubus`的运行需要三部分`ubusd`守护进程、`server`端和`client`端，本文对于`client`端的实现和`client`端与守护进程和`server`端的交互进行分析

**函数调用栈**

```c
main
    ->uloop_init
    ->ubus_connect
    	//这里创建了一个struct ubus_context变量，注册了各个回调
    	//将各种请求添加到对应的各个实践链表和一个avl树中
        ->ubus_connect_ctx
    //使用uloop监听刚才创建的ctx->sock.cb，在其对应的回调函数中去处理守护进程发来的消息
    ->ubus_add_uloop
```

这里需要特别的对`ubus_connect_ctx`函数展开解读，这个函数中执行了关键的初始化动作



**ubus_connect_ctx函数**

> 笔者注：为了方便阅读，此处省略了部分逻辑

```c
int ubus_connect_ctx(struct ubus_context *ctx, const char *path)
{
	uloop_init();
	memset(ctx, 0, sizeof(*ctx));

	ctx->sock.fd = -1;
	ctx->sock.cb = ubus_handle_data;
	ctx->connection_lost = ubus_default_connection_lost;
	ctx->pending_timer.cb = ubus_process_pending_msg;
	
    //UBUS_MSG_CHUNK_SIZE是65536
	ctx->msgbuf.data = calloc(1, UBUS_MSG_CHUNK_SIZE);
	ctx->msgbuf_data_len = UBUS_MSG_CHUNK_SIZE;

	INIT_LIST_HEAD(&ctx->requests);
	INIT_LIST_HEAD(&ctx->pending);
	INIT_LIST_HEAD(&ctx->auto_subscribers);
	avl_init(&ctx->objects, ubus_cmp_id, false, NULL);
    //path含义是守护进程与client端通信使用的socket可以在程序启动时输入参数来指定
    //如果没有指定。那么ctx->sock.fd使用默认值UBUS_UNIX_SOCKET
    //UBUS_UNIX_SOCKET在编译时指定，通常是/var/run/ubus/ubus.sock
	ubus_reconnect(ctx, path);

	return 0;
}

//这里展开ubus_reconnect核心内容
->ubus_reconnect
    //UBUS_UNIX_SOCKET通常是var/run/ubus/ubus.sock
    if (!path)
        path = UBUS_UNIX_SOCKET;
	ctx->sock.fd = usock(USOCK_UNIX, path, NULL);
```

**核心思想**

`ubus_connect_ctx`函数主要用于创建新的`ubus`对象`ctx`，这个对象使用全局变量进行存储。这里主要是创建`ctx`对应的句柄、回调并将事件注册到对应的链表和一个`avl`树中

**需要分析一下这些回调和链表还有这个avl树的作用是什么？**



## client端与ubus守护进程通信

### 守护进程接收client端消息

`ubus`的守护进程`ubusd`和`client`端进程的通信是依靠于`socket`句柄，在程序启动时可以使用参数指定使用什么句柄，如果没有指定那么就会使用`cmake`中指定默认句柄路径，`cmake`中会为`UBUS_UNIX_SOCKET`宏去赋值

在程序运行时就会使用`UBUS_UNIX_SOCKET`中存储的句柄来通信

**通信过程函数调用栈**

```c
//这里添加一个ubus对象,添加ubus对象的动作无法由client端自己完成，只能发消息给ubusd守护进程
->ubus_add_object
    ->ubus_start_request
        ->__ubus_start_request
            ->ubus_send_msg
    			//这里使用sendmsg函数通过fg去发送消息给守护进程
    			//fd实际上是ctx->sock.fd这个句柄会被默认配置成/var/run/ubus/ubus.sock
    			->sendmsg(fd, &msghdr, 0)
```



### clinet接收守护进程消息

`main`中会使用`ubus_add_uloop`监听`ctx->sock.cb`，并在回调函数`ubus_handle_data`中去处理守护进程发送的消息

**对应源码**

```c
void __hidden ubus_handle_data(struct uloop_fd *u, unsigned int events)
{
	struct ubus_context *ctx = container_of(u, struct ubus_context, sock);
	int recv_fd = -1;

	while (1) {
		if (!ctx->stack_depth)
			ctx->pending_timer.cb(&ctx->pending_timer);

		if (!get_next_msg(ctx, &recv_fd))
			break;
		ubus_process_msg(ctx, &ctx->msgbuf, recv_fd);
		if (uloop_cancelling() || ctx->cancel_poll)
			break;
	}

	if (!ctx->stack_depth)
		ctx->pending_timer.cb(&ctx->pending_timer);

	if (u->eof)
		ctx->connection_lost(ctx);
}
```

**核心思想**

这里可以看到函数中使用了一个`while(1)`的循环，在循环中会处理所有的与守护进程通信使用的句柄发送的消息，直到完全处理结束



## 各个DEMO测试模块

在`ubus`的`examples`目录下有数个以`test_`开头的函数，组成了这个演示`demo`

`client`端的演示中，最重要的是`client_main`函数



**client_main函数**

> 笔者注：为了方便阅读，此处省略了部分逻辑

```c
static void client_main(void)
{
	static struct ubus_request req;
	uint32_t id;

    ubus_add_object(ctx, &test_client_object);

	ubus_lookup_id(ctx, "test", &id);

	blob_buf_init(&b, 0);
	blobmsg_add_u32(&b, "id", test_client_object.id);
	ubus_invoke(ctx, id, "watch", b.head, NULL, 0, 3000);
	test_client_notify_cb(&notify_timer);

	blob_buf_init(&b, 0);
	blobmsg_add_string(&b, "msg", "blah");
	ubus_invoke_async(ctx, id, "hello", b.head, &req);
	req.fd_cb = test_client_fd_cb;
	req.complete_cb = test_client_complete_cb;
	ubus_complete_request_async(ctx, &req);

	uloop_timeout_set(&count_timer, 2000);

	uloop_run();
}
```

**核心思想**

`client`端的演示主要从这个函数开始

首先是通过`ubus_add_object`把`test_client_object`注册到`ubus`中，然后通过`ubus_lookup_id`找到`"test"`对应的`id`，然后调用了`"test"`的`"watch"`方法和`"hello"`方法。经过走读代码发现`"test"`实际上是在`server`端中声明的一个`ubus`接口。其中的`test_client_notify_cb`则是用于统计性能



### server端的"test"对象

**"watch"方法回调**

`watch`方法非常简单，在`client`端中发送数据，在`server`端中接收数据然后打印出来

* **client端**

  封装`ubus`消息，将 `test_client_object.id`封装在`id`字段中发送

  ```c
  blob_buf_init(&b, 0);
  blobmsg_add_u32(&b, "id", test_client_object.id);
  ubus_invoke(ctx, id, "watch", b.head, NULL, 0, 3000);
  ```

* **server端**

  从解析`ubus`消息然后从中读取`WATCH_ID`这个字段，然后打印到标准输出中

  ```c
  static int test_watch(struct ubus_context *ctx, struct ubus_object *obj,
  		      struct ubus_request_data *req, const char *method,
  		      struct blob_attr *msg)
  {
  	struct blob_attr *tb[__WATCH_MAX];
  	int ret;
  
  	blobmsg_parse(watch_policy, __WATCH_MAX, tb, blob_data(msg), blob_len(msg));
  	if (!tb[WATCH_ID])
  		return UBUS_STATUS_INVALID_ARGUMENT;
  
  	test_event.remove_cb = test_handle_remove;
  	test_event.cb = test_notify;
  	ret = ubus_subscribe(ctx, &test_event, blobmsg_get_u32(tb[WATCH_ID]));
  	fprintf(stderr, "Watching object %08x: %s\n", blobmsg_get_u32(tb[WATCH_ID]), ubus_strerror(ret));
  	return ret;
  }
  ```



**"hello"方法回调**

`hello`方法回调相对来说复杂了很多，在`client`端发起请求后，`server`端响应后，会创建定时器来间隔`1S`的循环发送



### test_client_notify_cb