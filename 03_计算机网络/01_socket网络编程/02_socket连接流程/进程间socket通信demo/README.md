# 进程间socket通信demo

程序分为`server`端，和`client`端，使用`socket`进行通信，端口使用`12345`



## 编译

执行如下代码进行编译

```shell
g++ server.cpp -o server
g++ client.cpp -o client
```

## 运行

分别在两个终端执行

首先运行`server`端

```shell
./server
```

然后运行`client`端

```shell
./client
```

## 结果

`server`端的终端可见打印如下

```shell
Server start
Server listening on port 12345...
Received from client: Hello, Server!
```

`client`端的终端可见打印如下

```shell
Connected to server.
Message sent to server: Hello, Server!
Received from server: Hello, Client!
```

