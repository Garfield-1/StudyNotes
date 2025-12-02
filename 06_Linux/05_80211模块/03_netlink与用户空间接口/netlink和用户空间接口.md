# netlink和用户空间交互接口

`netlink`模块提供了两个接口用来处理`802.11`的管理帧

**`NL80211_CMD_FRAME`**对应`nl80211_tx_mgmt`函数。这个函数把管理帧从用户空间接收进内核，然后交给内核/驱动去发送；在需要时（没有设置 `DONT_WAIT_FOR_ACK`）它还会把一个包含 `cookie` 的 `netlink` 回复发回用户空间

**`NL80211_CMD_REGISTER_FRAME`**对应`nl80211_register_mgmt`函数。用来处理来自用户空间的“注册接收管理帧”请求，函数本身接收并解析 `userspace` 的 `netlink` 属性，然后调用`cfg80211_mlme_register_mgmt` 在内核中登记过滤/回调。登记成功后，内核在收到匹配的管理帧时会把这些帧转发回用户空间（内核 `-> userspace`，异步发生）