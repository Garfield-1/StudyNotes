# 符号导出

> 参考文档：
>
> [Release v5.4 · torvalds/linux](https://github.com/torvalds/linux/releases/tag/v5.4)
>
> [Arch Linux](https://www.archlinuxcn.org/)
>
> [改进 EXPORT_SYMBOL（） [LWN.net\]](https://lwn.net/Articles/674303/)



## 概述

`Linux`中一组`API`使得开发人员可以将需要的符号导出，如下

* **EXPORT_SYMBOL**

    `EXPORT_SYMBOL` 是一个在 Linux 内核代码中使用的宏。它的主要作用是将一个符号（通常是一个函数名或变量名）导出，使其可以被内核的其他部分或者动态加载的内核模块`LKMs`访问

* **EXPORT_SYMBOL_GPL**

    `EXPORT_SYMBOL_GPL(save_stack_trace);` 的作用是将函数 `save_stack_trace` 导出到内核符号表中，但**仅限于遵循 GPL (GNU General Public License) 兼容许可证的内核模块使用**

查看源码可以看到，上述的两个宏其实都是`__EXPORT_SYMBOL`的一层封装

```c
//linux-5.4/include/linux/export.h
#define EXPORT_SYMBOL(sym)		__EXPORT_SYMBOL(sym, "")
#define EXPORT_SYMBOL_GPL(sym)		__EXPORT_SYMBOL(sym, "_gpl")
```



## __EXPORT_SYMBOL原理分析

`__EXPORT_SYMBOL`的基本原理是，将符号表导出到`ELF`文件一个特殊的以符号名称命名的特殊的内存节中，当内核或内核模块被加载时，链接器和加载器可以扫描这些特殊的节，收集所有的符号条目，从而构建一个可供查找的内核符号表。这使得内核模块可以访问和调用核心内核或其他模块导出的函数和变量

**源代码：**

```c
//linux-5.4/include/linux/export.h
#define __KSYMTAB_ENTRY(sym, sec)					\
	static const struct kernel_symbol __ksymtab_##sym		\
	__attribute__((section("___ksymtab" sec "+" #sym), used))	\
	__aligned(sizeof(void *))					\
	= { (unsigned long)&sym, __kstrtab_##sym, NULL }

struct kernel_symbol {
	unsigned long value;
	const char *name;
	const char *namespace;
};

#define ___EXPORT_SYMBOL(sym, sec)					\
	___export_symbol_common(sym, sec);				\
	__KSYMTAB_ENTRY(sym, sec)

#define __EXPORT_SYMBOL(sym,sec)	___EXPORT_SYMBOL(sym,sec)
```

**核心思想：**

通过`__KSYMTAB_ENTRY`将符号导出到ELF文件的内存段中，具体的流程是：

1. 创建一个名为 `__ksymtab_my_function` 的 `struct kernel_symbol` 变量
2. 将这个变量存储在一个特殊的、以符号名命名的内存节中（例如 `"___ksymtab+my_function"`）
3. 用 `my_function` 的实际地址和指向其名称字符串 `"my_function"` 的指针来初始化这个变量

这样，当内核或内核模块被加载时，链接器和加载器可以扫描这些特殊的节，收集所有的符号条目，从而构建一个可供查找的内核符号表。这使得内核模块可以访问和调用核心内核或其他模块导出的函数和变量

