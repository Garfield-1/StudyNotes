# STL 标准模板库

> 参考文档
>
> 《Effective STL中文版 50条有效使用STL的经验》		--清华大学出版社
>
> 《C++ Primer 第五版 中文版》						--电子工业出版社

`STL`是`C++`标准库的一个重要组成部分，它提供了一系列通用的模板类和函数，用于实现常见的数据结构和算法。`STL`包含了许多常用的容器、迭代器、算法和函数对象等，为`C++`提供了强大而高效的工具，用于解决各种问题。

`STL`主要由以下几个组件组成：

1. **容器（Containers）**
    - 作用：负责存储和组织一组数据元素。
    - 分类：
        - 顺序容器：`vector`、`list`、`deque`、`array`、`forward_list`
        - 关联容器：`set`、`multiset`、`map`、`multimap`
        - 无序关联容器：`unordered_set`、`unordered_map`、`unordered_multiset`、`unordered_multimap`
        - 容器适配器：`stack`、`queue`、`priority_queue`
2. **迭代器（Iterators）**
    - 作用：为容器提供一种统一的遍历接口，解耦容器与算法。
    - 分类：
        - 输入迭代器（`input_iterator`）
        - 输出迭代器（`output_iterator`）
        - 前向迭代器（`forward_iterator`）
        - 双向迭代器（`bidirectional_iterator`）
        - 随机访问迭代器（`random_access_iterator`）
3. **算法（Algorithms）**
    - 作用：对容器中的元素执行各种操作，如查找、排序、变换、拷贝、合并、统计等。
    - 特点：与容器和迭代器无关，只要满足迭代器要求，就能复用同一套算法。
    - 典型算法：`sort`、`find`、`for_each`、`transform`、`accumulate`、`binary_search`、`merge`
4. **函数对象（Function Objects / Functors）**
    - 作用：像函数一样使用的对象，可传递给算法以定制行为。
    - 优点：可保存状态、支持内联展开，比普通函数指针更高效。
    - 标准提供：`std::plus`、`std::greater`、`std::negate`、`std::logical_and`等
5. **适配器（Adapters）**
    - 作用：在容器、迭代器或函数对象之上提供额外接口或行为变换。
    - 类型：
        - 容器适配器：`stack`、`queue`、`priority_queue`
        - 迭代器适配器：`reverse_iterator`、`insert_iterator`、`istream_iterator`、`ostream_iterator`
        - 函数适配器：`std::bind`、`std::function`、`std::mem_fn`、`std::not_fn`
6. **分配器（Allocators）**
    - 作用：为容器提供内存分配和释放策略的定制点，默认使用 `std::allocator`
    - 应用：特殊场景下可替换为自定义分配器（如内存池、共享内存等）

## STL类型

### 1. 容器

* `vector`、`string`、`deque`和`list`被称为标准序列容器
* 标准关联容器是`set`、`muliset`、`map`和`multimap`

### 2. 迭代器

**为容器提供统一的访问接口**，根据迭代器所支持的操作，可以把迭代器分为五类。简单来说，

* 输入迭代器(`inputiterator`)是只读迭代器，在每个被遍历到的位置上只能被读取一次
* 输出迭代器(`output iterator`)是只写迭代器，在每个被遍历到的位置上只能被写入一次
  * 输入和输出迭代器的模型分别是建立在针对输入和输出流(例如文件)的读写操作的基础上的。所以不难理解，输入和输出迭代器最常见的表现形式是`istream_iterator`和`ostream_iterator`
* 前向迭代器(`forward iterator`)兼具输入和输出迭代器的能力，但是它可以对同一个位置重复进行读和写。前向迭代器不支持`operator--`,所以它只能向前移动。所有的标准`STL`容器都支持比前向迭代器功能更强大的迭代器
* 双向迭代器(`bidirectional iterator`)很像前向迭代器，只是它们向后移动和向前移动同样容易。标准关联容器都提供了双向迭代器。`list`也是如此。
* 随机访问迭代器(`random access iterator`)有双向迭代器的所有功能，而且，它还提供了“迭代器算术”,即在一步内向前或向后跳跃的能力。`vector`、`string`和`deque`都提供了随机访问迭代器。指向数组内部的指针对于数组来说也是随机访问迭代器。

### 3. 函数对象

所有重载了函数调用操作符(即`operator()`)的类都是一个函数子类(`functor class`)。从这些类创建的对象被称为函数对象(`function object`)或函数子(`functor`)。

在`STL`中，大多数使用函数对象的地方同样也可以使用实际的函数，所以我经常使用“函数对象”(`function object`)这个术语既表示`C++`函数，也表示真正的函数对象。函数`bind1st`和`bind2nd`被称为绑定器(`binder`)。

### 4. 算法

`sort`、`find`、`for_each`、`accumulate`、`binary_search` 等与容器无关的通用操作,包括：

- 非修改序列操作
    - 查找／计数：`find`、`count`、`find_if`、`count_if`
    - 遍历：`for_each`
    - 比较：`equal`、`mismatch`、`lexicographical_compare`
    - 查询：`all_of`、`any_of`、`none_of`
- 修改序列操作
    - 复制／移动：`copy`、`move`、`swap_ranges`
    - 替换：`replace`、`replace_if`
    - 填充：`fill`、`fill_n`
    - 删除：`remove`、`remove_if`、`erase–remove`
- 排序与相关操作
    - 排序：`sort`、`stable_sort`、`partial_sort`、`nth_element`
    - 二分查找：`binary_search`、`lower_bound`、`upper_bound`、`equal_range`
- 分区
    - `partition`、`stable_partition`、`partition_point`
- 堆算法
    - 构建／调整：`make_heap`、`push_heap`、`pop_heap`
    - 检查／排序：`is_heap`、`is_heap_until`、`sort_heap`
- 集合算法（要求有序区间）
    - 合并：`merge`
    - 并／交／差：`set_union`、`set_intersection`、`set_difference`、`set_symmetric_difference`
- 排列组合
    - 下／下一个排列：`next_permutation`、`prev_permutation`
- 数值算法
    - 累加：`accumulate`
    - 内积：`inner_product`
    - 差分／前缀和：`adjacent_difference`、`partial_sum`

这些算法都通过统一的迭代器接口工作，与容器解耦，极大提高了代码的复用性和可读性。
