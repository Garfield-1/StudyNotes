/*
 * 归并排序：将原始数组A[0:n-1]中的元素分成两个大小大致相同的子数组：A[0:n/2]和A[n/2+1:n-1]，分别对这两个子数组单独排序
 * 然后将已排序的两个数组归并成一个含有n个元素的有序数组。（不断地进行二分，直至待排序数组中只剩下一个元素为止，然后不断合并两个排好序的数组段）
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ARR_MAX 10

void PrintArr(int arr[], int len);
void MergeSort(int arr[], int left, int right);

int main()
{
    int arr[ARR_MAX], len;
    len = sizeof(arr) / sizeof(*arr);

    /* 填充随机值 */
    srand(time(NULL));
    for (int i = 0; i < len; i++) {
        arr[i] = rand() % 100;
    }

    printf("原始序列: ");
    PrintArr(arr, len);

    MergeSort(arr, 0, len - 1);

    printf("排序序列: ");
    PrintArr(arr, len);

    return 0;
}

//合并操作的代码实现
void Merge(int arr[], int left, int mid, int right)
{
	int *SortedArr;
    SortedArr = (int *)malloc(right - left + 1);
	int First = left;                                 
	int Last = mid + 1;
	int StackTop = 0;

	/*
	 * 将[First, mid]和[mid+1, Last]区间中的有序数组合并存入临时数组SortedArr中
	 */
	while (First <= mid && Last <= right) { 
		if (arr[First] <= arr[Last]) {
			SortedArr[StackTop++] = arr[First++];
		} else {
			SortedArr[StackTop++] = arr[Last++];
		}
	}
	while (First <= mid) {
		SortedArr[StackTop++] = arr[First++];
	}
	while (Last <= right) {
		SortedArr[StackTop++] = arr[Last++];
	}

	/*
	 * 回填arr并销毁临时数组
	 */
	for (First = left, StackTop = 0; First <= right; First++) {
		arr[First] = SortedArr[StackTop++];
	}
	free(SortedArr);
}

//递归形式的归并排序算法
void MergeSort(int arr[], int left, int right)
{
	int mid = 0;

	if (left < right) {
		mid = (left + right) / 2;
		MergeSort(arr, left, mid);
		MergeSort(arr, mid + 1, right);
		Merge(arr, left, mid, right);
	}
}

void PrintArr(int arr[], int len)
{
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }

    printf("end\n");
    return;
}