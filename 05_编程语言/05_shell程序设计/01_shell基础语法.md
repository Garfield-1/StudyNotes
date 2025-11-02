# Shell基础语法

## 1. 简介

### 1.1 Shell 环境

`Shell` 编程跟 `JavaScript`、`php` 编程一样，只要有一个能编写代码的文本编辑器和一个能解释执行的脚本解释器就可以了。`#!` 告诉系统其后路径所指定的程序即是解释此脚本文件的 `Shell` 程序。

`Linux` 的 `Shell` 种类众多，常见的有：

* `Bourne Shell`（`/usr/bin/sh`或`/bin/sh`）
* `Bourne Again Shell`（`/bin/Bash`）
* `C Shell`（`/usr/bin/csh`）
* `K Shell`（`/usr/bin/ksh`）
* `Shell for Root`（`/sbin/sh`）

## 2. 变量

### 2.1 变量定义赋值

定义变量时，变量名不加美元符号（$），注意，**变量名和等号之间不能有空格**，这可能和你熟悉的所有编程语言都不一样。有空格会出错

示例：

```shell
variableName="value"
```

### 2.2 使用变量

使用一个定义过的变量，只要在变量名前面加美元符号`$`即可

示例：

```shell
your_name="lalal"
echo $your_name
echo ${your_name}
```

### 2.3 只读变量

使用 `readonly` 命令可以将变量定义为只读变量，只读变量的值不能被改变

示例：

```shell
#!/bin/Bash

# 下面的例子尝试更改只读变量，结果报错
test="a"
readonly test
test="b"
```

### 2.4 删除变量

使用 `unset` 命令可以删除变量，变量被删除后不能再次使用，同时`unset` 命令不能删除只读变量。

示例：

```shell
unset variable_name
```

### 2.5 特殊变量

| 变量 | 含义                                                         |
| ---- | ------------------------------------------------------------ |
| `$0` | 当前脚本的文件名                                             |
| `$n` | 传递给脚本或函数的参数。n 是一个数字，表示第几个参数。例如，第一个参数是`$1`，第二个参数是`$2`。 |
| `$#` | 传递给脚本或函数的参数个数。                                 |
| `$*` | 传递给脚本或函数的所有参数。                                 |
| `$@` | 传递给脚本或函数的所有参数。被双引号(" ")包含时，与 `$*` 稍有不同 |
| `$?` | 上个命令的退出状态，或函数的返回值。                         |
| `$$` | 当前Shell进程ID。对于 Shell 脚本，就是这些脚本所在的进程ID。 |

`$*` 和 `$@` 的区别

* `$*` 和 `$@` 都表示传递给函数或脚本的所有参数，不被双引号(" ")包含时，都以`"$1" "$2" … "$n"` 的形式输出所有参数。但是当它们被双引号(" ")包含时，"`$*`" 会将所有的参数作为一个整体，以"`$1 $2 … $n`"的形式输出所有参数；"`$@`" 会将各个参数分开，以`"$1" "$2" … "$n"` 的形式输出所有参数。

* 示例：

    ```shell
    #!/bin/Bash
    
    echo "print each param from \"\$*\""
    for var in "$*"
    do
        echo "$var"
    done
    
    echo "print each param from \"\$@\""
    for var in "$@"
    do
        echo "$var"
    done
    
    #-------
    # 执行命令sh ./test.sh a b c d
    # 输出结果
    # print each param from "$*"
    # a b c d
    # print each param from "$@"
    # a
    # b
    # c
    # d
    #-------
    ```

退出状态
* `$?` 可以获取上一个命令的退出状态。所谓退出状态，就是上一个命令执行后的返回结果
* 退出状态是一个数字，一般情况下，大部分命令执行成功会返回 0，失败返回 1；不过，也有一些命令返回其他值，表示不同类型的错误。

* 示例：

  ```shell
  if [[ $? != 0 ]];then
      echo "error"
      exit 1;
  fi
  ```

### 2.6 变量检测语句

