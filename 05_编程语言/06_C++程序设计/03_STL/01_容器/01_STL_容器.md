# STL 容器

## vector

向量`Vector`以线性排列方式存储给定类型的元素，并允许快速随机访问任何元素，长度可变并且可以支持任意类型的元素。可以简单的认为，`vector`是一个可变大小数组。支持快速随机访问

`vector`有自己的定义规则，同时也包含了一系列函数可用于操作`vector`的内容，可以便捷的对`vector`成员进行增删改查等操作

### 1. vector特性

* 顺序序列
  * 顺序容器中的元素按照严格的线性顺序排序。可以通过元素在序列中的位置访问对应的元素。

* 动态数组
  * 支持对序列中的任意元素进行快速直接访问

* 能够感知内存分配器的
  * 容器使用一个内存分配器对象来动态地处理它的存储需求。在使用时会自动申请和释放内存

### 2. vector构造函数

- `vector<T> arr`

    创建一个空向量`arr`

    ```cpp
    vector<int> arr;  //一个空数组
    ```

- `vector<T> arr(int nSize)`

    创建一个向量`arr`,元素个数为`nSize`

    ```cpp
    vector<int> arr(4);    //开辟4个空间，值默认为0
    ```

- `vector<T> arr(int nSize,const t& t)`

    创建一个向量`arr`，元素个数为`nSize`,且值均为`t`

    ```cpp
    vector<int> arr(5, 3);	//5个值为3的数组
    ```

- `vector<T> arr{int size1, int size2, int size3, int size4}`

    创建一个包含四个元素的向量`arr`

    ```cpp
    vector<int> arr {1, 2, 3, 4, 5}; //包含1、2、3、4、5五个变量
    ```

- `vector<T> arr(const vector&)`

    复制构造函数，构造向量`arr`

    ```cpp
    vector<int> arr(arr4);  //将arr4的所有值复制进去，和arr4一样
    ```

- `vector<T> arr(begin,end)`

    复制`[begin,end)`区间内另一个数组的元素到`vector`中

    ```cpp
    vector<int> arr(arr4.begin(), arr4.end());		//将arr4的值从头开始到尾复制
    vector<int> arr(arr4.rbegin(), arr4.rend());	//将arr4的值从尾到头复制
    ```

- `vector<vector<T>> arr(x, <vector<T>(y))`

    定义二维动态数组`arr`规格为`x`行`y`列

    ```cpp
    vector<vector<int>> arr(5, vector<int>(6)); 	//定义二维动态数组5行6列
    ```

### 3. 遍历vector

* 迭代器访问

  - 通过迭代器访问从`begin()`到`end()`，需要定义`iterator`，当然可以用`auto`替代。

  - `begin()`表示第一个元素，而end()不是最后一个元素，end()是最后一个元素的前一个位置

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

    默认构造函数 - 创建空`deque deq`

    ```cpp
    deque<int> deq
    ```

* `deque<T> deq(n, ele);`

    构造函数将`n`个`ele`拷贝给本身，创建`deque deq`

    ```cpp
    deque<int> deq(5);      // 5个默认值(0)
    deque<int> deq(5, 10);  // 5个值为10的元素
    ```

* `deque<T> deq(begin, end);`

    构造函数将`[begin, end]`区间中的元素拷贝给本身，创建`deque deq`

    ```cpp
    deque<int> source = {1, 2, 3, 4, 5};
    deque<int> deq(source.begin(), source.end()); //从其他容器复制
    ```

* `deque<T> deq(const deque &deq);`

    拷贝构造函数，创建`deque deq`

    ```cpp
    deque<int> deq(dq4);
    ```

* `deque<T> deq{}`

    初始化列表构造函数，创建`deque deq`

    ```cpp
    deque<int> deq{1, 2, 3, 4, 5}
    ```

## list

双向链表。只支持双向顺序访问。在list中任何位置进行插入/删除操作速度都很快

## forward_list

单向链表。只支持单向顺序访问。在链表任何位置进行插入/删除操作速度都很快

## array

固定大小数组。支持快速随机访问。不能添加或删除元素
