参考文献：
C语言通用编程规范 V1.0     --XXXX终端有限公司

 [TOC]
# 1.前言
    代码编写规则应该遵循清晰、简洁、风格统一的原则。


    Programs are meant to be ready by humans and only icidentally for computers to execute.（代码是用来让人读的，只是顺便让机器执行而已）
    --Donald Ervin Knuth(唐纳德.克努特)
# 2.命名
* `正式工作中命名风格采用驼峰风格`

## 2.1驼峰式命名法
  * 由于大小写混用变量名看上去就像骆驼的驼峰一样高低起伏，故而得名。
  * 名称大小写混用，不同单词之间通过首字母大写进行区分 。
  * 小驼峰法：第一个单词首字母小写后面的单词全部首字母大写，例char myStudyNotes;
  * 大驼峰法：所有单词首字母全部大写，例char MyStudyNotes；

## 2.2内核风格（下划线定义法）
  * 全部单词使用小写，不同单词之间使用下划线分割，例char my_study_notes;
## 2.3匈牙利风格
  * 在‘大驼峰’的基础上，加上类型或用途前缀，例char cMyStudyNotes；

  |  前缀   | 对应类型  |
  |  ----  | ----  |
  | a  | 数组（Array） |
  | b  | 布尔值（Boolean） |
  | by  | 字节（Byte）|
  | c  | 有符号字符（Char） |
  | cb  | 无符号字符（Char Byte) |
  | cr  | 颜色参考值（Color Ref） |
  | cx,cy  | 坐标差（长度 Short Int） |
  | dw  | 双字（Double World） |
  | fn  | 函数（Function） |
  | h  | 句柄（Handle） |
  | i  | 整形（Int） |
  | l  | 长整型(Long Int) |
  | lp  | 长指针（Long Pointer） |
  | m_  | 类成员（Class Member） |
  | n  | 短整型（Short Int） |
  | np  | 近程指针（Near Pointer） |
  | p  | 指针（Pointer） |
  | s  | 字符串（String） |
  | sz  | 以NULL做结尾的字符串型（String with Zero End) |
  | w  | 字（World） |
		
## 2.4不同类型变量的命名规则
  |  类型   | 风格  |
  |  ----  | ----  |
  | 函数，结构体类型，枚举类型，联合体类型  | 大驼峰 |
  | 变量，函数参数，宏参数，结构体中字段，联合体中成员  | 小驼峰 |
  | 宏，常量，枚举值，goto 标签  | 全大写，下划线分割 |

  * 常量是指，全局作用域下，const 修饰的基本数据类型、枚举、字符串类型的变量。
  * 变量是指，全局作用域下，const 修饰的基本数据类型、枚举、字符串类型的变量。
  * 模块前缀不能超过两级

## 2.5文件命名规则

  * `文件名统一采用全小写字母命名。`
  * 用下划线(_)分开。禁止使用空格。
  * 文件名应尽量简短、准确、无二义性。

# 3.排版格式
## 3.1整体规范
### 3.1.1行宽
  * 行宽不得超过120个字符   

### 3.1.2缩进
  * `使用空格缩进，每次缩进四个空格`

### 3.1.3表达式
  * 表达式换行时要保持换行的一致性
  * 操作符放行尾。

  ```
  int sum=longVaribleName1 + longVaribleName2 + longVaribleName3 +
      longVaribleName4 + longVaribleName5 + longVaribleName6;// OK: 4空格缩进

  int sum=longVaribleName1 + longVaribleName2 + longVaribleName3 + longVaribleName4 + longVaribleName5 +              longVaribleName6;// OK: 保持对齐
  ```
  
## 3.2函数
### 3.2.1大括号
  * K&R风格，Kernighan和Ritchie共同完成C语言”白皮书”C Programing Language一书的时候使用的风格。
  * 左边的花括号紧跟函数名称，放在行末。
  * 如遇到需要换行的情况时，函数左大括号另起一行放行首，独占一行；其余大括号跟随语句放行末；

  ```
  if (line_num == MAX_LINES) {
       line_num = 0;
       page_num++;
   }
  ```