| 形式              | 说明                                                         |
| ----------------- | ------------------------------------------------------------ |
| `${var}`          | 变量本来的值                                                 |
| `${var:-word}`    | 如果变量 `var` 为空或已被删除(unset)，那么返回 word，但不改变 `var` 的值。 |
| `${var:=word}`    | 如果变量 `var` 为空或已被删除(unset)，那么返回 word，并将 `var` 的值设置为 word。 |
| `${var:?message}` | 如果变量 `var` 为空或已被删除(unset)，那么将消息 message 送到标准错误输出，可以用来检测变量 `var` 是否可以被正常赋值。若此替换出现在Shell脚本中，那么脚本将停止运行。 |
| `${var:+word}`    | 如果变量 `var` 被定义，那么返回 word，但不改变 var 的值。    |

`${var}`语句

* 变量本来的值

`${var:-word}`语句

* 如果变量 `var` 为空或已被删除，那么返回 `word`，但不改变 `var` 的值;否则返回`var`的值

* 示例：

    ```shell
    #!/bin/Bash
    
    var="var"
    word="word"
    
    echo ${var:-word}            #var
    unset var
    echo ${var:-word}            #word
    ```

`${var:=word}`语句

* 如果变量 `var` 为空或已被删除，那么返回 `word`，并将 `var` 的值设置为 `word`；否则返回`var`的值

* 示例：

  ```shell
  #!/bin/Bash
  
  var="var"
  word="word"
  
  echo ${var:=word}              #word
  echo $var                      #var
  unset var
  echo ${var:=word}              #var
  echo $var                      #word
  ```

`${var:?message}`语句

* 如果变量 `var` 为空或已被删除，那么将`message`发送至标准错误输出 ，否则返回`var`；
* 可以用来检测变量 `var` 是否可以被正常赋值。**若此替换出现在Shell脚本中，那么脚本将停止运行**
* 示例：

  ```shell
  #!/bin/Bash
  
  name="Alice"
  echo "Hello, ${name:?Variable 'name' is not defined or empty!}"  #Hello, Alice
  
  unset name  # 删除变量name
  echo "Hello, ${name:?Variable 'name' is not defined or empty!}"  #Variable 'name' is not defined or empty!
  
  echo "Hello World"              #不执行
  ```

`${var:+word}`语句

* 如果变量 `var` 被定义，那么返回 `word`，但不改变 `var` 的值；否则返回空
* 示例：

  ```shell
  #!/bin/Bash
  
  var="var"
  word="word"
  
  echo ${var:+word}                #word
  
  unset var
  echo ${var:+word}                #空值
  ```

## 3. 字符串

`Shell`的字符串由单引号或双引号包裹；两者不完全相同

### 3.1 单双引号的区别

* 双引号里可以有变量或转义字符，**单引号里的任何字符都会原样输出**，单引号字符串中的变量是无效的；
* 单引号字串中不能出现单独一个的单引号（对单引号使用转义符后也不行），但可成对出现，作为字符串拼接使用

### 3.2 字符串截取

1. **`#`号截取，删除左边字符，保留右边字符**
    
    其中 `var` 是变量名，`#` 号是运算符，`*//` 表示从左边开始删除第一个 `//` 号及左边的所有字符
    
    ```shell
    var=http://www.aaa.com/123.htm
    # 即删除 'http://'
    echo ${var#*//}                         #www.aaa.com/123.htm
    ```
    
2. **`##`号截取，删除左边字符，保留右边字符**
    
    `\##*/` 表示从左边开始删除最后（最右边）一个 `/` 号及左边的所有字符
    
    ```shell
    var=http://www.aaa.com/123.htm
    # 删除 'http://www.aaa.com/'
    echo ${var##*/}                          #123.htm
    ```
    
3. **`%`号截取，删除右边字符，保留左边字符**
    
    `%/*` 表示从右边开始，删除第一个 `/` 号及右边的字符
    
    ```shell
    var=http://www.aaa.com/123.htm
    # 删除 '/123.htm'
    echo ${var%/*}                            #http://www.aaa.com
    ```
    
4. **`%%` 号截取，删除右边字符，保留左边字符**
    
    `%%/*` 表示从右边开始，删除最后（最左边）一个 `/` 号及右边的字符
    
    ```shell
    var=http://www.aaa.com/123.htm
    # 删除'http://www.aaa.com'
    echo ${var%%/*}                          #http:
    ```
    
