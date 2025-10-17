# STL 容器

## 概述

一个容器就是一些特定类型对象的集合。顺序容器(`sequential container`)为程序员提供了控制元素存储和访问顺序的能力。这种顺序不依赖于元素的值，而是与元素加入容器时的位置相对应。与之相对的，有序和无序关联容器，则根据关键字的值来存储元素

## 顺序容器类型

**各个顺序容器的特点如下：**

| 容器           | 特点                                                         |
| -------------- | ------------------------------------------------------------ |
| `vector`       | 支持快速随机访问。在尾部之外的位置插入或删除元素可能很慢     |
| `deque`        | 双端队列。支持快速随机访问。在头尾位置插入/删除速度很快      |
| `list`         | 双向链表。只支持双向顺序访问。在list中任何位置进行插入/删除操作速度都很快 |
| `forward_list` | 单向链表。只支持单向顺序访问。在链表任何位置进行插入/删除操作速度都很快 |
| `array`        | 固定大小数组。支持快速随机访问。不能添加或删除元素           |
| `string`       | 与`vector`相似的容器，但专门用于保存字符。随机访问快。在尾部插入/删除速度快 |

## vector

向量`Vector`以线性排列方式存储给定类型的元素，并允许快速随机访问任何元素，长度可变并且可以支持任意类型的元素。可以简单的认为，`vector`是一个可变大小数组。支持快速随机访问

`vector`有自己的定义规则，同时也包含了一系列函数可用于操作`vector`的内容，可以便捷的对`vector`成员进行增删改查等操作

### 1. vector特性

* 顺序序列
  * 顺序容器中的元素按照严格的线性顺序排序。可以通过元素在序列中的位置访问对应的元素

* 动态数组
  * 支持对序列中的任意元素进行快速直接访问

* 能够感知内存分配器的
  * 容器使用一个内存分配器对象来动态地处理它的存储需求。在使用时会自动申请和释放内存

### 2. vector构造函数

* `vector<T> arr;`
    创建元素类型为`T`的一个空向量`arr`

    ```cpp
    vector<int> arr;  //一个空数组
    ```

* `vector<T> arr(int nSize);`
    创建元素类型为`T`的一个向量`arr`,元素个数为`nSize`

    ```cpp
    vector<int> arr(4);    //开辟4个空间，值默认为0
    ```

* `vector<T> arr(int nSize,const t& t);`
    创建元素类型为`T`的一个向量`arr`，元素个数为`nSize`,且值均为`t`

    ```cpp
    vector<int> arr(5, 3);    //5个值为3的数组
    ```

* `vector<T> arr{int size1, int size2, int size3, int size4};`
    创建元素类型为`T`的一个包含四个元素的向量`arr`

    ```cpp
    vector<int> arr {1, 2, 3, 4, 5}; //包含1、2、3、4、5五个变量
    ```

* `vector<T> arr(const vector&);`
    复制构造函数，创建元素类型为`T`的一个向量`arr`

    ```cpp
    vector<int> arr(arr4);  //将arr4的所有值复制进去，和arr4一样
    ```

* `vector<T> arr(begin,end);`
    创建元素类型为`T`的一个向量`arr`，复制`[begin,end)`区间内另一个数组的元素到`vector`中

    ```cpp
    vector<int> arr(arr4.begin(), arr4.end());        //将arr4的值从头开始到尾复制
    vector<int> arr(arr4.rbegin(), arr4.rend());    //将arr4的值从尾到头复制
    ```

* `vector<vector<T>> arr(x, <vector<T>(y));`
    创建元素类型为`T`的一个向量`arr`，定义二维动态数组`arr`规格为`x`行`y`列

    ```cpp
    vector<vector<int>> arr(5, vector<int>(6));     //定义二维动态数组5行6列
    ```

### 3. 遍历vector

* 迭代器访问

  * 通过迭代器访问从`begin()`到`end()`，需要定义`iterator`，当然可以用`auto`替代

  * `begin()`表示第一个元素，而`end()`不是最后一个元素，`end()`是最后一个元素的前一个位置

    ```cpp
    //迭代器：vector<int>::iterator
    for (vector<int>::iterator it = arr.begin(); it != arr.end(); it++)
    {
        cout << *it << endl;
    }
    //迭代器：vector<int>::reverse_iterator
    for (vector<int>::reverse_iterator it = arr.rbegin(); it != arr.rend(); it++)
    {
        cout << *it << endl;
    }
    ```

* 下标访问
  * 和数组类似，从下标`0`开始遍历

* 范围`for`循环

## string

### 1. string特性

`string`是`c++`中的字符串类型，相当于`C`语言中的`char *`其本质是一个封装好的类

**C++中的string与传统C字符串比较：**

