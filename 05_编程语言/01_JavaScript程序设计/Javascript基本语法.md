[TOC]

# Javascript基本语法

参考文档：

1. JavaScript高级程序设计(中文)-第3版  人民邮电出版社    【美】Nicholas C.Zakas 著        李松峰  曹力 译
2. MDN Web Docs [MDN Web Docs (mozilla.org)](https://developer.mozilla.org/zh-CN/)



## 1. 在HTML中使用JavaScript

* 脚本可位于 `HTML` 的 `<body>` 或 `<head>` 部分中，或者同时存在于两个部分中。

* 通常的做法是把函数放入 `<head>` 部分中，或者放在页面底部。这样就可以把它们安置到同一处位置，不会干扰页面的内容。
  * 在`<head>`中的`JavaScript`函数

    ```javascript
    <!DOCTYPE html>
    <html>
    
    <head>
        <script>
            function myFunction() {
                document.getElementById("demo").innerHTML = "我的第一个 JavaScript 函数";
            }
        </script>
    </head>
    
    <body>
        <h1>我的 Web 页面</h1>
        <p id="demo">一个段落</p>
        <button type="button" onclick="myFunction()">尝试一下</button>
    </body>
    
    </html>
    ```
  
  * `<body>` 中的 JavaScript 函数
  
    ```javascript
    <!DOCTYPE html>
    <html>
    
    <body>
        <h1>我的 Web 页面</h1>
        <p id="demo">一个段落</p>
        <button type="button" onclick="myFunction()">尝试一下</button>
        <script>
            function myFunction() {
                document.getElementById("demo").innerHTML = "我的第一个 JavaScript 函数";
            }
        </script>
    </body>
    
    </html>
    ```



## 2.数据类型

* `JavaScript` 拥有动态类型。这意味着相同的变量可用作不同的类型；变量的数据类型可以使用 **`typeof`** 操作符来查看

### 2.1 基本类型

* `JavaScript`中有6种值类型（基本类型）变量

  * `String` （字符串）

    * 字符串是存储字符的变量，字符串可以是引号中的任意文本。您可以使用单引号或双引号

      ```javascript
      var carname="Volvo XC60";
      var carname='Volvo XC60';
      
      //在字符串中包含引号
      var answer="He is called 'Johnny'";
      var answer='He is called "Johnny"';
      ```

  * `Number` （数字）

    * `JavaScript` 只有一种数字类型。数字可以带小数点，也可以不带

      ```javascript
      var x1=34.00;          //使用小数点来写
      var x2=34;             //不使用小数点来写
      
      /* 科学计数法 */
      var y=123e5;           // 12300000
      var z=123e-5;          // 0.00123
      ```

  * `Boolean` （布尔）

    * 布尔（逻辑）只能有两个值：true 或 false

      ```javascript
      var x=true;
      var y=false;
      ```

  * `Null` （空）

    * `Null`类型只有一个值，即特殊的`Null`，从逻辑角度看null值表示一个空指针对象

  * `Undefined` （未定义）

    * `Undefined` 类型只有一个值，即特殊的`undefined`，在使用`var`声明变量但未对其加以初始化时，这个变量的值就是`undefined`

      ```javascript
      var x;
      ```

  * `Symbol`

    * `symbol`是一个`ES6标准`种新增的一种基本数据类型；`symbol` 的值是通过 `Symbol()` 函数生成，每一个 `symbol` 的值都是唯一的，并且可以作为对象的属性标识符使用


### 2.2 对象类型

* 三种引用数据类型（对象类型）

  * `Object`（对象）

    * 对象由花括号分隔。在括号内部，对象的属性以名称和值对的形式 (`name : value`) 来定义，属性由逗号分隔

      ```javascript
      /* 创建Object变量并初始化
       * 风格1.使用new操作符后跟Object构造函数
       */
      var person = new Object();
      person.name = "tom";
      person.age = "3";
      
      /* 风格2.在创建变量时直接赋值 */
      var person = {
          name:"tom",
          age:"3"
      };
      ```

  * `Array`（数组）

    * 数组中的每一项都可以保存任何类型的数据

    * 数组的大小是可以动态调整的，即可以随着数据的添加自动增长以容纳新增数据

      ```javascript
      /* 创建数组长度基本方式 
       * 方式1.使用Array构造函数
       */
      var colors_1 = new Array();       					//使用Array构造函数创建数组
      var colors_2 = new Array(20);     					//创建数组时指定数组的长度
      var colors_3 = new Array("red", "blue", "green");	//在构建函数时指定数组中包含的值   
      var colors_4 = new Array("Greg");   				//创建一个包含1项，即字符串“Greg”的数组
      
      /* 方式2.创建数组时指定数组的内容 */
      var arr_1 = ["a", "b", "c"];    					//创建一个包含三个字符串的数组
      var arr_2 = [];         							//创建一个空数组
      ```

    * 数组元素可以通过数组下标的形式访问

  * `Function`（函数）

    * 在`JavaScript`中函数实际上是对象，每个函数都是`Function`类型的实例，而且都与其他引用类型一样具有属性和方法；由于函数是对象，因此函数名实际上也是一个指向函数对象的指针，不会和某个函数绑定

      ```javascript
      function sum (num1, num2) {
          return num1 + num2;
      }
      
      /* 调用变量sum_3等价于调用函数sum */
      var sum_3 = function sum_1 (num1, num2) {
          return num1 + num2;
      };
      ```

    * `JavaScript`不支持函数重载特性，如果连续声明两个同名函数，结果是后面的函数覆盖了前面的函数。

### 2.3 特殊类型对象

* 两种特殊的对象

  * `RegExp`（正则）

    * `JavaScript`通过`RegExp`类型来支持正则表达式，其中的模式部分可以是任何简单或复杂的正则表达式

    * 每个表达式都可带有一或多个标志，用以标明正则表达式的行为

      * `g`：表示全局（`global`）模式，即模式将被应用于所有字符串，而并非在发现第一个匹配项时立即停止

      * `i`：表示不区分大小写模式，即确定匹配项时忽略模式与字符串的大小写

      * `m`：表示多行模式，即在到达一行文本末尾时还会继续查找下行中是否存在与模式匹配的项

        ```javascript
        var pattern_1 = /ab+c/i; //字面量形式
        var pattern_2 = new RegExp("ab+c", "i"); // 首个参数为字符串模式的构造函数
        var pattern_3 = new RegExp(/ab+c/, "i"); // 首个参数为常规字面量的构造函数
        ```

  * `Date`（日期）

    * `Date`类型是在早期Java中的`java.util.Date`类基础上构建的。为此`Date`类型使用来自`UTC`时间的1970年1月1日0时开始经过的毫秒数来保存日期

      ```javascript
      /* 创建一个日期对象 */
      var now = new Date()
      ```

 

## 3.操作符与语句

* `JavaScript`中大部分语句和操作符格式、风格、使用方法与`C`语言相同，在此仅针对部分讲解，其余部分不做赘述

### 3.1 for-in语句

* `for/in` 语句常用于循环遍历对象的属性

  ```javascript
  var obj = { a: 1, b: 2, c: 3 };
  
  for (var prop in obj) {
      console.log("obj." + prop + " = " + obj[prop]);
  }
  /* 输出：
   * "obj.a = 1"
   * "obj.b = 2"
   * "obj.c = 3"
   */ 
  ```



## 4. 函数表达式

### 4.1 函数创建

#### 4.1.1 函数声明和函数表达式

* 定义函数的方式有函数声明、函数表达式

  ```javascript
  /* 使用使用function创建函数 */
  function functionName(arg0, arg1, arg2) {
      //函数体
  }
  
  /* 使用函数表达式创建函数 */
  var functionName = function (argo, argl, arg2) {
      //函数体
  };
  ```

* 使用函数表达式创建函数，这样创建的函数称为匿名函数

* 这种方式`function`关键字后面没有标识符，匿名函数的`name`属性是空字符串

#### 4.1.2 箭头函数

* ES6 新增了箭头函数，箭头函数表达式的语法比普通函数表达式更简洁

    ```javascript
    (参数1, 参数2, …, 参数N) => { 函数声明 }
    
    (参数1, 参数2, …, 参数N) => 表达式(单一)
    // 相当于：(参数1, 参数2, …, 参数N) =>{ return 表达式; }
    ```

* 当只有一个参数时圆括号是可选的

    ```javascript
    (单一参数) => {函数声明}
    单一参数 => {函数声明}
    ```

* 没有参数的函数应该写成一对圆括号

    ```javascript
    () => {函数声明}
    ```

### 4.2 闭包

* `JavaScript`中允许在一个函数中定义另一个函数

  * 内部函数可以使用外部函数的局部变量，即使这个内部函数被返回了，而且是在其他地方被调用了，但他任然可以访问外部函数的局部变量

* 有权访问另一个函数作用域中的变量的函数称为闭包

  ```javascript
  function createCounter() {
      let count = 0;
  
      return function increment() {
          count++;
          console.log(count);
      };
  }
  
  const counter = createCounter();
  
  counter(); // 输出 1
  counter(); // 输出 2
  counter(); // 输出 3
  ```



## 5. 变量、作用域和内存问题

### 5.1 私有变量

* 任何在函数中定义的变量，都可认为是私有变量，因为不能在函数外部访问这些变量

  * 私有变量包括函数的参数、局部变量和在函数内部定义的其他函数、

* 我们把有权访问私有变量和私有函数的公有方法称为特权方法(`privileged method`)

  ```javascript
  function Myobject() {
      //私有变量和私有函数var privateVariable = 10;
      function privateFunction() {
          return false;
      }
      //特权方法
      this.publicMethod = function () {
          privateVariable++;
          return privateFunction();
      };
  }
  ```

### 5.2 执行环境及作用域

### 5.2.1 执行环境

* 执行环境是`JavaScript`中的最重要的一个概念，执行环境定义了变量或函数有权访问的其他数据；每个执行环境都有一个与之关联的变量对象，环境中定义的所有变量和函数都保存在这个对象中，解释器在处理数据时会在后台使用它
* 在`web`浏览器中，全局执行环境被认为是`windows`对象，因此所有全局变量和函数都是作为`windows`对象的属性和方法创建的

### 5.2.2 作用域

* `JavaScript` 的作用域分以下三种：
  * 全局作用域：脚本模式运行所有代码的默认作用域
  * 模块作用域：模块模式中运行代码的作用域
  * 函数作用域：由函数创建的作用域
* 在 `JavaScript` 中，`ES6`（`ECMAScript 2015`）引入了块级作用域的概念。块级作用域意味着变量和函数的作用域限定在由花括号 `{}` 包裹的代码块（例如，`if` 语句、`for` 循环等）内部。在 `ES6` 之前，`JavaScript` 只有函数作用域，

## 5.3 垃圾回收

* `JavaScript`具有自动垃圾收集机制，也就是说，执行环境会负责管理代码执行过程中使用的内存。

### 5.3.1 标记清除

* `JavaScript`中最常用的垃圾收集方式是标记清除

### 5.3.2 引用计数

* 另一种垃圾回收策略叫做引用计数(`reference counting`)。引用计数是指跟踪记录每个值被引用的次数。当声明了一个变量并将一个引用类型值赋给该变量时，则这个值的引用次数就是`1`。如果同一个值又被赋给另一个变量，则该值的引用次数加`1`。相反，如果包含对这个值引用的变量又取得了另外一个值，则这个值的引用次数减`1`。当这个值的引用次数变成0时，则说明没有办法再访问这个值了，因而就可以将其占用的内存空间回收回来。



## 6. 面对对象编程