5. **从左边第几个字符开始，及字符的个数**
    
    其中的 `0` 表示左边第一个字符开始，`5` 表示左边第五个字符结束
    
    ```shell
    var=http://www.aaa.com/123.htm
    #截取第1到第5个字符
    echo ${var:0:5}                         #http:
    ```
    
6. **从左边第几个字符开始，一直到结束**
    
    其中的 **`7` 表示左边第`8`个字符开始**，一直到结束
    
    ```shell
    var=http://www.aaa.com/123.htm
    #截取第8到结束的字符串
    echo ${var:7}                           #www.aaa.com/123.htm
    ```
    
7. **从右边第几个字符开始，及字符的个数**
    
    其中的 `0-7` 表示右边算起第七个字符开始，`3` 表示字符的个数
    
    ```shell
    var=http://www.aaa.com/123.htm
    #截取从后向前的七个字符中的前三个
    echo ${var:0-7:3}                           #123
    ```
    
8. **从右边第几个字符开始，一直到结束**
    
    表示从右边第七个字符开始，一直到结束
    
    ```shell
    var=http://www.aaa.com/123.htm
    #截取从后向前的七个字符
    echo ${var:0-7}                           #123.html
    ```

**注：**（左边的第一个字符是用 `0` 表示，右边的第一个字符用 `0-1` 表示）

## 4. 数组

`Bash`支持一维数组（不支持多维数组），并且没有限定数组的大小。类似与`C`语言，数组元素的下标由`0`开始编号。获取数组中的元素要利用下标，下标可以是整数或算术表达式，其值应大于或等于`0`。

### 4.1 数组赋值

在`Shell`中，用括号来表示数组，数组元素用`空格`符号分割开。

```shell
# 风格1
array_name=(value0 value1 value2 value3)

# 风格2
array_name=(
    value0
    value1
    value2
    value3
)
```

还可以单独定义数组的各个分量：

以不使用连续的下标，而且下标的范围没有限制

```shell
array_name[0]=value0
array_name[1]=value1
array_name[2]=value2
```

### 4.2 数组打印

`Shell`中有几种方式可以打印数组

```shell
echo ${array_name[2]} #读取下标为2的元素
echo ${array_name[*]} #读取所有元素
echo ${array_name[@]} #读取所有元素

echo ${#array_name[*]} #获取数组长度
echo ${#array_name[@]} #获取数组长度
echo ${#array_name[1]} #获取数组中单个元素的长度

# 以上述定义的数组为例，输出结果
# value2
# value0 value1 value2 value3
# value0 value1 value2 value3
# 4
# 4
# 6
```

## 5. 运算符

`Bash` 支持很多运算符，包括算数运算符、关系运算符、布尔运算符、字符串运算符和文件测试运算符

### 5.1 算术运算符

原生`Bash`不支持简单的数学运算，但是可以通过其他命令来实现，例如 `awk` 和 `expr`，`expr` 最常用

* `expr` 是一款表达式计算工具，使用它能完成表达式的求值操作。

