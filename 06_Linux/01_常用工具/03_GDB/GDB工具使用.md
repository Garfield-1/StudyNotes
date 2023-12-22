# GDB使用方式

## 1.  GDB是什么？

**GNU调试器（英语：GNU Debugger，缩写：GDB）**，是GNU软件系统中的标准调试器，此外GDB也是个具有移携性的调试器，经过移携需求的调修与重新编译，如今许多的类UNIX操作系统上都可以使用GDB，而现有GDB所能支持调试的编程语言有C、C++、Pascal以及FORTRAN

GDB具备各种调试功效，能针对计算机程序的执行进行追踪与警告，使用GDB的调试人员可以监督及修改程序的内部变量值，甚至监督与修改独立于主程序运作外，以独立个体类型调用（调用使用）的函数

## 2. gdb调试源代码流程

1. 进入GDB　　#gdb test
只需输入GDB和要调试的可执行文件即可，在GDB的启动画面中指出了GDB的版本号、遵循的许可等信息，接下来就进入了由"(gdb)"开头的命令行界面了；
2. 查看文件　　(gdb) l
在GDB中键入"l"(list)就可以查看程序的源码了，GDB列出的源代码中明确地给出了对应的行号，方便代码的定位；
3. 设置断点　　(gdb) b 6
只需在"b"(break)后加入对应的行号即可，在GDB中利用行号设置断点是指代码运行到对应行之前暂停；设置断点可以使程序执行到某个位置时暂停，程序员在该位置处可以方便地查看变量的值、堆栈情况等；
一般情况下，源代码中大家行号与用户书写程序的行号是一致的，但有时由于用户的某些编译选项会导致行号不一致的情况，因此，一定要查看在GDB中的行号；
4. 查看断点处情况　　(gdb) info b
可以键入"info b"来查看断点处情况，可以设置多个断点；
5. 运行代码　　(gdb) r
GDB默认从首行开始运行代码，键入"r"(run)即可；程序运行到断点处停止。
6. 看变量值　　(gdb) p n
在程序暂停之后，程序员可以查看断点处的相关变量值，在GDB中只需键入"p 变量名"(print)即可；
GDB在显示变量值时都会在对应值之前加上"$N"标记，它是当前变量值的引用标记，以后若想再次引用此变量，就可以直接写作"$N"，而无需写冗长的变量名；
7. 观察变量　　(gdb) watch n
在某一循环处，往往希望能够观察一个变量的变化情况，这时就可以键入命令"watch"来观察变量的变化情况，GDB在"n"设置了观察点；
8. 单步运行　　(gdb) n
单步运行是指一次只运行一条语句，这样可以方便查看程序运行的结果，在此处只需键入"n"(next)即可；随着程序的单步运行，当变量n的值发生变化时，GDB就会自动显示出n的变化情况。
9. 程序继续运行　　(gdb) c
命令"c"(continue)可以使程序继续往下运行，直到再次遇到断点或程序结束；
10. 退出GDB　　(gdb) q
只需使用指令"q"(quit)即可。

## vscode使用图形化界面GDB调试

1. 安装微软官方提供的C/C++插件

2. 配置将文件launch.json改为如下

   ```cpp
   {
       // 使用 IntelliSense 了解相关属性。 
       // 悬停以查看现有属性的描述。
       // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
       "version": "0.2.0",
       "configurations": [
           {
               "name": "(gdb) 启动", //配置名称，显示在配置下拉菜单中
               "type": "cppdbg", //配置类型
               "request": "launch", //请求配置类型，可以是启动或者是附加
               "program": "${fileDirname}/a.out", //程序可执行文件的完整路径，${workspaceFolder}表示远程连接的初始路径
               "args": [], //传递给程序的命令行参数
               "stopAtEntry": true, //可选参数，如果为true,调试程序应该在入口（main）处停止
               "cwd": "${workspaceFolder}", //用于查找依赖项和其他文件的当前工作目录
               "environment": [], //表示要预设的环境变量
               "externalConsole": false, //如果为true，则为调试对象启动控制台
               "MIMode": "gdb", //要连接到的控制台启动程序
               "setupCommands": [ //为了安装基础调试程序而执行的一个或多个GDB/LLDB命令
                   {
                       "description": "开启pretty-printer",
                       "text": "-enable-pretty-printing",
                       "ignoreFailures": true
                   },
                   {
                       "description": "设置反汇编风格为inter",
                       "text": "-gdb-set disassembly-flavor intel",
                       "ignoreFailures": true
                   }
               ]
           }
       ]
   }
   ```

## 调试时单步执行遇到库函数

* 在调试时如果单步执行遇到库函数`srand(time(NULL));`则GDB会提示无法加载源文件

  ![无法加载源文件](./image/01_无法加载源文件.png)

* 此时代码中的`time(NULL)`函数指向libc.so.6库，由于在GDB配置路径中不存在对应版本的glibc源代码所以无法正常跳转至函数定义处

  >  /lib/x86_64-linux-gnu/libc.so.6是Linux操作系统中的动态链接库，全称为C标准库。在Linux系统中，所有使用C语言编写的程序都需要使用C标准库提供的函数、变量和常量。C标准库的主要功能包括文件操作、字符串操作、数学计算、内存管理、时间处理、网络通信等方面。C标准库是Linux操作系统中的一项核心组件，对于开发Linux应用程序非常重要。/lib/x86_64-linux-gnu/libc.so.6这个文件是C标准库的本地动态链接库，也就是所谓的libc库。在运行Linux程序时，如果发现程序需要用到C标准库中的函数或变量，就会自动调用该库中相应的代码来完成操作

  ![程序启动时链接libc.so.6](./image/02_%E9%93%BE%E6%8E%A5libc.so.6%E5%BA%93.png)

* 通过ldd --version、直接执行对应的libc.so.6文件都可以找到自身系统的libc.so.6对应的glibc版本是多少

  ![查看libc.so.6对应版本号](./image/03_%E6%9F%A5%E7%9C%8Blibc.so.6%E5%AF%B9%E5%BA%94%E7%89%88%E6%9C%AC%E5%8F%B7.png)

* 下载对应版本的glibc源代码，并将源代码目录放在配置文件中的`"cwd": "${workspaceFolder}/glibc-2.31",`中，此时再使用gdb调试遇到库函数时便会跳转至glibc源码中对应的函数实现

  ![time函数](./image/04_time函数.png)
