# Lua程序设计

参考文档

1. 《Lua程序设计（第二版）》 							--电子工业出版社  [巴西]Roberto Ierusalimschy 著  周惟迪 译
2. 菜鸟教程(RUNOOB.COM)



## 1. 类型与值

​	`Lua`是一种动态类型语言，在语言中没有类型定义的语法，任何变量都可以包含任何类型的值；`Lua`中有8中基础类型，可以使用函数`type`返回一个值的类型名称

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

  ```lua
  --风格1. 使用方括号--
  table_test["a"] = 3         --->将索引a对应的值更改为3
  table_test[1] = "c"         --->将索引1对应的值更改为c
  table_test[2] = "d"         --->新增索引2并将元素的值赋值为c
  
  --风格2.使用在table变量后使用"."连接索引值,但索引值不可以为数组--
  table_test.a = 3
  
  --删除元素,将索引对应的值置为nil--
  table_test.a = nil
  table_test[1] = nil 
  ```


#### 1.5.3 table遍历

* `pairs`可以遍历所有成员
  * `Luajit`中使用`for key, value in pairs(table) do`这样的遍历顺序并非是代码中`table`中的排列顺序，而是根据`table`中key的hash值的排列顺序来遍历，key的hash是以时间戳相关的随机值作为种子生成的，导致最终打印的值的顺序是随机值
* `ipairs`只遍历1，2，数值型下标，下一个3不连续就中断了
* 由于`#tbtest`获取的值为4，故下标3的值为`nil`

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



## 2. 语句

* `Lua`支持的常规语句基本上和C语言中的差不多，这些语句包括赋值、控制结构、过程调用等。另外`Lua`还支持一些不太常见的语句，比如多重赋值和局部变量声明



### 2.1 多重赋值语句

* `Lua`允许多重赋值，也就是一次将多个值赋给多个变量，每个值和变量之间用逗号分隔

  * 如果等号两侧的值数量不同的话，多余的变量会被赋值为`nil`,如果值的个数更多的话，多余的值会被丢弃

    ```lua
    x, y = 10, 10*10
    
    print(x.. "：" ..y)				--->10:100
    ```

    

### 2.2 局部变量和块

* 相对于全局变量，Lua还提供了局部变量，通过local语句来创建局部变量

  * 局部变量的作用域仅限于声明它们的那个块

    > 一个块是一个控制结构的执行体、或者是一个函数的执行体或是一个程序块

* 局部变量通常会随着其作用域的结束而消失，相对应的内存资源由垃圾回收机制进行回收



## 3.控制结构

​	控制结构中的条件表达式可以是任何值，`Lua`将所有不是`false`和`nil`的值视为真



### 3.1 if then else

* 判断条件放置在`if`和`then`之间，函数体在`then`之后，整个判断语句程序块以`end`结束

* 多层嵌套结构使用`elseif`语句

  ```lua
  if a > b then
      print("a > b")
  elseif a == b then
      print("a = b")
  else
      print("a < b")
  end
  ```



### 3.2 while循环

* 先判断`while`的条件是否为真，如果条件为假那么结束循环，不然执行循环体并重复这一过程

  ```lua
  local i = 1
  while a[i] do
      print(a[i])
      i = i + 1
  end
  ```



### 3.3 repeat循环

* 可视为C语言中的`do while`语句

* 一条repeat-until语句重复执行其循环体直到条件为真时结束。检测条件是在循环体之后做的，循环体会至少执行一次

  ```lua
  --打印输入的第一行不为空的内容--
  repeat
      line = io.read()
  until line ~= ""
  print(line)
  ```

  

### 3.4 for循环

* `for`语句有两种形式：数字型`for`和泛型`for`

#### 3.4.1 数字型for循环

* `var`从`exp1`变化到exp2`，`每次变化都以`exp3`作为步长递增`var`，并执行一次循环体。第三个表达式`exp3`是可选参数，如果不指定的话，`Lua`会将步长默认为1

  ```lua
  for var = exp1, exp2, exp3 do
      <执行体>
  end
  
  --例如,依次打印从100到1--
  for i = 100, 1, -1 do
      print(i)
  end
  ```

* 如果不想给循环设置上限的话，可以使用常量`math.huge`

  ```lua
  --无限循环打印--
  for i = 1, math.huge do
      print(i)
  end
  ```

* 在`for`循环中可以使用`break`语句退出循环

* `for`循环的三个表达式是在循环开始前一次性求值的

* 控制变量会被自动的声明为`for`循环语句的局部变量，并且仅在循环体内可见，因此控制变量在循环结束后就不存在了

#### 3.4.2 泛型for循环

* 泛型for循环通过一个迭代器函数来遍历所有值

  ```lua
  for key, value in pairs(table_1) do      
      print(tostring(key)..":"..tostring(value))
  end
  ```

#### 3.4.3 迭代器

* 迭代器是一种可以遍历集合中所有元素的机制，lua 中迭代器使用函数表示，每调用函数一次，返回集合中的下一个元素

  * 迭代器的使用方法是定义一个工厂，然后利用这个工厂生产闭包，然后调用这个闭包来获取集合中的元素。这种方式的缺点就是每次都得创建一个新的闭包，有一定的开销

  * 使用迭代器的方式也很简单，直接调用迭代器就相当于调用闭包函数，迭代器会返回元素集合的下一个元素，当返回 nil 时就代表元素遍历完了

    ```lua
    function value(t)
        local i = 0
        return function()
            i = i + 1
            return t[i]
        end
    end
    
    t = {1, 2, 3, 4, 5}
    -- 创建一个迭代器
    iter = value(t)
    -- 遍历迭代器
    while true do
        local ele = iter()
        if ele == nil then
            break
        end
        print(ele)
    end
    ```

* 无状态迭代器

  * 无状态迭代器指的是在循环过程中，不保留任何状态的迭代器，这样我们就可以避免创建闭包所花费的代价。典型的无状态迭代器就是：`ipairs`

  * 泛型 for 循环的设计本身就包含迭代器，而且是一种无状态的迭代器，即不需要创建闭包

  * 泛型 for 循环内部保存了迭代器状态，包括迭代器函数、控制变量和恒定状态

    * 迭代器函数：迭代器工厂产生的匿名函数，不是闭包
    * 控制变量：for 循环使用这个变量控制当前遍历的位置及何时结束遍历
    * 恒定状态：迭代器遍历的目标，一般是指 table

    ```lua
    iter = function(t, i)
        i = i + 1
        local v = t[i]
        if v then
            return i, v
        end
    end
    
    ipairs = function(t)
        return iter, t, 0
    end
    
    for i, v in ipairs(t) do
        print(i, v)
    end
    ```

* 多状态迭代器

  * 泛型 for 循环可以实现只有一个恒定状态的迭代器，如果有多个状态需要保存就不行了。如果要保存多个状态，可以有下面两种方式

    * 使用闭包
    * 把多个状态封装在一个 `table` 里面，然后使用泛型 for 循环

    ```lua
    array = {"Google", "Runoob"}
    
    function elementIterator (collection)
       local index = 0
       local count = #collection
       -- 闭包函数
       return function ()
          index = index + 1
          if index <= count
          then
             --  返回迭代器的当前元素
             return collection[index]
          end
       end
    end
    
    for element in elementIterator(array)
    do
       print(element)
    end
    ```

    