* 例如，两个数相加使用反引号(**注意使用的是反引号` 而不是单引号'**)：或者**$()**符号

    ```shell
    #!/bin/Bash
    
    val=`expr 2 + 2`
    echo "两数之和为 : $val"
    ```

  * 表达式和运算符之间要有空格，例如 `2+2` 是不对的，必须写成 `2 + 2`，这与我们熟悉的大多数编程语言不一样。
  * 完整的表达式要被 \`包含

* 可以使用其他的的运算符进行计算

    ```shell
    #!/bin/Bash
    
    a=10
    b=20
    
    # 乘号(*)前边必须加反斜杠(\)才能实现乘法运算
    val=`expr $a \* $b`
    echo "a * b : $val"
    ```

### 5.2 关系运算符

* 关系运算符只支持数字，不支持字符串，除非字符串的值是数字
* 关系运算语句需要在`[]`符号中

常见关系运算符

| 运算符 | 说明                                                | 举例                         |
| :----- | :-------------------------------------------------- | :--------------------------- |
| -eq    | 检测两个数是否相等，相等返回 true                   | [ `$a` -eq `$b` ] 返回 false |
| -ne    | 检测两个数是否不相等，不相等返回 true               | [ `$a` -ne `$b` ] 返回 true  |
| -gt    | 检测左边的数是否大于右边的，如果是，则返回 true     | [ `$a` -gt `$b` ] 返回 false |
| -lt    | 检测左边的数是否小于右边的，如果是，则返回 true     | [ `$a` -lt `$b` ] 返回 true  |
| -ge    | 检测左边的数是否大于等于右边的，如果是，则返回 true | [ `$a` -ge `$b` ] 返回 false |
| -le    | 检测左边的数是否小于等于右边的，如果是，则返回 true | [ `$a` -le `$b` ] 返回 true  |

示例：

```shell
if [ $a -eq $b ]
then
    echo "$a -eq $b : a 等于 b"
else
    echo "$a -eq $b: a 不等于 b"
fi
```

### 5.3 布尔运算符

常见布尔运算符

| 运算符 | 说明                                              | 举例                                       |
| :----- | :------------------------------------------------ | :----------------------------------------- |
| !      | 非运算，表达式为 true 则返回 false，否则返回 true | [ ! false ] 返回 true                      |
| -o     | 或运算，有一个表达式为 true 则返回 true           | [ `$a` -lt 20 -o `$b` -gt 100 ] 返回 true  |
| -a     | 与运算，两个表达式都为 true 才返回 true           | [ `$a` -lt 20 -a `$b` -gt 100 ] 返回 false |

### 5.4 逻辑运算符

使用 **`[[ ... ]]`** 条件判断结构，而不是 **`[ ... ]`**，能够防止脚本中的许多逻辑错误。比如，**`&&`**、**`||`**、**`<`** 和 **`>`** 操作符能够正常存在于 **`[[ ]]`** 条件判断结构中，但是如果出现在 **`[ ]`** 结构中的话，会报错

常见逻辑运算符

| 运算符 | 说明       | 举例                                           |
| :----- | :--------- | :--------------------------------------------- |
| &&     | 逻辑的 AND | [[ `$a` -lt 100 && `$b` -gt 100 ]] 返回 false  |
| \|\|   | 逻辑的 OR  | [[ `$a` -lt 100 \|\| `$b` -gt 100 ]] 返回 true |

### 5.5 字符串运算符

常见字符串运算符

| 运算符 | 说明                                         | 举例                         |
| :----- | :------------------------------------------- | :--------------------------- |
| =      | 检测两个字符串是否相等，相等返回 true。      | [ `$a` = `$b` ] 返回 true。  |
| !=     | 检测两个字符串是否不相等，不相等返回 true。  | [ `$a` != `$b` ] 返回 true。 |
| -z     | 检测字符串长度是否为0，为0返回 true。        | [ -z `$a` ] 返回 true。      |
| -n     | 检测字符串长度是否不为 0，不为 0 返回 true。 | [ -n "`$a`" ] 返回 true。    |
| $      | 检测字符串是否不为空，不为空返回 true。      | [ `$a` ] 返回 true。         |

实例：

```shell
#!/bin/Bash

# 正确的做法是 $a 这里应该加上双引号，否则 -n $a 的结果永远是 true：
a=""
if [ -n "$a" ]
then
   echo "-n $a : 字符串长度不为 0"
else
   echo "-n $a : 字符串长度为 0"
fi
```

### 5.6 文件测试运算符

文件测试运算符用于检测 `Unix` 文件的各种属性

常见文件测试运算符

| 操作符  | 说明                                                         | 举例                      |
| :------ | :----------------------------------------------------------- | :------------------------ |
| -b file | 检测文件是否是块设备文件，如果是，则返回 true。              | [ -b $file ] 返回 false。 |
| -c file | 检测文件是否是字符设备文件，如果是，则返回 true。            | [ -c $file ] 返回 false。 |
| -d file | 检测文件是否是目录，如果是，则返回 true。                    | [ -d $file ] 返回 false。 |
| -f file | 检测文件是否是普通文件（既不是目录，也不是设备文件），如果是，则返回 true。 | [ -f $file ] 返回 true。  |
| -g file | 检测文件是否设置了 SGID 位，如果是，则返回 true。            | [ -g $file ] 返回 false。 |
| -k file | 检测文件是否设置了粘着位(Sticky Bit)，如果是，则返回 true。  | [ -k $file ] 返回 false。 |
| -p file | 检测文件是否是有名管道，如果是，则返回 true。                | [ -p $file ] 返回 false。 |
| -u file | 检测文件是否设置了 SUID 位，如果是，则返回 true。            | [ -u $file ] 返回 false。 |
| -r file | 检测文件是否可读，如果是，则返回 true。                      | [ -r $file ] 返回 true。  |
| -w file | 检测文件是否可写，如果是，则返回 true。                      | [ -w $file ] 返回 true。  |
| -x file | 检测文件是否可执行，如果是，则返回 true。                    | [ -x $file ] 返回 true。  |
| -s file | 检测文件是否为空（文件大小是否大于0），不为空返回 true。     | [ -s $file ] 返回 true。  |
| -e file | 检测文件（包括目录）是否存在，如果是，则返回 true。          | [ -e $file ] 返回 true。  |

实例:

```shell
#!/bin/Bash

file="/var/test.sh"
if [ -r $file ]
then
   echo "文件可读"
else
   echo "文件不可读"
fi
```

## 6. 特殊符号

### 6.1 `[]`符号

在Shell中，`[]` 是一种用于条件测试和比较的特殊符号。它通常用于控制流程语句（如 if-then、while 和 for 循环）中，`[]` 中的条件和操作符之间需要有空格，`[]` 可以用于以下情境：

1. 条件测试：`[ condition ]` 用于检查条件是否为真。条件可以是变量、比较操作符和逻辑操作符的组合。如果条件为真，返回退出码 0；如果条件为假，返回非零退出码。

    **示例：**

    ```shell
    if [ $count -gt 10 ]; 
        then echo "count is greater than 10"; 
    fi
    ```

2. 字符串比较：`[ string1 = string2 ]` 用于比较两个字符串是否相等。其他字符串比较操作符还包括`!=`（不相等）、`-z`（空字符串）、`-n`（非空字符串）等。

    **示例：**

    ```shell
    if [ "$name" = "John" ]; 
        then echo "Hello, John!"; 
    fi
    ```

3. 数值比较：`[ num1 -eq num2 ]` 用于比较两个数值是否相等。其他数值比较操作符还包括`-ne`（不相等）、`-lt`（小于）、`-gt`（大于）、`-le`（小于等于）、`-ge`（大于等于）等。

    **示例：**

    ```shell
    if [ $age -ge 18 ]; 
        then echo "You are an adult."; 
    fi
    ```

### 6.2 `[[]]`符号

在`Shell`中，`[[]]` 是一种扩展的条件测试结构，通常用于复杂的条件表达式和模式匹配。相比于传统的`[]`，`[[]]` 提供了更多的功能和更灵活的语法

`[]`符号与`[[]]`符号之间存在一些区别

1. `[ ]`是符合`POSIX`标准的测试语句，兼容性更强，几乎可以运行在所有的`Shell`解释器中，`[[ ]]`仅可运行在特定的几个Shell解释器中(如`Bash`等)
2. `<`和`>`在`[[ ]]`中用作排序，而`[ ]`不支持
3. `[ ]`中使用`-a`和`-o`表示逻辑与和逻辑或，`[[ ]]`使用`&&`和`||`来表示
4. 文件名扩展：`[[]]` 支持使用 `*` 和 `?` 等通配符进行文件名的扩展匹配，而 `[]` 不支持文件名扩展
5. 正则表达式匹配：`[[]]` 支持使用 `=~` 运算符进行正则表达式的匹配判断，而 `[]` 不支持直接的正则表达式匹配，需要借助其他工具或命令来实现正则匹配

## 7. 条件控制

### 7.1 if语句

语法格式：

```shell
if [ expression ]
then
   Statement(s) to be executed if expression is true
fi
```

注意：`expression` 和方括号`[ ]`之间必须有空格，否则会有语法错误。

示例：

```shell
if [ $a == $b ]
then 
    echo "a is equal to b"
else
    echo "a is less to b"
fi
```

### 7.2 case语句

`case ... esac` 与其他语言中的 `switch ... case` 语句类似，是一种多分枝选择结构

**注意：**

* 取值后面必须为关键字 `in`，每一模式必须以右括号结束。取值可以为变量或常数。
* 匹配发现取值符合某一模式后，其间所有命令开始执行直至 `;;`。`;;` 与其他语言中的 `break` 类似，意思是跳到整个 `case` 语句的最后。

**示例：**

```shell
case $grade in 
    "A")
        echo "Very Good!";;
    "B")
        echo "Good!";;
    "C")
        echo "Come On!";;
    *)
        echo "You Must Try!"
        echo "Sorry!";;
