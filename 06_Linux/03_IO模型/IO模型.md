# Linux系统常用IO模型

Linux系统在访问设备的时候，存在以下几种IO模型：

1. `Blocking IO Model，阻塞IO模型`；
2. `Nonblocking I/O Model，非阻塞IO模型`；
3. `I/O Multiplexing Model，IO多路复用模型`;
4. `Signal Driven I/O Model，信号驱动IO模型`；
5. `Asynchronous I/O Model，异步IO模型`；

IO多路复用机制，在Linux中是通过`select/poll/epoll`机制来实现的