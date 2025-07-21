# STL 容器

## 标准序列容器

## vector

向量`（Vector）`是一个封装了动态大小数组的顺序容器`（Sequence Container）`。跟任意其它类型容器一样，它能够存放各种类型的对象。可以简单的认为，`vector`是一个可变大小数组。支持快速随机访问，在尾部之外的位置插入或删除元素可能很慢

`vector`有自己的定义规则，同时也包含了一系列函数可用于操作`vector`的内容，可以便捷的对`vector`成员进行增删改查等操作

### 1. vector特性

* 顺序序列
  * 顺序容器中的元素按照严格的线性顺序排序。可以通过元素在序列中的位置访问对应的元素。

* 动态数组
  * 支持对序列中的任意元素进行快速直接访问

* 能够感知内存分配器的
  * 容器使用一个内存分配器对象来动态地处理它的存储需求。在使用时会自动申请和释放内存

### 2. 构造函数

- `vector()`:创建一个空`vector`
- `vector(int nSize)`:创建一个`vector`,元素个数为`nSize`
- `vector(int nSize,const t& t)`:创建一个`vector`，元素个数为`nSize`,且值均为`t`
- `vector(const vector&)`:复制构造函数
- `vector(begin,end)`:复制[`begin`,`end`)区间内另一个数组的元素到`vector`中

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

与`vector`相似的容器，但专门用于保存字符。随机访问快。在尾部插入/删除速度快

## deque

双端队列。支持快速随机访问。在头尾位置插入/删除速度很快

## list

双向链表。只支持双向顺序访问。在list中任何位置进行插入/删除操作速度都很快

## forward_list

单向链表。只支持单向顺序访问。在链表任何位置进行插入/删除操作速度都很快

## array

固定大小数组。支持快速随机访问。不能添加或删除元素

## 标准关联容器

* set
* multiset
* map
* multimap