# Linux环境下coredump配置

设置产生`coredump`文件的大小，命令如下：

`ulimit -c unlimited`

`ubuntu`系统下如果可修改`/proc/sys/kernel/core_pattern文`件的内容指定`coredump`文件的位置，如写入`core`则在当前目录下生成一个名称为`core`的`dump`文件