### 3.2.2声明定义和调用
  * 函数的返回值类型和函数名应在同一行
  * 函数参数列表换行时，应合理对齐
  * 参数列表左圆括号和函数名在同一行，右圆括号跟随最后一个参数

  ```
  ReturnType FunctionName(ArgType paramName1, ArgType paramName2) // Good：全在同一行
  {
  ...
  }

  ReturnType VeryVeryVeryLongFunctionName(ArgType paramName1, // 行宽不满足所有参数，进行换行
                                          ArgType paramName2, // Good：和上一行参数对齐
                                          ArgType paramName3)
  {
  ...
  }

  ReturnType LongFunctionName(ArgType paramName1, ArgType paramName2, // 行宽限制，进行换行
      ArgType paramName3, ArgType paramName4, ArgType paramName5) // Good: 换行后 4 空格缩进
  {
  ...
  }

  ```

### 3.2.3条件语句&循环语句
  * 必须使用大括号，规则见上文
  * 禁止if/else写在同一行

  ```
  if (someConditions) {
  ...
  } else {    // Good: else 与 if 在不同行
  ...
  }


  switch 语句的 case/default 要缩进一层
  switch (var)
      case 0:
          DoSomething1(); // Good: 缩进
          break;
      case 1: {   // Good: 带大括号格式
          DoSomething2(); break;
      }
      default:
          break;
  ```

# 4.初始化
  * 初始化时如需要换行则需要有缩进，并进行合理对齐
  * 对于复杂结构的初始化，应当清晰、紧凑

## 4.1简单变量初始化和赋值
  * 每行只有一个变量初始化语句
  * `对于多个相关性强的变量定义，且无需初始化时，可以定义在一行`

  ```
  //需要初始化时，每行应只有一个初始化语句
  int maxCount = 10;
  bool isCompleted = false;

  //例外情况
  int i, j; // Good：多变量定义，未初始化，可以写在一行
  for (i = 0; i < row; i++) {
      for (j = 0; j < col; j++) {
  ...
      }
  }
  ```

## 4.2复杂结构初始化
### 4.2.1结构体或联合体初始化
  * `结构体和联合体在按成员初始化时，每个成员初始化单独一行`

  ```
  struct Date {
      int year; int month; int day;
  };

  struct Date date = {    // Good：使用指定初始化方式时，每行初始化一个
      .year   = 2000,         //此处为“指定初始化”方式
      .month = 1,
      .day    = 1
  };
  ```

### 4.2.2数组初始化
  * 一维数组初始化时，如果参数过长应合理换行

  ```
  int rank[] = {                  // Good: 行宽较长时，换行让可读性更好
      16, 16, 16, 16, 32, 32, 32, 32,
      64, 64, 64, 64, 32, 32, 32, 32
  };
  ```

  * 二维数组初始化时，应当遵循清晰、紧凑的原则
  * 换行时需要缩进
  ```
  int a[][4] = {
      { 1, 2, 3, 4 }, { 2, 2, 3, 4 }, // OK.
      { 3, 2, 3, 4 }, { 4, 2, 3, 4 }
  };
  ```
### 4.2.3指针初始化
  * 声明或定义指针变量或者返回指针类型函数时，"\*" 靠左靠右都可以
  * 不允许出现两边都有或者都没有空格
  * `需要一行同时声明多个变量时，"\*"   跟随变量`

  ```
  int *p1;    // OK.
  int* p2;    // OK.

  int*p3;     // Bad: 两边都没空格
  int * p4;   // Bad: 两边都有空格

  int *p, *q; // OK.
  int* a, b;  // Bad: 很容易将 b 误理解成指针
  ```
# 5.注释
  * 在 C 代码中，使用`/\*  \*/`和`//`都是可以的
