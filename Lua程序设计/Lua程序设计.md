# Lua程序设计

参考文档

1.《Lua程序设计（第二版）》--电子工业出版社



## 1. 类型与值

`Lua`是一种动态类型语言，在语言中没有类型定义的语法，任何变量都可以包含任何类型的值；`Lua`中有8中基础类型，可以使用函数`type`返回一个值的类型名称

* `nil`空

  ```lua
  print(type(nil))					--->nil
  ```
  
* `boolean`布尔

  ```lua
  print(type(true))					--->boolean
  ```
  
* `number`数字

  ```lua
  print(type(10))						--->number
  ```
  
* `string`字符串

  ```lua
  print(type("Hello world"))			--->string
  ```
  
* `userdata`自定义类型

* `function`函数

  ```lua
  print(type(print))					--->function
  ```
  
* `thread`线程

* `table`表

  ```lua
  local a ={}
  print(type(a))						--->table
  ```

如果一个变量没有初始化，它的类型为nil

```lua
local a
print(type(a))							--->nil
```



### 1.1 nil（空）

​	`nil`类型是一种特殊的类型，仅有一个值`nil`；其主要功能就是用于区分其他任何值，一个全局变量在第一次赋值前的默认值就是`nil`，将`nil`赋予一个全局变量等同于删除它，`Lua`将`nil`用于表述一种无效值的情况



### 1.2 Boolean（布尔）

​	`boolean`类型有两个值可以选择`false`和`true`；`Lua`将`false`和`nil`视为“假”，其余之外的其他值视为“真”



### 1.3 number（数字）

​	`Lua`中对于整数和浮点数不进行区分，全部使用`number`类型表示



### 1.4 string（字符串）

* `Lua`中的字符串需要以一对匹配的单引号或者双引号来界定

* 可以在字符串中使用类似于C语言中的转义字符

  * 使用字母进行转义，如`\n   换行`、 `\r   回车`

  * 使用ASCII码来指定字符串中的字符，如`\97   a`、`\10   \n`

  * 如果不想使用转义序列，可以使用一对匹配的双方括号界定一个字符串`Lua`不会解释其中的转义字符，这种形式的字符串可以延伸多行

    ```lua
    a = "\97\97"
    b = [[\97\97]]
    
    print(a)				--->aa
    print(b)				--->\97\97
    ```

* 字符串连接操作符`..`，当直接在一个数字后面输入它时必须要用一个空格来分隔它们，不然`Lua`会将第一个点理解为小数点，或者直接报错

  ```lua
  a = "abc"
  b = 123
  
  print(a..123)				--->abc123
  print(123 ..456)			--->123456
  ```

* 在字符串前放置操作符`#`可以获得该字符串的长度

  ```lua
  a = "abc"
  
  print(#a)					--->3
  ```



### 1.5 table(表)

​	`Lua` 中的表（table）其实是一个"关联数组"（associative arrays），数组的索引可以是数字、字符串或表类型。在 `Lua` 里，table 的创建是通过"构造表达式"来完成，最简单构造表达式是{}，用来创建一个空表。

* `Lua table`是不固定大小的，可以根据自己的需要进行扩充
* `Lua table` 使用关联型数组，你可以用任意类型的值来作数组的索引，但这个值不能是 nil
* `Lua`也是通过table来解决模块（module）、包（package）和对象（Object）的管理

#### 1.5.1 table初始化与赋值

* 一个table中包含许多条目，每个条目包含一个索引和一个元素
* table的长度可以动态增长
* 不同的变量可以持有相同的table，一个持有table的变量与table自身之间没有固定的关联性
* 当没有任何对table的引用时，`Lua`的垃圾回收器会删除该table并复用它的内存

