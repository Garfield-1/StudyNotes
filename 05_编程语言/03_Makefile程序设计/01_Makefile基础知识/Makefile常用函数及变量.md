# Makefile常用函数

* GNU make 的函数提供了处理文件名、变量、文本和命令的方法。使用函数我们的 Makefile 可以书写的更加灵活和健壮

[TOC]

## 1. 函数的调用语法

* GNU make 函数的调用格式类似于变量的引用，以“$”开始表示一个引用。语法格式如下：

  ```makefile
  $(FUNCTION ARGUMENTS)
  
  # 等价于
  ${FUNCTION ARGUMENTS}
  ```
  
  * `FUNCTION`是需要调用的函数名，它需要是 make 内嵌的函数名；对于用户自己的函数需要通过 `make` 的`call`函数来间接调用
  
  * `ARGUMENTS`是函数的参数，参数和函数名之间使用若干个空格或者`[tab]字符`分割，如果存在多个参数时，参数之间使用逗号`,`分开
  
  * 函数处理参数时，参数中如果存在对其它变量或者函数的引用，首先对这些引用进行展开得到参数的实际内容。而后才对它们进行处理。参数的展开顺序是按照参数的先后顺序来进行的
  
  * 书写时，函数的参数不能出现逗号`,`和空格。这是因为逗号被作为多个参数的分隔符
  
    ```makefile
    # 当有逗号或者空格作为函数的参数时，需要把它们赋值给一个变量，在函数的参数中引用这个变量来实现
    
    comma:= , 
    empty:= 
    space:= $(empty) $(empty) 
    foo:= a b c 
    bar:= $(subst $(space),$(comma),$(foo))
    ```

## 2. 文本处理函数

### 2.1 字符串替换函数—subst

* 函数原型：**$(subst FROM,TO,TEXT)**

* 函数功能：把字串“TEXT”中的“FROM”字符替换为“TO”

* 返回值：替换后的新字符串

* 示例：

  ```makefile
  $(subst FROM,TO,TEXT)
  
  # 示例
  $(subst ee, EE , test ee)    #test EE
  ```

### 2.2 模式替换函数—patsubst

