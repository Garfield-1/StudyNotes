# Linux模块相关API

[TOC]

## 模块入口和出口注册API

在各个`linux`子模块中经常可以看到的`module_init`和`module_exit`两个宏，这分别注册了模块的初始化和结束部分需要调用的函数

* **module_init(demo_init)：**
    `module_init`宏的作用是告诉内核，当这个模块被加载时，应该执行名为 `demo_init` 的函数。`demo_init` 函数通常包含模块启动时需要执行的所有初始化代码，比如分配资源、注册设备驱动、创建内核线程等。如果 `demo_init` 函数成功执行并返回 `0`，模块就会成功加载；如果返回错误码，模块加载就会失败

* **module_exit(demo_exit)：**
    `module_exit`宏的作用是告诉内核，当这个模块被卸载时，应该执行名为 `demo_exit` 的函数。`demo_exit` 函数负责执行模块退出时所需的所有清理工作，例如释放在 `demo_init` 中分配的资源、注销设备驱动、停止内核线程等。确保所有资源都被正确释放对于维持系统稳定性至关重要

从下文中可以看到`module_init`和`module_exit`宏有着两种不同的实现方式，使用了`MODULE`编译参数来进行区分，其主要的作用是**控制最终编译产物是.KO文件还是完整的Linux镜像**。不管是哪一种情况，二者最终展开都是一系列宏的嵌套，宏的最终展开是的`inline`函数和`GCC`编译参数。也就是说这一系列的逻辑实际上会**在程序的编译阶段完成**

### module_init原理分析

查看`Linux 5.4`源码可以看到`module_init`宏对应源代码，可以看到`module_init`对应着两种不同的实现。`MODULE` 宏本身并不是直接定义某个值或函数，而是一个**编译时开关**。编译器通常会通过命令行参数（例如 `-DMODULE`）来定义 `MODULE` 宏

```c
//linux-5.4/include/linux/module.h
#ifndef MODULE
#define module_init(x)    __initcall(x);
#else
#define module_init(initfn)                    \
    static inline initcall_t __maybe_unused __inittest(void)        \
    { return initfn; }                    \
    int init_module(void) __copy(initfn) __attribute__((alias(#initfn)));
#endif
```

1. **`MODULE` 宏开启时的逻辑**

    **源代码：**

    ```c
    //linux-5.4/include/linux/module.h
    #define module_init(initfn)                    \
        static inline initcall_t __maybe_unused __inittest(void)        \
        { return initfn; }                    \
        int init_module(void) __copy(initfn) __attribute__((alias(#initfn)));
    ```

    **核心思想：**

    当文件被编译成内核模块也就是`.KO`文件时，会有明确的 `init_module` 函数作为加载时的入口点。这段宏代码的目的是将用户提供的初始化函数 (`initfn`) 设置为内核模块的官方入口点 (`init_module`)。它通过`GCC`的 `alias` 属性使得 `init_module` 直接指向 `initfn` 的代码

2. **`MODULE` 宏关闭（默认情况）时的逻辑**

    **源代码：**

    通过查看`linux 5.4`的对应源代码，展开各个宏可以看到最终的调用

    ```c
    //linux-5.4/include/linux/init.h
    #define ___define_initcall(fn, id, __sec) \
        static initcall_t __initcall_##fn##id __used \
            __attribute__((__section__(#__sec ".init"))) = fn;
    
    #define __define_initcall(fn, id) ___define_initcall(fn, id, .initcall##id)
    
    #define device_initcall(fn)        __define_initcall(fn, 6)
    
    #define __initcall(fn) device_initcall(fn)
    
    #define module_init(x)    __initcall(x);
    ```

    **核心思想：**

    主要的目的是修改最终编译生成的`ELF`二进制文件中的部分，将需要执行的函数指针放在某个特定的位置

    `___define_initcall` 宏的目的是获取一个函数 `fn`，并创建一个指向该函数的静态函数指针。这个函数指针会被赋予一个基于 `fn` 和 `id` 的唯一名称，并被标记为“已使用”（`__used`），以防止被编译器优化掉。最关键的是，这个函数指针会被放置在一个特殊的内存段中，段名由 `__sec` 参数和 `.init` 后缀构成（例如 `".initcall0.init"`、`".initcall1.init"` 等）

### module_exit原理分析

查看`Linux 5.4`源码可以看到`module_exit`宏对应源代码，可以看到`module_exit`对应着两种不同的实现。`MODULE` 宏本身并不是直接定义某个值或函数，而是一个**编译时开关**。编译器通常会通过命令行参数（例如 `-DMODULE`）来定义 `MODULE` 宏

```c
//linux-5.4/include/linux/module.h
#ifndef MODULE
#define module_exit(x)    __exitcall(x);
#else
#define module_exit(exitfn)                    \
    static inline exitcall_t __maybe_unused __exittest(void)        \
    { return exitfn; }                    \
    void cleanup_module(void) __copy(exitfn) __attribute__((alias(#exitfn)));
#endif
```

1. **MODULE宏开启时的逻辑**

    **源代码：**

    ```c
    //linux-5.4/include/linux/module.h
    #define module_exit(exitfn)                    \
        static inline exitcall_t __maybe_unused __exittest(void)        \
        { return exitfn; }                    \
        void cleanup_module(void) __copy(exitfn) __attribute__((alias(#exitfn)));
    ```

    **核心思想：**

    当文件被编译成内核模块也就是`.KO`文件时，会有明确的 `cleanup_module` 函数作为卸载时的入口点。将 `cleanup_module` 函数设置为 `exitfn` 函数的一个别名。`#exitfn` 使用预处理器的字符串化操作符将 `exitfn` 转换为字符串字面量。因此，当内核调用 `cleanup_module` 时，实际上会直接执行开发者提供的 `exitfn` 函数的代码

2. **MODULE宏关闭时的逻辑**

    **源代码：**

    ```c
    //linux-5.4/include/linux/module.h
    #define __exit_call    __used __section(.exitcall.exit)
    
    #define __exitcall(fn)                        \
        static exitcall_t __exitcall_##fn __exit_call = fn
    
    #define module_exit(x)    __exitcall(x)
    ```

    **核心思想：**

    综上所述，`__exitcall` 宏的目的是提供一种机制，用于注册在程序或模块退出时需要调用的函数。它通过创建一个静态的、具有唯一名称的函数指针，并将其初始化为指定的函数地址。通过 `.exitcall.exit` 段属性，所有这样定义的函数指针会被链接器收集到内存中的一个特定区域。之后，在退出流程中，系统可以遍历这个区域，并调用所有注册的退出函数。

## 参考文档

> [Release v5.4 · torvalds/linux](https://github.com/torvalds/linux/releases/tag/v5.4)
>
> [Arch Linux](https://www.archlinuxcn.org/)
