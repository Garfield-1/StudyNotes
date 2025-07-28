# Linux环境下coredump配置

设置产生coredump文件的大小，命令如下：

ulimit -c unlimited

ubuntu系统下如果可修改/proc/sys/kernel/core_pattern文件的内容指定coredump文件的位置，如写入core则在当前目录下生成一个名称为core的dump文件