## 5.1文件头注释
  * `文件头注释必须包含版权许可、功能说明、作者和创建日期 ；后面以相同格式补充。比如：注意事项、修改历史、等等`
  * 版权许可内容及格式必须如下
  ```
          中文版：

  版权所有 (c) 荣耀终端有限公司 2020-2021

          英文版：

  Copyright (c) Honor Device Co., Ltd. 2020-2021. Allrights reserved.
  ```


  * 其中，2020-2021 根据实际需要可以修改， 2020 是文件首次创建年份，而 2021 是最后文件修改年份。对文件有重大修改时，必须更新后面年份。
  * `文件头注释必须从文件顶头开始,项目内保持统一格式。`
  增加包含了 'Note' 和 'History 的文件头格式如下
  ```
  /*
  * Copyright (c) Honor Device Co., Ltd. 2020-2021. All rights reserved.
  * Description: 文件功能描述
  * Author: 王二
  * Create: 2020-12-22
  * Notes: 特别需要注意的信息
  * History: 2021-01-01 张三 某特性扩展
  * 2021-11-30 李四 某重大修改
  */
  ````

## 5.2函数头注释
  * 并不是所有的函数都需要函数头注释；
  * 函数签名无法表达的信息，加函数头注释辅助说明;
  * 函数头注释统一放在函数声明或定义上方；
  * `函数头注释内容可选，但不限于：功能说明、返回值，性能约束、用法、内存约定、算法实现、可重入的要求等。`

  ```
  /*
  * 返回实际写入的字节数，-1表示写入失败
  * 注意，内存 buf 由调用者负责释放
  */
  int WriteString(char *buf, int len);
  ```

## 5.3代码注释
  * 代码注释放于对应代码的上方或右边
  * 注释符与注释内容间要有1空格；右置注释与前面代码至少1空格

  ```
  /* 这是单行注释 */ DoSomething();

  /*
  * 这是单/多行注释
  * 第二行
  */ DoSomething();


  #define A_CONST 100         /* 相关的同类注释，可以考虑上下对齐 */
  #define ANOTHER_CONST 200   /* 上下对齐时，与左侧代码保持间隔 */
  ```

### 5.3.1其他注释规则
#### 5.3.1.1不用的代码段直接删除，不要注释掉
  * 被注释掉的代码，无法被正常维护；
  * 当企图恢复使用这段代码时，极有可能引入易被忽略的缺陷。
  * `正确的做法是，不需要的代码直接删除掉。若再需要时，考虑移植或重写这段代码。`

#### 5.3.1.2 正式交付给客户的代码不能包含 TODO/TBD/FIXME 注释
  * TODO/TBD 注释一般用来描述已知待改进、待补充的修改点
  * FIXME 注释一般用来描述已知缺陷
  * 它们都应该有统一风格，方便文本搜索统一处理。

  ```
  // TODO(<author-name>): 补充XX处理
  // FIXME: XX缺陷
  ```

#### 5.3.1.3case语句块结束时如果不加break/return，需要有注释说明(fall- through)
 
  * 有时候需要对多个case标签做相同的事情，case语句在结束不加break或return，直接执行下一个case标签中的语 句，这在C语法中称之为"fall-through"。
  * 这种情况下，需要在"fall-through"的地方加上注释，清晰明确的表达出这样做的意图；或者至少显式指明是 "fall- through"。

  ```
  /*
  * 例，显式指明 fall-through
  * 特别的，如果 case 语句是空语句，则可以不用加注释特别说明
  */
  switch (var) { 
      case 0:
          DoSomething();
          /* fall-through */ 
      case 1:
          DoSomeOtherThing();
          ...
          break; 
      default:
          DoNothing(); 
          break;
  }
  ```

# 6.头文件
  * 每一个.c文件都应该有相应的.h文件，用于声明需要对外公开的接口
  * 每一个功能模块都应该提供一个单独.h，用于声明模块整体对外提供的接口
  * 头文件的扩展名只使用.h
  * `禁止包含用不到的头文件`
  * `头文件应当自包含（简单的说，自包含就是任意一个头文件均可独立编译。如果一个文件包含某个头文件，还要包含另外一个头文件才 能工作的	话，给这个头文件的用户增添不必要的负担）`
  * `头文件必须编写#deﬁne保护，防止重复包含`

  ```
  #ifndef VOS_INCLUDE_TIMER_H // 测试#idndef后内容是否被宏定义过, 如果没有被定义过则执行被保护内容
  #define VOS_INCLUDE_TIMER_H 

  //被保护内容

  #endif//否则执行#endif后内容
  ```

  * `禁止通过声明的方式引用外部函数接口、变量`
  * `禁止在 extern "C" 中包含头文件`

# 7.函数

  * 函数的作用：避免重复代码、增加可重用性；分层，降低复杂度、隐藏实现细节，使程序更加模块化，从而更有利 于程序的阅读，维护
  * 函数应该简洁、短小
  * 一个函数只完成一件事情

