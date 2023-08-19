/*
 * 计数排序基本思想：将无序序列转换成一个一维线性数组例如无序序列元素最大100
 * 1.无序序列最大元素为100则开辟一个100长度的数组，将对应下标元素的值+1对应无序序列元素对应有序序列中对应的位置
 * 2.反向填充，得到有序序列数组
 *
 * 缺陷：空间换时间的思路，存在大量空间浪费，负数元素需偏移后进行处理
 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ARR_LEN 10
#define ARR_MAX 100

void PrintArr(int arr[], int len);
void CountingSort(int *IniArr, int *SortedArr, int n);

int main()
{
	int arr[ARR_LEN], SortedArr[ARR_LEN], len;
    len = sizeof(arr) / sizeof(*arr);

	/*填充随机值*/
	srand(time(NULL));
	for (int i = 0; i < len; i++) {
		arr[i] = rand() % 100;
	}

	printf("原始序列: ");
	PrintArr(arr, len);
    CountingSort(arr, SortedArr, len);

	printf("排序序列: ");
	PrintArr(SortedArr, len);
	return 0;
}

void CountingSort(int IniArr[], int *SortedArr, int n) 
{
	int count_arr[ARR_MAX] = {0};
	int num;

	for (int i = 0; i < n; i++) {
		count_arr[IniArr[i]]++;
	}

	for (int k = 1; k < ARR_MAX; k++) {
		count_arr[k] += count_arr[k - 1];
	}

	for (int j = n -1; j > 0; j--){
		num = count_arr[IniArr[j]];
		SortedArr[--num] = IniArr[j];
	}

	return;
}

void PrintArr(int arr[], int len)
{
    int i;
    for(i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return;
}