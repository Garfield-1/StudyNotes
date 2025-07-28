# dhcp协议

## dhcp协议是什么

DHCP（动态主机配置协议，Dynamic Host Configuration Protocol）是一种网络管理协议，用于自动分配IP地址和其他网络配置参数（如子网掩码、网关、DNS服务器等）给网络中的设备，使它们能够在IP网络中进行通信。DHCP协议简化了网络管理员的工作，避免了手动配置每个设备的IP地址

## dhcp流程

DHCP的工作流程大致如下：

1. **DHCP发现（DHCP Discover）**：客户端设备发送一个广播消息，寻找可用的DHCP服务器。
2. **DHCP提供（DHCP Offer）**：DHCP服务器响应客户端的请求，提供一个可用的IP地址和其他网络配置参数。
3. **DHCP请求（DHCP Request）**：客户端选择一个DHCP服务器的提供，并请求使用该服务器提供的IP地址。
4. **DHCP确认（DHCP Acknowledgment）**：DHCP服务器确认客户端的请求，并正式分配IP地址和其他网络配置参数。
