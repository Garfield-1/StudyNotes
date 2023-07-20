# Lua程序设计

参考文档

1.《Lua程序设计（第二版）》 --电子工业出版社  [巴西]Roberto Ierusalimschy 著  周惟迪 译



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

  * 使用`ASCII`码来指定字符串中的字符，如`\97   a`、`\10   \n`

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

#### 1.5.1 table的特性

* 一个`table`中包含许多成员，每个条目包含一个索引和一个元素，索引和元素为一组键值对
* `table`的索引初始值默认为1，且索引不可以使用负数
* `table`的长度可以动态增长
* 不同的变量可以持有相同的`table`，一个持有`table`的变量与`table`自身之间没有固定的关联性
* 当没有任何对`table`的引用时，`Lua`的垃圾回收器会删除该`table`并复用它的内存

#### 1.5.2 table初始化与赋值

* 在定义时初始化

  * 最简单的构造式就是一个空构造式即`{}`，用于创建一个空`table`

    ```lua
    table_tese = {}
    ```

  * 也可以使用构造式在初始化`table`时进行赋值

    ```lua
    local local_A = "local_A"
    local num_3 = 3
    
    table_test1 = {
        --风格1.初始化时使用“=”赋值--
        style_a_1 = "a",
        style_a_2 = 1,
        style_a_3 = "2",
        style_a_4 = local_A,
    
        --风格2.使用将索引值防在方括号中--
        [1] = 1,
        [2] = "a",
        [num_3] = local_A,
    
        --风格3.Lua提供了一种更加通用的风格，这种格式允许使用负数或运算符作为索引--
        ["-1"] = "2",
        ["+"] = "3"
    }
    
    --风格4.仅填充元素值，索引值由1开始自动依次递增--
    table_test2 = { "a", "b", "c", "d" }
    
    --风格5.仅填充元素值，并指定初始索引值，索引值由初始值开始自动依次递增--
    table_test3 = { [0] = "a", "b", "c", "d" }
    ```

* 在创建后修改`table`中的某些字段

#### 1.5.3 table遍历

* `pairs`可以遍历所有成员
  * `Luajit`中使用`for key, value in pairs(table) do`这样的遍历顺序并非是代码中`table`中的排列顺序，而是根据`table`中key的hash值的排列顺序来遍历，key的hash是以时间戳相关的随机值作为种子生成的，导致最终打印的值的顺序是随机值
* `ipairs`只遍历1，2，数值型下标，下一个3不连续就中断了
* 由于`#tbtest`获取的值为4，故下标3的值为`nil`
* `table`中元素的顺序随机，每次执行时打印顺序都不同

```lua
table_1 = {
    a = "a",
    [1] = "1",
    b = "b", 
    c = "c",
    [2] = "2",
    [4] = "4"
}

for key, value in pairs(table_1) do      
    print(tostring(key)..":"..tostring(value))
end
结果：
1:1
2:2
4:4
a:a
b:b
c:c

for key, value in ipairs(table_1) do      
    print(tostring(key)..":"..tostring(value))
end
结果：
1:1
2:2

for i=1, #table_1 do 
    print(tostring(i)..":"..tostring(table_1[i]))
end
结果：
1:1
2:2
3:nil
4:4
```

