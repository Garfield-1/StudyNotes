# 802.11帧的Reason Code位和Status Code位

`802.11`帧主要有三种类型数据帧、控制帧、管理帧

本文讲到的`Reason Code`和`Status Code`就是其中的管理帧的两个位

## Reason Code位

当对方不适合加入网络时，工作站会送出`Disassociation`（解除连接）或`Deauthentication`（解除身份认证）帧作为应答。这些帧当中包含一个长度`16bit`的`Reason Code`（原因代码）位，表示对方的做法有误

| 代码 | 含义                                                         |
| ---- | ------------------------------------------------------------ |
| 0    | 保留，未使用<br>`Reserved; unused`                           |
| 1    | 未指定<br/>`Unspecified`                                     |
| 2    | 之前的身份认证无效<br/>`Prior authentication is not valid`   |
| 3    | 工作站已经离开基本服务区或扩展服务区，目前已经接触身份认证<br/>`Station has left the basic service area or extendedservice area and is deauthenticated` |
| 4    | 闲置计时器超时，且工作站已经解除连接<br/>`Inactivity timer expired and station was disassociated` |
| 5    | 基站资源不足，因此解除连接<br/>`Disassociated due to insufficient resources at the access point` |
| 6    | 从尚未认证的工作站所收到的帧类型或次类型不正确<br/>`Incorrect frame type or subtype received from unauthenticated station` |
| 7    | 从尚未连接的工作站所收到的帧类型或者次类型不正确<br/>`Incorrect frame type or subtype received from unassociated station` |
| 8    | 工作站已经离开基本服务区或扩展服务区，目前已经解除连接<br/>`Station has left the basic service area or extended service area and is disassociated` |
| 9    | 在身份认证完成之前要求连接或者重新连接<br/>`Association or reassociation requested before authentication is complete` |
| 10   | `802.11h`无法接受`Power Capability`信息元素的设定值，因此解除连接<br/>`Disassociated because of unacceptable values in Power Capability element` |
| 11   | `802.11h`无法接受`Supported Channels`信息元素的设定值，因此解除连接<br/>`Disassociated because of unacceptable values in Supported Channels element` |
| 12   | 保留<br>`Reserved`                                           |
| 13   | `802.11i`信息元素不正确（ `802.11i` 所加入的原因代码， 因此应指`802.11i`的信息元素）<br/> `Invalid information element (added with 802.11i, and likely one of the 802.11i information elements` |
| 14   | `802.11i`数据完整性检验失败<br/>`Message integrity check failure` |
| 15   | `802.11i`四道密钥磋商超时<br/>`4-way keying handshake timeout` |
| 16   | `802.11i`群组密钥磋商超时<br/>`Group key handshake timeou`t  |
| 17   | `802.11i`四道磋商信息元素的安全参数与原始参数组合不符<br/>`4-way handshake information element has different security parameters from initial parameter set` |
| 18   | `802.11i`群组密码锁不正确<br/>`Invalid group cipher`         |
| 19   | `802.11i`成对密码锁不正确<br/>`Invalid pairwise cipher`      |
| 20   | `802.11i`身份认证与密钥管理协议不正确<br/>`Invalid Authentication and Key Management Protocol` |
| 21   | `802.11i`未支持的固安网络信息元素版本<br/>`Unsupported Robust Security Network Information Element (RSN IE) version` |
| 22   | `802.11i RSN IE`的性能项不正确<br/>`Invalid capabilities in RSN information element` |
| 23   | `802.11i 802.1X`身份认证失败<br/>`802.1X authentication failure` |
| 24   | `802.11i`所设定的使用政策拒绝所提议的密钥锁组<br/>`Proposed cipher suite rejected due to configured policy` |
| 25   | 保留，未使用<br/>`Reserved; unused`                          |

## Status Code 位

状态代码用来表示某项过程成功或失败。 `Status Code`（状态代码）位如果某项过程成功，该位的值就会被设定为`0`，否则设为非零值

| **代码** | **含义**                                                     |
| -------- | ------------------------------------------------------------ |
| 0        | 操作成功<br/>`Operation completed successfully`              |
| 1        | 未指定失败原因<br/>`Unspecified failure`                     |
| 2-9      | 保留，未使用<br/>`Reserved; unused`                          |
| 10       | 所要求的能力范围过广无法支持<br/>`Requested capability set is too broad and cannot be supported` |
| 11       | 拒绝重新连接； 之前的连接无法辨识与转移<br/>`Reassociation denied; prior association cannot be identified and transferred` |
| 12       | 拒绝重新连接， 原因不在 802.11 标准指定范围内<br/>`Association denied for a reason not specified in the 802.11 standard` |
| 13       | 不支持所使用的身份认证算法<br/>`Requested authentication algorithm not supported` |
| 14       | 超乎预期的身份认证序号<br/>`Unexpected authentication sequence number` |
| 15       | 身份认证被拒绝；回应信息有误<br/>`Authentication rejected; the response to the challenge failed` |
| 16       | 身份认证被拒绝；下一个帧并未出现在预定的期间<br/>`Authentication rejected; the next frame in the sequence did not arrive in the expected window` |
| 17       | 连接被拒绝；基站资源有限<br/>`Association denied; the access point is resource-constrained` |
| 18       | 连接被拒绝； 工作站并未支持 BSS 要求的所有数据传输速率<br/>`Association denied; the mobile station does not support all of the data rates required by the BSS` |
| 19       | `802.11b`连接被拒绝； 工作站并未支持`Short Preamble`功能<br/>`Association denied; the mobile station does not support the Short Preamble option` |
| 20       | `802.11b`连接被拒绝；工作站并未支持`PBCC`调制技术<br/>`Association denied; the mobile station does not support the PBCC modulation option` |
| 21       | 802.11b连接被拒绝；工作站并未支持`Channel Agility`功能<br/>`Association denied; the mobile station does not support the Channel Agility option` |
| 22       | `802.11h`连接被拒绝；需要`Spectrum Management`功能<br/>`Association denied; Spectrum Management is required` |
| 23       | `802.11h`连接被拒绝；不接受`Power Capability`<br/>`Association denied; Power Capability value is not acceptable` |
| 24       | `802.11h`连接被拒绝；不接受`Supported Channels`<br/>`Association denied; Supported Channels is not acceptable` |
| 25       | `802.11g`连接被拒绝；工作站并未支持`Short slot Time`功能<br/>`Association denied; the mobile station does not support the Short Slot Time` |
| 26       | `802.11g`连接被拒绝； 工作站并未支持 DSSS-OFDM 功能<br/>`Association denied; the mobile station does not support DSSS-OFDM` |
| 27-39    | 保留<br/>`Reserved`                                          |
| 40       | `802.11i`信息元素不正确<br/>`Information element not valid`  |
| 41       | `802.11i`群组（广播/组播） 密码锁不正确<br/>`Group (broadcast/multicast) cipher not valid` |
| 42       | `802.11i`成对（单点传播）密码锁不正确<br/>`Pairwise (unicast) cipher not valid` |
| 43       | `802.11i`身份认证与密钥管理协议不正确<br/>`Authentication and Key Management Protocol (AKMP) not valid` |
| 44       | `802.11i`未支持的固安网络信息元素版本<br/>`Robust Security Network information element (RSN IE) version is not supported` |
| 45       | `802.11i`不支持`RSN IE`性能<br/>`RSN IE capabilites are not supported` |
| 46       | `802.11i`密码锁组被使用政策拒绝<br/>`Cipher suite rejected due to policy` |
| 47-63    | `535`保留给未来的标准使用<br/>`Reserved for future standardization work` |
