# syslog原理分析

参考文档

[RFC 3164: The BSD Syslog Protocol (rfc-editor.org)](https://www.rfc-editor.org/rfc/rfc3164)

[syslog协议介绍_syslog是什么协议-CSDN博客](https://blog.csdn.net/chdhust/article/details/50989785)

[sysklogd源码](https://github.com/troglobit/sysklogd)

[BusyBox源码](https://www.busybox.net/)



## 概述

syslog是一个协议规范，需要有server端和client端，由server端收集日志并记录，通常使用busybox所提供的server端服务，底层会使用sysklogd库提供的接口