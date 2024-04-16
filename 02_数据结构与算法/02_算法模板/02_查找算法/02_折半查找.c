/*
 * 折半查找算法也称二分查找算法。
 * 每次将待查找序列一分为二，将目标元素与序列中位数比较，若目标元素小则去前半部分继续执行上述逻辑，反之则在
 * 后半序列查找
 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ARR_MAX 10
#define TARGET 66   //查找目标数字

void PrintArr(int arr[], int len);
void bubbleSort(int arr[], int len);
int binarySearch(int r[],int n,int k);

int main()
{
    int arr[ARR_MAX], len;
    len = sizeof(arr) / sizeof(*arr);

	srand(time(NULL));
	for (int i = 0; i < len; i++) {
		arr[i] = rand() % 100;
	}

    bubbleSort(arr, len);
    printf("原始序列: ");
	PrintArr(arr, len);

    int a = binarySearch(arr, len, TARGET);

    printf("a = %d\n", a);

    return 0;
}

int binarySearch(int r[], int n, int k)
{
    int low = 0, high = n-1 , mid;

	while (low <= high) {
		mid = (low + high)/2;
		if (r[mid] == k) {
            return mid;
        }

		if (r[mid] > k) {
            high = mid - 1;  //左半区间 
        } else {
            low = mid + 1;   //右半区间 
        }
	}
	return -1; 
}

void bubbleSort(int arr[], int len)
{
    int i, j, num;
    for (i = 0; i < len - 1; i++) {
        for (j = 0; j < len - 1 - i; j++) {
            if (arr[j] > arr[j+1]) {
                num = arr[j+1];
                arr[j+1] = arr[j];
                arr[j] = num;
            }
        }
    }
    return;
}

void PrintArr(int arr[], int len)
{
    if (arr == NULL) {
        return;
    }

    for(int i = 0; i < len; i++) {
        printf("%d ",arr[i]);
    }
    printf("\n");
    return;
}