## 7.1函数设计规则
### 7.1.1函数长度
  * 避免函数过长，函数不超过50行（非空非注释）

### 7.1.2函数嵌套
  * 函数的代码块嵌套深度指的是函数中的代码控制块（例如：if、for、while、switch等）之间互相包含的深度
  * `函数嵌套深度不能超过四层`

### 7.1.3函数错误返回值
  * 一个函数能够提供一些指示错误发生的方法。 可以通过使用错误标记、特殊的返回数据或者其他手段，调用程序应该在函数返回时立刻检查错误指	示。
  * 如果所有调用者都不关注函数返回值时，请将函数设计成void型

  ```
  char fileHead[128];
  ret = ReadFileHead(fileName, fileHead, sizeof(fileHead));
  if (ret != OK) {    // Good: 确保 fileHead 被有效写入
      return ERROR;
  }
  ```

## 7.2函数参数
  * 设计函数时，优先使用返回值而不是输出参数
  * 函数的参数个数不超过5个
  * `函数的指针参数如果不是用于修改所指向的对象就应该声明为指向const的指针`

  ```
  int strncmp(const char *s1, const char *s2, size_t n); // Good：不变参数声明为const
  ```

# 8.内联函数&宏
## 8.1内联函数
  * 内联函数不超过10行
  * `被多个源文件调用的内联函数要放在头文件中定义`

  ```
  /*
  * 内联函数是在编译时内联展开，因此要求内联函数定义必须在调用此函数的每个源文件内可见。
  *
  * 如下所示代码，inline.h 只有SomeInlineFunc函数的声明而没有定义。othter.c包含inline.h调用SomeInlineFunc时无* 法内联。由于这个限制，多个源文件如果要调用同一个内联函数，需要将内联
  * 函数的定义放在头文件中。
  * gnu89 在内联函数实现上跟C99标准有差异，兼容做法是将函数声明为 static inline。
  *
  */

  //inline.h
  inline int SomeInlineFunc();

  //inline.c
  inline int SomeInlineFunc()
  {
  // 实现代码
  }

  //other.c
  #include "inline.h" 
  int OtherFunc()
  {
      int ret = SomeInlineFunc();
  }
  ```

## 8.2函数式宏
  * 函数式宏是指形如函数的宏(示例代码如下所示)，其包含若干条语句来实现某一特定功能。
  * 尽可能使用函数代替函数式宏
  * 函数式宏不超过10行(非空非注释)
  * 定义宏时，宏参数要使用完备的括号

  ```
  /* 如下所示，是一种错误的写法: */
  #define SUM(a, b) a + b // Bad.
  /* 下面这样调用宏，执行结果跟预期不符：100/SUM(2,8)将扩展成(100/2)+8，预期结果则是100/(2+8) */   

  //这个问题可以通过将整个表示式加上括号来解决，如下所示：
  #define SUM(a, b) (a + b)   // Bad.
  /*
  * 但是这种改法在下面这种场景又有问题：SUM(1 << 2, 8)扩展成1 << (2, 8)（因为 << 优先级低于 + ），
  * 跟预期结果(1 << 2) + 8不符
  */ 

  /* 这个问题可以通过将每个宏参数都加上括号来解决，如下所示：*/
  #define SUM(a, b) (a) + (b) // Bad.
  /* 
  * 再看看第三种问题场景：SUM(2, 8) * 10扩展后的结果为（2)+((8) * 10)跟预期结果(2 + 8) * 10不符。
  * 综上所述，正确的写法如下： 
  */ 
  #define SUM(a, b) ((a) + (b))   // Good.
  包含多条语句的函数式宏的实现语句必须放在 do-while(0) 中
  // Good.
  #define FOO(x) do { \ (void)printf("arg is %d\n", (x)); \ DoSomething((x)); \
  } while (0)
  ```

  * 不允许把带副作用的表达式作为参数传递给函数式宏,如a++等
  * 函数式宏定义中慎用 return、goto、continue、break等改变程序流程的语句

# 9.变量

## 9.1全局变量
  * `不用或少用全局变量。`
  * `模块间，禁止使用全局变量作接口`

## 9.2局部变量
  * `严禁使用未经初始化的变量`
  * `禁止无效、冗余的变量初始化。如果没有确定的初始值则不要进行初始化`
