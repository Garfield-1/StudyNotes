# STL 容器

## 标准序列容器

## 1. 容器常用方法

STL提供了一系列封装好的接口和方法，他们大多数都是通用的，但也有例外。这里列举了一部分

### 1.1 迭代器（iterators）

| 名字    | 描述                                                         |
| ------- | ------------------------------------------------------------ |
| begin   | 返回指向容器中第一个元素的迭代器                             |
| end     | 返回指向容器最后一个元素所在位置后一个位置的迭代器           |
| rbegin  | 返回容器逆序的第一个元素的迭代器                             |
| rend    | 返回容器逆序的最后一个元素的前一个位置的迭代器               |
| cbegin  | 和begin()功能相同，在其基础上增加了 const 属性，不能用于修改元素 |
| cend    | 和end()功能相同，在其基础上增加了 const 属性，不能用于修改元素 |
| crbegin | 和rbegin()功能相同，在其基础上增加了 const 属性，不能用于修改元素 |
| crend   | 和rend()功能相同，在其基础上增加了 const 属性，不能用于修改元素 |

### 1.2 容量（Capacity）

| 名字          | 描述                                             |
| ------------- | ------------------------------------------------ |
| size          | 返回实际元素的个数                               |
| capacity      | 返回总共可以容纳的元素个数                       |
| max_size      | 返回元素个数的最大值。这个值非常大，一般是2^32-1 |
| empty         | 判断vector是否为空，为空返回true否则false        |
| resize        | 改变实际元素的个数，对应于size                   |
| reserve       | 增加容器的容量，控制vector的预留空间             |
| shrink_to_fit | 减少capacity到size的大小，即减小到size的大小     |

### 1.3 元素访问（Element access）

| 名字       | 描述                                            |
| ---------- | ----------------------------------------------- |
| operator[] | vector可以和数组一样用[]访问元素                |
| at         | vector.at(i)等同于vector[i]，访问数组下表的元素 |
| front      | 返回第一个元素                                  |
| back       | 返回最后一个元素                                |
| data       | 返回指向容器中第一个元素的**指针**              |

### 1.4 修改器（Modifiers）

| 名字         | 描述                                     |
| ------------ | ---------------------------------------- |
| push_back    | 在容器的尾部插入元素                     |
| pop_back     | 删除最后一个元素                         |
| insert       | 插入元素                                 |
| erase        | 删除元素                                 |
| clear        | 清除容器内容，size=0，存储空间不变       |
| swap         | 交换两个元素的所有内容                   |
| assign       | 用新元素替换原有内容                     |
| emplace      | 插入元素，和insert实现原理不同，速度更快 |
| emplace_back | 在容器的尾部插入元素，和push_back不同    |



## 2. vector

向量（Vector）是一个封装了动态大小数组的顺序容器（Sequence Container）。跟任意其它类型容器一样，它能够存放各种类型的对象。可以简单的认为，vector是一个可变大小数组。支持快速随机访问，在尾部之外的位置插入或删除元素可能很慢

vector有自己的定义规则，同时也包含了一系列函数可用于操作vector的内容，可以便捷的对vector成员进行增删改查等操作

### 2.1 vector特性

* 顺序序列
  * 顺序容器中的元素按照严格的线性顺序排序。可以通过元素在序列中的位置访问对应的元素。

* 动态数组
  * 支持对序列中的任意元素进行快速直接访问

* 能够感知内存分配器的
  * 容器使用一个内存分配器对象来动态地处理它的存储需求。在使用时会自动申请和释放内存

### 2.2 构造函数

- `vector()`:创建一个空vector
- `vector(int nSize)`:创建一个vector,元素个数为nSize
- `vector(int nSize,const t& t)`:创建一个vector，元素个数为nSize,且值均为t
- `vector(const vector&)`:复制构造函数
- `vector(begin,end)`:复制[begin,end)区间内另一个数组的元素到vector中

例如

```cpp
vector<int> arr1;								//一个空数组
vector<int> arr2 {1, 2, 3, 4, 5};				//包含1、2、3、4、5五个变量
vector<int> arr3(4);							//开辟4个空间，值默认为0
vector<int> arr4(5, 3);							//5个值为3的数组
vector<int> arr5(arr4);							//将arr4的所有值复制进去，和arr4一样
vector<int> arr6(arr4.begin(), arr4.end());		//将arr4的值从头开始到尾复制
vector<int> arr7(arr4.rbegin(), arr4.rend());	//将arr4的值从尾到头复制
vector<vector<int> > obj(5, vector<int>(6)); 	//定义二维动态数组5行6列
```

### 2.3 遍历vector

* 迭代器访问

  - 通过迭代器访问从begin()到end()，需要定义iterator，当然可以用auto替代。

  - begin()表示第一个元素，而end()不是最后一个元素，end()是最后一个元素的前一个位置

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
  * 和数组类似，从下标0开始遍历

* 范围for循环



## 3. string

与vector相似的容器，但专门用于保存字符。随机访问快。在尾部插入/删除速度快

## 4. deque

双端队列。支持快速随机访问。在头尾位置插入/删除速度很快

## 5. list

双向链表。只支持双向顺序访问。在list中任何位置进行插入/删除操作速度都很快

## 6. forward_list

单向链表。只支持单向顺序访问。在链表任何位置进行插入/删除操作速度都很快

## 7. array

固定大小数组。支持快速随机访问。不能添加或删除元素

## 标准关联容器

* set
* multiset
* map
* multimap