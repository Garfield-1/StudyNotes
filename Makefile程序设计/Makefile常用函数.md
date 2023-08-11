# Makefile常用函数

* GNU make 的函数提供了处理文件名、变量、文本和命令的方法。使用函数我们的 Makefile 可以书写的更加灵活和健壮



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
  $(patsubst %.c,%.o,x.c.c bar.c)  		#x.c.o bar.o
  
  # 把字串“x.c.c bar.c”中以.c 结尾的单词替换成以.o 结尾的字符。函数的返回结果是“x.c.o bar.o”
  ```

  

### 2.3 去空格函数—strip

* 函数原型：**$(strip STRINT)** 

* 函数功能：去掉字符串`STRINT`的开头和结尾的空字符，并将其中多个连续空字符合并为一个空字符

* 返回值：无前导和结尾空字符、使用单一空格分割的多单词字符串

* 函数说明：空字符包括空格、`[Tab]`等不可显示字符

* 示例：

  ```makefile
  STR =        a			 b  c 
  LOSTR = $(strip $(STR))
  all:
  	$(info $(STR))					#        a			 b  c
  	$(info $(LOSTR))				#a b c
  
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

  

### 3.7 单词连接函数——join

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

  