| 操作            | string                         | C字符串              |
| --------------- | ------------------------------ | -------------------- |
| 声明字符串      | `string s;`                    | `char s[100];`       |
| 取得第`i`个字符 | `s[i];`                        | `s[i];`              |
| 字符串长度      | `s.length();`<br />`s.size();` | `strlen(s);`         |
| 读取一行        | `getline(cin, s);`             | `gets(s);`           |
| 设置字符串      | `s="TTTT";`                    | `strcpy(s, "TTTT");` |
| 字符串相加      | `s=s+"TTTT"`                   | `strcat(s, "TTTT");` |
| 字符串比较      | `s=="TTTT";`                   | `strcmp(s, "TTTT");` |

### 2. string的构造函数

* `string s1;` 
    默认构造，创建一个空字符串`s1`

    ```cpp
    string s1;  // 默认构造
    ```

* `string s1(const char* s);`
    使用字符串初始化字符串`s1`

    ```cpp
    string s1("text");  // C字符串构造
    ```

* `string s1(const string& str);`
    拷贝构造，使用一个`string`对象初始化另一个`string`对象`s2`

    ```cpp
    string s1("text");
    string s2(s1);  // 拷贝构造
    ```

* `string s1(int n,char c);`
    使用`n`个字符`c`初始化字符串`s1`

    ```cpp
    string s1(10, 'x');  // 重复字符
    ```

* `string s2(const string& str, x, y);`
    使用字符串的第`x`到第`y`位构造字符串`s2`

    ```cpp
    string s1("text");
    string s2(s1, 1, 3);               // 子串构造
    ```

## deque

### 1. deque特性

`deque`容器为一个给定类型(可以是用户自定义类型)的元素进行线性处理，像向量一样，它能够快速地随机访问任一个元素，并且能够高效地插入和删除容器的尾部元素。但它又与`vector`不同，`deque`支持高效插入和删除容器的头部元素，因此也叫做双端队列

### 2. deque的构造函数

* `deque<T> deq;`
    默认构造函数 - 创建元素类型为`T`的空`deque deq`

    ```cpp
    deque<int> deq
    ```

* `deque<T> deq(n, ele);`
    创建元素类型为`T`的`deque deq`将`n`个`ele`拷贝给本身，

    ```cpp
    deque<int> deq(5);      // 5个默认值(0)
    deque<int> deq(5, 10);  // 5个值为10的元素
    ```

* `deque<T> deq(begin, end);`
    创建元素类型为`T`的`deque deq`将`[begin, end]`区间中的元素拷贝给本身

    ```cpp
    deque<int> source = {1, 2, 3, 4, 5};
    deque<int> deq(source.begin(), source.end()); //从其他容器复制
    ```

* `deque<T> deq(const deque &deq);`
    拷贝构造函数，创建元素类型为`T`的`deque deq`，将`source`的值拷贝过去作为初始值

    ```cpp
    deque<int> source = {1, 2, 3, 4, 5};
    deque<int> deq(source);
    ```

* `deque<T> deq{};`
    创建元素类型为`T`的`deque deq`，初始化列表构造函数

    ```cpp
    deque<int> deq{1, 2, 3, 4, 5}
    ```

## list

### 1. list特性

`list`用于存储元素集合，支持双向迭代器，由双向链表实现而成，元素也存放在堆中，每个元素都是放在一块内存中，他的内存空间可以是不连续的，通过指针来进行数据的访问，这个特点使得它的随机存取变得非常没有效率，因此它没有提供`[]`操作符的重载

**特点**

* **双向迭代**：`<list>` 提供了双向迭代器，可以向前和向后遍历元素
* **动态大小**：与数组不同，`<list>` 的大小可以动态变化，不需要预先分配固定大小的内存
* **快速插入和删除**：可以在列表的任何位置快速插入或删除元素，而不需要像向量那样移动大量元素

### 2. list的构造函数

* `list<T> list1;`
    创建元素类型为`T`的空链表`list1`

    ```cpp
    list<int> list1
    ```

* `list<T> list1(N);`
    创建元素类型为`T`，包含`N`个默认值(`0`)的链表`list1`

    ```cpp
    list<int> list1(5);        // 创建包含5个默认值(0)的链表
    ```

* `list<T> list1(N, X);` 
     创建元素类型为`T`，包含`N`个值为`X`的链表`list1`

    ```cpp
    list<int> list1(5, 100);   // 创建包含5个值为100的链表
    ```

* `list<T> list(begin, end);`
     创建元素类型为`T`的链表`list1`，将数组`arr`的首元素到`arr+3`之间的元素赋值给`list1`

    ```cpp
    int arr[] = {10, 20, 30};
    list<int> list1(arr, arr + 3);  // 从数组构造
    
    vector<int> vec = {1, 2, 3, 4, 5};
    list<int> list1(vec.begin(), vec.end());  // 从迭代器范围构造
    ```

* `list<T> list1(list2);`

     创建元素类型为`T`的链表`list1`，将`list2`的内容拷贝到`list1`中

    ```cpp
    list<int> list6 = {1, 2, 3, 4};
    list<int> list7(list6);  // 拷贝构造
    ```

## forward_list

单向链表。只支持单向顺序访问。在链表任何位置进行插入/删除操作速度都很快

## array

固定大小数组。支持快速随机访问。不能添加或删除元素