esac
```

**转换成C语言是：**

```shell
switch(grade){
    case 'A': printf("Very Good!");
        break;
    case 'B': printf("Very Good!");
        break;
    case 'C': printf("Very Good!");
        break;
    default: 
        printf("You Must Try!");
        printf("Sorry!");
        break;
}
```

### 7.3 for循环

当变量值在列表里，`for` 循环即执行一次所有命令，使用变量名获取列表中的当前取值。命令可为任何有效的 `Shell` 命令和语句。列表可以包含替换、字符串和文件名

```shell
for 变量 in 列表
do
    command1
    command2
    ...
    commandN
done
```

示例：

```shell
#!/bin/Bash/

for value in 1 2 3 4 5
do 
    echo "The value is $value"
done

# 输出
# The value is 1
# The value is 2
# The value is 3
# The value is 4
# The value is 5
```

### 7.4 while循环

只要while后面的条件满足，就一直执行do里面的代码块。

格式：

```shell
while command
do
   Statement(s) to be executed if command is true
done
```

### 7.5 until循环

`until`语句类似于其他语言的`do while`语句

`condition` 一般为条件表达式，如果返回值为 `false`，则继续执行循环体内的语句，否则跳出循环

```shell
until condition
do
    command
done
```

### 7.6 跳出循环

在循环过程中，有时候需要在未达到循环结束条件时强制跳出循环，像大多数编程语言一样，`Shell`也使用 `break` 和 `continue` 来跳出循环。

1. `break`语句
   * `break` 命令允许跳出所有循环（终止执行后面的所有循环）

2. `continue`
   * `continue`命令与`break`命令类似，只有一点差别，它不会跳出所有循环，仅仅跳出当前循环

## 8. 函数

### 8.1 函数定义

`Shell` 函数的定义格式如下：

其中`function`关键字是可选的。

```shell
function function_name () {
    list of commands
    [ return value ]
}
```

函数返回值，可以显式增加return语句；如果不加，会将最后一条命令运行结果作为返回值

像删除变量一样，删除函数也可以使用 `unset` 命令，不过要加上 `.f` 选项，如下所示：

```shell
$unset .f function_name
```

### 8.2 函数调用

调用函数只需要给出函数名，不需要加括号

### 8.3 函数参数

在Shell中，调用函数时可以向其传递参数。在函数体内部，通过 `$n` 的形式来获取参数的值，例如，`$1`表示第一个参数，`$2`表示第二个参数...

带参数的函数示例：

```shell
#!/bin/Bash

funWithParam () {
    echo "第一个参数为 $1 !"
    echo "第二个参数为 $2 !"
    echo "第十个参数为 $10 !"
    echo "第十个参数为 ${10} !"
    echo "参数总数有 $# 个!"
}
funWithParam 1 2 3 4 5 6 7 8 9 34 73

# 输出
# 第一个参数为 1 !
# 第二个参数为 2 !
# 第十个参数为 10 !
# 第十个参数为 34 !
# 第十一个参数为 73 !
# 参数总数有 11 个!
```

注意，`$10` 不能获取第十个参数，获取第十个参数需要`${10}`。当n>=10时，需要使用`${n}`来获取参数
