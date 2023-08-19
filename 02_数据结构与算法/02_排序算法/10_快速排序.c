/*
 * 快速排序：采用“分治”的思想，对于一组数据，选择一个基准元素（base），通常选择第一个或最后一个元素，通过第一
 * 轮扫描，比base小的元素都在base左边，比base大的元素都在base右边，再有同样的方法递归排序这两部分，直到序列
 * 中所有数据均有序为止。
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ARR_MAX 10

void PrintArr(int arr[], int len);
void QuickSort(int arr[], int begin, int end);

int main()
{
    int arr[ARR_MAX];
    int len = sizeof(arr) / sizeof(*arr);

    /* 填充随机值 */
    srand(time(NULL));
    for (int i = 0; i < len; i++) {
        arr[i] = rand() % 100;
    }

    printf("原始序列: ");
    PrintArr(arr, len);

    QuickSort(arr, 0, len - 1);

    printf("排序序列: ");
    PrintArr(arr, len);

    return 0;
}

void QuickSort(int arr[], int begin, int end)
{
    if (begin > end) {
        return;
    }

    int tmp = arr[begin];
    int i = begin;
    int j = end;

    /*
     * 优先改变尾部指针的值，保证最后头尾指针重合的位置小于哨兵位的值
     */
    while (i != j) {
        while (arr[j] >= tmp && j > i) {
            j--;
        }   
        while (arr[i] <= tmp && j > i) {
            i++;
        }   
        if (j > i) {
            int t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
        }
    }

    arr[begin] = arr[i];
    arr[i] = tmp;

    /*
     * 此处采用双层递归，一层递归结束进入二层，在二层中在调用一层，数组并不是从中间被一分为二，而是从头尾两个
     * 方向不断缩短
     */
    QuickSort(arr, begin, i-1);
    QuickSort(arr, i+1, end);

    return;
}


void PrintArr(int arr[], int len)
{
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }

    printf("end\n");
    return;
}