* 函数原型：**$(patsubst PATTERN,REPLACEMENT,TEXT)** 
* 函数功能：搜索`TEXT`中以空格分开的单词，将否符合模式`PTATTERN`替换为`REPLACEMENT`。参数`PATTERN`中可以使用模式通配符`%`来代表一个单词中的若干字符。在参数中如果需要将第一个出现的`%`作为字符本身而不作为模式字符时，可使用反斜杠`\`进行转义处理

* 返回值：替换后的新字符串

* 函数说明：参数“TEXT”单词之间的多个空格在处理时被合并为一个空格，并忽略

  前导和结尾空格

* 示例：

  ```makefile
  $(patsubst %.c,%.o,x.c.c bar.c)     #x.c.o bar.o
  
  # 把字串“x.c.c bar.c”中以.c 结尾的单词替换成以.o 结尾的字符。函数的返回结果是“x.c.o bar.o”
  ```

### 2.3 去空格函数—strip

* 函数原型：**$(strip STRINT)**

* 函数功能：去掉字符串`STRINT`的开头和结尾的空字符，并将其中多个连续空字符合并为一个空字符

* 返回值：无前导和结尾空字符、使用单一空格分割的多单词字符串

* 函数说明：空字符包括空格、`[Tab]`等不可显示字符

* 示例：

  ```makefile
  STR =        a          b  c 
  LOSTR = $(strip $(STR))
  all:
  	$(info $(STR))              #        a          b  c
  	$(info $(LOSTR))            #a b c
  
  # strip”函数经常用在条件判断语句的表达式中，确保表达式比较的可靠和健壮
  ```

### 2.4 查找字符串函数—findstring

* 函数原型： **$(findstring FIND,IN)** 

* 函数功能：在搜索字符串`IN`，查找`FIND`字符串

* 返回值：如果在`IN`之中存在`FIND`，则返回`FIND`，否则返回空

* 函数说明：字符串`IN`之中可以包含空格、`[Tab]`。搜索需要是严格的文本匹配

* 示例：

  ```makefile
  $(info $(findstring a,a b c))
  $(info $(findstring a,b c))
  
  # 第一个函数结果是字“a”；第二个值为空字符
  ```

### 2.5 过滤函数—filter

* 函数原型：**$(filter PATTERN…,TEXT)**

* 函数功能：过滤掉字串`TEXT`中所有不符合模式`PATTERN`的单词，保留所有符合此模式的单词。可以使用多个模式。模式中一般需要包含模式字符`%`；存在多个模式时，模式表达式之间使用空格分割

* 函数说明：`filter`函数可以用来去除一个变量中的某些字符串

* 示例：

  ```makefile
  sources := foo.c bar.c baz.s ugh.h 
  
  all :
  	$(info $(filter %.c %.s,$(sources)))
  # 返回值为：foo.c bar.c baz.s
  ```

### 2.6 反过滤函数—filter-out

* 函数原型：**$(filter-out PATTERN...,TEXT)**

* 函数功能：和`filter`函数实现的功能相反。过滤掉字串`TEXT`中所有符合模式`PATTERN`的单词，保留所有不符合此模式的单词。可以有多个模式。存在多个模式时，模式表达式之间使用空格分割

* 返回值：空格分割的`TEXT`字串中所有不符合模式`PATTERN`的字串

* 函数说明：`filter-out函数`也可以用来去除一个变量中的某些字符串，（实现和`filter函数`相反）

* 示例：

  ```makefile
  sources := foo.c bar.c baz.s ugh.h 
  
  all :
  	$(info $(filter-out %.c %.s,$(sources)))
  # 返回值为：ugh.h
  ```

### 2.7 排序函数—sort

* 函数原型：**$(sort LIST)**

* 函数功能：给字符串`LIST`中的单词以首字母为准进行排序（升序），并取掉重复的单词

* 返回值：空格分割的没有重复单词的字符串

* 函数说明：两个功能，排序和去字串中的重复单词。可以单独使用其中一个功能

* 示例：

  ```makefile
  $(sort foo bar lose foo)
  # 返回值为：bar foo lose
  ```
  
### 2.8 取单词函数—word

* 函数原型：**$(word N,TEXT)**

* 函数功能：取字串`TEXT`中第`N`个单词（`N`的值从 1 开始）

* 返回值：返回字串`TEXT`中第`N`个单词

* 函数说明：如果`N`值大于字串`TEXT`中单词的数目，返回空字符串。如果`N`为 0，出错

* 示例：

  ```makefile
  $(word 2, foo bar baz)
  # 返回值为：bar
  ```

### 2.9 取字串函数—wordlist

* 函数原型： **$(wordlist S,E,TEXT)**

* 函数功能：从字符串`TEXT`中取出从`S`开始到`E`的单词串。**`S`和`E`表示单词在字符串中位置的数字**

* 返回值：字串`TEXT`中从第`S`到`E`（包括`E`）的单词字符串

* 函数说明：`S`和`E`都是从 1 开始的数字。

  * 当`S`比`TEXT`中长度大时，返回空
  * 如果`E`大于`TEXT`长度，返回从`S`开始，到`TEXT`结束的单词串
  * 如果`S`大于`E`，返回空

* 示例：

  ```makefile
  $(wordlist 2, 3, foo bar baz)
  # 返回值为：bar baz
  ```

### 2.10 统计单词数目函数—words

* 函数原型：**$(words TEXT)**

* 函数功能：字算字符串`TEXT`中单词的数目

* 返回值：`TEXT`字符串中的单词数

* 示例：

  ```makefile
  sources := foo.c bar.c baz.s ugh.h 
  
  all :
  	$(info $(words $(sources)))
  # 返回值为：4
  ```

### 2.11 取首单词函数—firstword

* 函数原型：**$(firstword NAMES…)**

* 函数功能：取字符串`NAMES…`中的第一个单词

* 返回值：字符串`NAMES…`的第一个单词

* 函数说明：`NAMES`被认为是使用空格分割的多个单词（名字）的序列。函数忽略`NAMES…`中除第一个单词以外的所有的单词

* 示例：

  ```makefile
  $(firstword foo bar)
  # 返回值为：foo
  ```

## 3. 文件名处理函数

### 3.1 取目录函数—dir

* 函数原型：**$(dir NAMES…)**

* 函数功能：从文件名序列`NAMES…`中取出各个文件名的目录部分。将路径字符串中的文件名部分去除，只返回目录部分

* 这个函数会将路径字符串中的文件名部分去除，只返回目录部分

* 返回值：空格分割的文件名序列`NAMES…`中目录部分，就是最后一个斜线`/`之前的部分（包含斜线`/`）

* 函数说明：如果文件名中没有斜线，认为此文件为当前目录`./`下的文件

* 示例：

  ```makefile
  $(dir /path/to/file.txt)
  # 返回值为：/path/to/
  ```

### 3.2 取文件名函数—notdir

* 函数原型： **$(notdir NAMES…)**

* 函数功能：从文件名序列`NAMES…`中取出非目录部分。

* 返回值：文件名序列`NAMES…`中每一个文件的非目录部分；非目录部分是指最后一个斜线`/`（不包括斜线）之后的部分，删除所有文件名中的目录部分，只保留非目录部分

* 函数说明：如果`NAMES…`中存在不包含斜线的文件名，则不改变这个文件名。以反斜线结尾的文件名，是用空串代替，因此当`NAMES…`中存在多个这样的文件名时，返回结果中分割各个文件名的空格数目将不确定！这是**此函数的一个缺陷**

* 示例：

  ```makefile
  $(dir /path/to/file.txt)
  # 返回值为：/file.txt
  ```

### 3.3 取后缀函数—suffix

* 函数原型：**$(suffix NAMES…)**

* 函数功能：从文件名序列`NAMES…`中取出各个文件名的后缀。后缀是文件名中最后一个以点`.`开始的（包含点号）部分，如果文件名中不包含一个点号，则为空

* 返回值：以空格分割的文件名序列`NAMES…`中每一个文件的后缀序列

* 函数说明：`NAMES…`是多个文件名时，返回值是多个以空格分割的单词序列。如果文件名没有后缀部分，则返回空

* 示例：

  ```makefile
  $(suffix src/foo.c src-1.0/bar.c)
  # 返回值为：.c .c
  ```

### 3.4 取前缀函数—basename

* 函数原型：**$(basename NAMES…)**

* 从文件名序列**NAMES…**中取出各个文件名的前缀部分（点号之后的部分）。前缀部分指的是文件名中最后一个点号之前的部分

* 返回值：空格分割的文件名序列**NAMES…**中各个文件的前缀序列。如果文件没有前缀，则返回空字串

* 函数说明：如果“NAMES…”中包含没有后缀的文件名，此文件名不改变。如果一个文件名中存在多个点号，则返回值为此文件名的最后一个点号之前的文件名部分

* 示例：

  ```makefile
  $(basename src/foo.c src-1.0/bar.c /home/jack/.font.cache-1 hacks)
  # 返回值为：src/foo src-1.0/bar /home/jack/.font
  ```

### 3.5 加后缀函数—addsuffix

* 函数原型：**$(addsuffix SUFFIX,NAMES…)**

* 函数功能：为`NAMES…`中的每一个文件名添加后缀`SUFFIX`。参数`NAMES…`为空格分割的文件名序列，将`SUFFIX`追加到此序列的每一个文件名的末尾

* 返回值：以单空格分割的添加了后缀“SUFFIX”的文件名序列

* 示例：

  ```makefile
  $(addsuffix .c,foo bar)
  # 返回值为：foo.c bar.c
  ```

### 3.6 加前缀函数—addprefix

* 函数原型：**$(addprefix PREFIX,NAMES…)**

* 函数功能：为`NAMES…`中的每一个文件名添加前缀`PREFIX`。参数`NAMES…`是空格分割的文件名序列，将`SUFFIX`添加到此序列的每一个文件名之前

* 返回值：以单空格分割的添加了前缀“PREFIX”的文件名序列

* 示例：

  ```makefile
  $(addprefix src/,foo bar)
  # 返回值为：src/foo src/bar
  ```

### 3.7 单词连接函数—join

* 函数原型：**$(join LIST1,LIST2)**

* 函数功能：将字串`LIST1`和字串`LIST2`各单词进行对应连接。

  * 就是将`LIST2`中的第一个单词追加`LIST1`第一个单词字后合并为一个单词；
  * 将`LIST2`中的第二个单词追加到`LIST1`的第二个单词之后并合并为一个单词，……依次列推

* 函数说明：如果`LIST1`和`LIST2`中的字数目不一致时，两者中多余部分将被作为返回序列的一部分

* 示例 ：

  ```makefile
  $(join a b, .c .o)
  # LIST1与LIST2数量相等
  # 返回值为：a.c b.o
  
  $(join a b c, .c .o)
  # LIST1与LIST2数量不相等，多余部分连接在最后一并返回
  # 返回值为：a.c b.o c
  ```

### 3.8 获取匹配模式文件名函数—wildcard

* 函数原型：**$(wildcard PATTERN)**
* 函数功能：列出当前目录下所有符合模式`PATTERN`格式的文件名
* 返回值：空格分割的、存在当前目录下的所有符合模式`PATTERN`的文件名

* 函数说明：`PATTERN`使用`shell`可识别的通配符，包括`?`（单字符）、`*`（多字符）等

* 示例：

  ```makefile
  $(wildcard *.c)
  # 返回值foo.c bar.c；返回当前目录下所有.c文件列表
  ```

### 3.9 相对路径转化绝对路径函数—abspath

* 函数原型：**$(abspath FILE_PATH)**

* 函数功能：`FILE_PATH`是一个相对路径的变量或字符，`abspath函数`作用是将`FILE_PATH`转化为绝对路径

* 返回值：转化后的绝对路径

* 示例：

  ```makefile
  name = ../openwrt/COPYING
  
  all:
  	$(info $(abspath $(name)))      #COPYING文件的绝对路径
  ```

## 4. 其他函数

### 4.1 foreach 函数

* 函数原型：**$(foreach VAR,LIST,TEXT)**

* 函数功能：函数“foreach”不同于其它函数。它是一个循环函数。类似于 Linux 的 shell 中的for 语句

* 函数执行流程：
  * 展开变量`VAR`和`LIST`的引用，而表达式`TEXT`中的变量引用不展开；
  * 将`LIST`中使用空格分割的单词依次取出赋值给变量`VAR`，然后执行`TEXT`表达式，重复直到`LIST`的最后一个单词（为空时结束）
  * **`TEXT`中的变量或者函数引用在执行时才被展开，因此如果在`TEXT`中存在对`VAR`的引用，那么`VAR`的值在每一次展开式将会到的不同的值**

* 返回值：空格分割的多次表达式`TEXT`的计算的结果

* 示例：

  ```makefile
  dirs := a b c d 
  files := $(foreach dir, $(dirs), $(dir)/)
  # 返回值：a/  b/  c/  d/
  ```

  * 示例中的`TEXT`的表达式为 `$(dir)/`
  
  * 表达式第一次执行时将展开为`a/`
  
  * 表达式第一次执行时将展开为`b/`
  
  * 表达式第一次执行时将展开为`c/`以此类推
  
    ```makefile
    # 所以此函数所实现的功能就和一下语句等价
    files := a/ b/ c/ d/
    ```

### 4.2 if函数

* 函数原型：**$(if CONDITION,THEN-PART[,ELSE-PART])**

* 函数功能：如果`CONDITION`的展开结果非空，则条件为真，就将第二个参数`THEN_PATR`作为函数的计算

  表达式；`CONDITION`的展开结果为空，将第三个参数`ELSE-PART`作为函数的表达式，函数的返回结果为有效表达式的计算结果

* 函数说明：第一个参数`CONDITION`，在函数执行时忽略其前导和结尾空字符，如果包含对其他变量或者函数的引用则进行展开

* 返回值：根据条件决定函数的返回值是第一个或者第二个参数表达式的计算结果。当不存在第三个参数`ELSE-PART`，并且`CONDITION`展开为空，函数返回空

* 示例：

  ```makefile
  # 当SRC_DIR值不为空时返回 /home/src否则返回$(SRC_DIR)
  SRC_DIR = a
  SUBDIR = $(if $(SRC_DIR), $(SRC_DIR), /home/src)
  ```

### 4.3 call函数

`call函数`是唯一一个可以创建定制化参数函数的引用函数。使用这个函数可以实现对用户自己定义函数引用。我们可以将一个变量定义为一个复杂的表达式，用`call函数`根据不同的参数对它进行展开来获得不同的结果

* 函数原型：**$(call VARIABLE,PARAM,PARAM,...)**

* 函数功能：执行时，将每个参数`PARAM`依次赋值给临时变量`$(1)`、`$(2)`**（这些临时变量定义在“VARIABLE”的值中）**，执行时变量`VARIABLE`被展开，变量定义中的`$(1)`作为第一个参数，并将函数参数值中的第一个参数赋值给它；变量中的`$(2)`一样被赋值为函数的第二个参数值；依此类推（变量`$(0)`代表变量`VARIABLE`本身）之后对变量`VARIABLE` 表达式的计算值

* 函数说明：

  1. 函数中`VARIBLE`是一个变量名，而不是变量引用。因此，通常`call`函数中的`VARIABLE`中包含`$`（当然，除非此变量名是一个计算的变量名）

  2. 当变量`VARIBLE`是一个 `make` 内嵌的函数名时（如`if`、`foreach`、`strip`等），对`PARAM`参数的使用需要注意，因为不合适或者不正确的参数将会导致函数的返回值难以预料。

  3. 函数中多个`PARAM`之间使用逗号分割

  4. **如果使用`define`定义变量并使用`call`调用，`define`中的变量不能直接使用赋值语句，需要使用eval函数执行**

     ```makefile
     define Print_test
     	info = $(1)                #错误用法
     	$(eval info = $(1))        #正确用法
     endef
     
     all:
     	$(call Print_test, 111)
     ```

  5. 当`call`函数执行没有变量接收返回值时，`VARIBLE`的内容会在函数调用时执行。如果有变量接收`call`函数的返回值则`VARIBLE`在变量调用时执行（例如使用`eval`函数时）

* 返回值：参数值`PARAM`依次替换`$(1)`、`$(2)`…… 之后变量`VARIABLE`定义的表达式的计算值

* 示例：

  ```makefile
  reverse = $(2) $(1) 
  # foo中接收到的返回值为已经替换过的reverse值
  foo = $(call reverse, a, b)
  
  all :
  	$(info $(foo))        #b a
  ```

### 4.4 value函数

* 函数原型：**$(value VARIABLE)**

* 函数功能：不对变量`VARIBLE`进行任何展开操作，直接返回变量`VARIBALE`的值。这里`VARIABLE`是一个变量名，一般不包含`$`（除非计算的变量名）

* 返回值：变量`VARIBALE`所定义文本值（如果变量定义为递归展开式，其中包含对其他变量或者函数的引用，那么函数不对这些引用进行展开，函数的返回值是包含有引用值）

* 函数说明：`value`函数通常与`eval`函数一起使用

* 示例：

  ```makefile
  FOO = $PATH 
  
  all: 
  @echo $(FOO)                   #ATH
  @echo $(value FOO)             #当前系统环境变量“PATH”的值
  ```

### 4.5 eval函数

函数`eval`是一个比较特殊的函数。使用它可以在`Makefile`中构造一个可变的规则结构关系（依赖关系链），其中可以使用其它变量和函数；函数`eval`对它的参数进行展开，展开的结果作为`Makefile`的一部分，`make`可以对展开内容进行语法解析。展开的结果可以包含一个新变量、目标、隐含规则或者是明确规则等

* 函数功能：根据其参数的关系、结构，对它们进行替换展开
* 函数说明：`eval`函数执行时会对它的参数进行两次展开。
  * 第一次展开过程发是由函数本身完成的，第二次是函数展开后的结果被作为`Makefile`内容时由`make`解析时展开的。
  * 实际使用时，如果在函数的展开结果中存在引用（格式为：`$(x)`），那么在函数的参数中应该用`$$`来代替`$`）所以所以通常它的参数中会使用`value`函数来取一个变量的文本值
  * **`eval函数`通常与`call函数`一起使用**，用于执行call函数的返回值
* 返回值：函数`eval`的返回值为空
* 示例：

```makefile
define foo 
var := 123
arg := $1
endef 

