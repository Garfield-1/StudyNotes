# hostapad event loop模块梳理

`event loop`模块（即`eloop`模块）的主要作用是提供一个事件驱动的机制，用于管理和处理异步事件。它是`hostapd`的核心组件之一，负责协调各种事件的调度和执行



## 对外API

