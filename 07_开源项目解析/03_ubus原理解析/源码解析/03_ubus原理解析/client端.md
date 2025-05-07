# ubus client端原理分析

> 笔者注：本文分析的是ubus源码中的官方demo，源码见：ubus/examples/client.c



## 整体流程

整体流程上，`ubus`的运行需要三部分

1. `ubusd`守护进程
2. `server`端
3. `client`端

从日志上初步的看`client`端就是在一直不断的向`server`端发起请求，一直不断的去打印信息



**函数调用栈**

```c
main
    ->uloop_init
    ->ubus_connect
    	//这里创建了一个struct ubus_context变量，注册了各个回调
    	//将各种请求添加到对应的各个实践链表和一个avl树中
        ->ubus_connect_ctx
    //这里会将创建的新的socket句柄添加到uloop中去
    ->ubus_add_uloop
```



**ubus_connect_ctx函数**

`ubus_connect_ctx`函数主要用于创建新的`ubus`对象，这个对象使用全局变量进行存储。这里主要是创建`ubus`对应的句柄、回调并将事件注册到对应的链表和一个`avl`树中

需要分析一下这些回调和链表还有这个`avl`树的作用是什么？

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
	if (ubus_reconnect(ctx, path)) {
		free(ctx->msgbuf.data);
		ctx->msgbuf.data = NULL;
		return -1;
	}

	return 0;
}
```



## client端与ubus守护进程通信

### 守护进程接收client端消息

`ubus`的守护进程`ubusd`和`client`端进程的通信是依靠于`socket`句柄，在程序启动时可以使用参数指定使用什么句柄，如果没有指定那么就会使用`cmake`中指定默认句柄路径，`cmake`中会为`UBUS_UNIX_SOCKET`宏去赋值

在程序运行时就会使用`UBUS_UNIX_SOCKET`中存储的句柄来通信

**通信句柄的配置函数调用栈**

```c
//在main函数中可以指定通信使用的句柄
main
    ->ubus_connect
        ->ubus_connect_ctx
    		//如果之前的流程中没有指定通信使用的句柄
    		//在这里就会使用UBUS_UNIX_SOCKET就是var，它的值是在cmake中被指定的
            ->ubus_reconnect
              if (!path)
                  //UBUS_UNIX_SOCKET就是var/run/ubus/ubus.sock
                  path = UBUS_UNIX_SOCKET;
              ctx->sock.fd = usock(USOCK_UNIX, path, NULL);
```



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

`ubus`中会使用`uloop`监听一个`struct ubus_context *ctx`变量，这里监听的句柄实际上是守护进程与`clinet`通信的句柄，并在回调函数`ubus_handle_data`中使用一个死循环去处理守护进程发送的消息

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

这里可以看到函数中使用了一个`while(1)`的循环，在循环中会处理所有的与守护进程通信使用的句柄发送的消息，知道完全处理结束