# 当使用eval函数时，foo的内容将执行，否则不执行
$(eval $(call foo, pointer))

all:
	@echo var: $(var), arg: $(arg)
# 当eval函数被注释时结果为：var: , arg:
# 当eval函数被执行时结果为：var: 123, arg: pointer
```

### 4.6 origin函数

函数`origin`和其他函数不同，函数`origin`的动作不是操作变量（它的参数）。它只是获取此变量（参数）相关的信息，告诉我们这个变量的出处（定义方式）

* 函数原型：**$(origin VARIABLE)**

* 函数功能：查询参数`VARIABLE`（一个变量名）的出处
* 函数说明：`VARIABLE`是一个变量名而不是一个变量的引用。因此通常它不包含`$`（计算的变量名例外）
* 返回值：
  * **undefined**
    * 变量`VARIABLE`没有被定义
  * **default**
    * 变量`VARIABLE`是一个默认定义（内嵌变量）。如`CC`、`MAKE`、`RM`等变量
  * **environment**
    * 变量`VARIABLE`是一个系统环境变量，并且`make`没有使用命令行选项`-e`（启用环境变量的覆盖功能）
  * **environment override**
    * 变量`VARIABLE`是一个系统环境变量，并且`make`使用了命令行选项`-e`（启用环境变量的覆盖功能）
  * **file**
    * 变量`VARIABLE`在某一个 `makefile` 文件中定义
  * **command line**
    * 变量`VARIABLE`在命令行中定义
  * **override**
    * 变量`VARIABLE`在 `makefile` 文件中定义并使用`override`指示符声明
  * **automatic**
    * 变量`VARIABLE`是自动化变量

### 4.7 shell函数

* 函数原型：**$(shell parameter)**

* 函数功能：执行对应的`shell`命令，并返回执行结果

* 返回值：函数`shell`的参数（一个 shell 命令）在 `shell` 环境中的执行结果

* 函数说明：`make`仅仅对它的回返结果进行处理；`make`将函数返回结果中的所有换行符`\n`或者一对`\n\r`替换为单空格；并去掉末尾的回车符号`\n`或者`\n\r`。进行函数展开式时，它所调用的命令（它的参数）得到执行。

* ```makefile
  $(shell pwd)           #当前路径
  ```

## 5. 流程控制语句

* **需要特别注意的时在一个目标内使用时，前面不可以有`[tab]符号`，否则会被认为是`shell语句`；相关语句会传给shell解释器使用shell语法解析，导致报错**

### 5.1 ifeq函数

* 判断参数是否不相等；相等为 true，不相等为 false

  ```makefile
  ifeq ($(a), $(b))
  	@echo "a = b"
  else
  	@echo "a != b"
  endif
  ```

### 5.2 ifneq函数

* 判断参数是否不相等；不相等为 true，相等为 false

  ```makefile
  ifneq ($(a), $(b))
  	@echo "a = b"
  else
  	@echo "a != b"
  endif
  ```

### 5.3 ifdef函数

* 判断变量的值是不是为空；有值为 true，没有值为 false

  ```makefile
  ifdef v
  	@echo "a = b"
  else
  	@echo "a != b"
  endif
  ```

### 5.4 ifndef函数

* 判断变量的值是不是为空；没有值为 true，有值为 false

  ```makefile
  ifndef v
  	@echo "a = b"
  else
  	@echo "a != b"
  endif
  ```

## 6. 特殊变量

* makefile中内置了一些特殊的变量，巧妙的使用他们可以极大的提升我们的开发效率

### 6.1 环境变量

#### 6.1.1 MAKEFILES变量

* `MAKEFILES` 是一个特殊的 Makefile 变量，在 Makefile 中使用它可以指定其他要包含的 Makefile 文件

#### 6.1.2 MAKEFILES_LIST变量

* 在 Makefile 中，`MAKEFILES_LIST` 是一个特殊的变量，它包含了当前构建过程中所有被包含的 Makefile 文件的列表。`MAKEFILES_LIST` 变量是一个只读变量，它按照 `Makefile` 文件被包含的顺序存储了这些文件的路径

#### 6.1.3 VPATH变量

* `VPATH` 是一个特殊的 Makefile 变量，用于指定在构建过程中搜索源文件的路径。通常情况下，Make 工具会在当前目录下搜索依赖项中指定的源文件。但是，有时源文件可能位于其他目录中，这时就可以使用 `VPATH` 变量来告诉 Make 工具在指定目录中搜索源文件

* 示例：

  ```makefile
  VPATH = ./src
  
  test : main.c
  	gcc -o $@ $<
  
  # 文件目录结构如下
  ├── src
  │   └── main.c
  ```

#### 6.1.4 SHELL变量

* `SHELL` 是一个环境变量，用于指定在运行 Makefile 规则中的命令时使用的 shell 程序。在 Makefile 中，默认情况下，`SHELL` 变量的值为 `/bin/sh`，表示使用标准的 Bourne shell 或兼容的 shell 解析执行命令

#### 6.1.5 MAKE_SHELL变量

* `MAKE_SHELL`用于指定在运行 Makefile 规则中的命令时使用的 shell 程序。可以通过设置 `MAKE_SHELL` 变量来指定要使用的 shell 解释器。例如，使用 `Bash shell` 来执行命令，可以将 `MAKE_SHELL` 设置为 `/bin/bash`

#### 6.1.6 MAKE变量

* `MAKE` 是一个预定义的环境变量，在 Makefile 中表示正在使用的 Make 工具的名称

* `MAKE`后可以携带后缀已达到调用其他效果，最常见的一种方式就是`$(MAKE) -C` 的意思是调用make命令，并切换到指定路径下执行Makefile

* 示例：

  ```makefile
  # 使用make执行path/to/other/directory文件
  all:
      $(MAKE) -C path/to/other/directory
  ```

#### 6.1.7 MAKELEVEL变量

* `MAKELEVEL` 是一个预定义的 Makefile 变量，用于表示当前 Make 的递归层级。在 Make 的递归构建过程中，当进行嵌套调用时，每次调用都会递增 `MAKELEVEL` 的值。这个值最初为 0，表示最外层的 Makefile。每当进行一次嵌套调用时，`MAKELEVEL` 的值就会增加

#### 6.1.8 MAKEFLAGS变量

* `MAKEFLAGS` 是一个预定义的 Makefile 变量，用于指定 Make 工具的参数和标志。如果执行总控 `Makefile` 时，`make` 命令带有参数或者在上层的 `Makefile` 中定义了这个变量，那么 `MAKEFLAGS` 变量的值将会是 `make` 命令传递的参数，并且会传递到下层的 `Makefile` 中，这是一个系统级别的环境变量

#### 6.1.9 MAKECMDGOALS变量

* `MAKECMDGOALS` 是一个预定义的 Makefile 变量，用于表示在命令行中指定的目标列表。

  * 当你在命令行中运行 `make` 命令，并指定要构建的目标列表时，这些目标会被存储在 `MAKECMDGOALS` 变量中。`MAKECMDGOALS` 的值是一个以空格分隔的目标列表

    ```makefile
    all:
        $(info $(MAKECMDGOALS))
    
    # 命令行中运行 make target1 target2 时，打印target1 target2
    ```

#### 6.1.10 CURDIR变量

* `CURDIR` 是一个预定义的 Makefile 变量，用于表示当前工作目录的路径。在 Makefile 中，`CURDIR` 变量会被设置为执行 Make 命令时所在的当前工作目录的绝对路径

#### 6.1.11 TOPDIR变量

* `TOPDIR`表示项目的顶级目录路径。通常用于构建相关的路径变量，例如源文件路径、目标文件路径等
  * `CURDIR`表示当前*Makefile*的工作目录路径。当前Makefile 指的是正在执行的Makefile 文件的所在目录。这个变量的值通常由Makefile 解析时自动设置。注意，在包含其他Makefile 时，`CURDIR`可能会改变

### 6.2 自动化变量

#### 6.2.1 $@变量

* `$@` 是一个在 Makefile 中使用的自动化变量，它表示当前规则的目标（target）

  ```makefile
  test : main.c
  	gcc -o $@ $<        # $@是test
  ```

#### 6.2.2 $<变量

* 在 Makefile 中，`$<` 是一个自动化变量，它表示当前规则的第一个依赖文件

  ```makefile
  test : main.c
  	gcc -o $@ $<           # $<是main.c
  ```

#### 6.2.3 $?变量

* 在 Makefile 中，`$?` 是一个自动化变量，它表示比目标文件更新的所有依赖文件的列表，以空格分隔。

  * 当一个目标文件的依赖文件发生变化时，Make 在运行时会比较目标文件和依赖文件的时间戳来判断是否需要重新生成目标文件。如果目标文件的时间戳早于其中任何一个依赖文件的时间戳，则说明依赖文件发生了更新，此时 `$?` 会被替换为所有发生更新的依赖文件的列表

  * 通过使用 `$?`，我们可以在规则的命令中引用发生更新的依赖文件，从而只重新生成需要更新的部分，提高构建的效率

    ```makefile
    all: program
    
    program: main.o utils.o
        gcc -o $@ $?
    
    main.o: main.c
        gcc -c $< -o $@
    
    utils.o: utils.c
        gcc -c $< -o $@
    ```

#### 6.2.4 $^变量

* 在 Makefile 中，`$^` 是一个自动化变量，它表示当前规则的所有依赖文件的列表，以空格分隔

#### 6.2.5 $+变量

* 在 Makefile 中，`$+` 是一个非标准的自动化变量。它与 `$^` 类似，表示当前规则的所有依赖文件的列表，以空格分隔。
  * 与 `$^` 不同的是，`$+` 保留了依赖文件在 Makefile 中出现的顺序。这意味着它会按照你在规则中写的顺序列出依赖文件，而不考虑依赖文件中是否存在重复的文件

#### 6.2.6 $(@D)变量

* 在 Makefile 中，`$(@D)` 是一个自动化变量，表示当前目标文件的目录部分

#### 6.2.7 $(@F)变量

* 在 Makefile 中，`$(@F)` 是一个自动化变量，表示当前目标文件的文件名部分（不包含目录）

#### 6.2.8 $(<D)变量

* 在 Makefile 中，`$(<D)` 是一个自动化变量，表示第一个依赖文件（输入文件）的目录部分

#### 6.2.9 $(<F)变量

* 在 Makefile 中，`$(<F)` 是一个自动化变量，表示第一个依赖文件（输入文件）的文件名部分

#### 6.2.10 $(?D)变量

* 在 Makefile 中，`$(?D)` 是一个自动化变量，用于表示所有已修改的依赖文件的目录部分

#### 6.2.11 $(?F)变量

* 在 Makefile 中， `$(?F)`是一个自动化变量。这个变量用来表示所有已修改的依赖文件的文件名部分
