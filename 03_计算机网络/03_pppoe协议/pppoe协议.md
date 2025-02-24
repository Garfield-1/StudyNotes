# pppoe协议



## pppoe协议是什么

PPPoE（Point-to-Point Protocol over Ethernet，以太网点对点协议）是一种网络协议，用于通过以太网连接在用户计算机和互联网服务提供商（ISP）之间建立点对点连接。PPPoE结合了PPP协议的功能和以太网的便利性，常用于DSL宽带连接。

PPPoE的主要功能包括：

1. **认证**：通过用户名和密码验证用户身份。
2. **加密**：提供数据加密，确保数据传输的安全性。
3. **压缩**：支持数据压缩，提高传输效率。
4. **IP地址分配**：动态分配IP地址给用户设备。



## pppoe连接流程

PPPoE连接的流程主要包括发现阶段、会话阶段、认证阶段和数据传输阶段。每个阶段涉及的报文如下：

1. **发现阶段**：
   - **PADI（PPPoE Active Discovery Initiation）**：客户端发送PADI报文，寻找可用的PPPoE服务器。
   - **PADO（PPPoE Active Discovery Offer）**：PPPoE服务器响应PADI报文，发送PADO报文，提供服务。
   - **PADR（PPPoE Active Discovery Request）**：客户端选择一个PPPoE服务器，发送PADR报文，请求建立会话。
   - **PADS（PPPoE Active Discovery Session-confirmation）**：PPPoE服务器响应PADR报文，发送PADS报文，确认会话建立。
2. **会话阶段**：
   - **LCP（Link Control Protocol）**：客户端和服务器之间进行LCP协商，配置和测试数据链路。
3. **认证阶段**：
   - **PAP（Password Authentication Protocol）或CHAP（Challenge Handshake Authentication Protocol）**：客户端通过PAP或CHAP协议进行身份验证。
4. **数据传输阶段**：
   - **IPCP（IP Control Protocol）**：客户端和服务器之间进行IPCP协商，分配IP地址和其他网络配置参数。
   - **数据报文**：客户端和服务器之间进行实际的数据传输。

以下是PPPoE连接流程的示意图：

```shell
客户端             			PPPoE服务器
 |                 				   |
 |---- PADI (发现) ---------------->|
 |<--- PADO (提供) -----------------|
 |---- PADR (请求) ---------------->|
 |<--- PADS (确认) -----------------|
 |                 				   |
 |---- LCP (链路控制协议) ---------->|
 |<--- LCP (链路控制协议) -----------|
 |                 				   |
 |---- PAP/CHAP (认证) ------------>|
 |<--- PAP/CHAP (认证) -------------|
 |                 				   |
 |---- IPCP (IP控制协议) ----------->|
 |<--- IPCP (IP控制协议) ------------|
 |                 				   |
 |---- 数据传输 -------------------->|
 |<--- 数据传输 ---------------------|
```

通过上述流程，PPPoE协议实现了在以太网上建立点对点连接，并进行认证和数据传输