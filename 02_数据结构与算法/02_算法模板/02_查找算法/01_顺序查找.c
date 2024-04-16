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
void binarySearch(int arrr[], int len, int target);

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

    binarySearch(arr, len, TARGET);

    return 0;
}

void binarySearch(int arr[], int len, int target)
{
    int low = 0, high = len-1 , mid;
    int target_num = 0;

	while (low <= high) {
		mid = (low + high)/2;
		if (arr[mid] == target) {
            target_num++;
            printf("匹配到元素的下标为 %d\n", mid);
        }

		if (arr[mid] > target) {
            high = mid - 1;  //前半区间 
        } else {
            low = mid + 1;   //后半区间 
        }
	}

    if (target_num == 0) {
        printf("查找元素不存在\n");
    } else {
        printf("匹配到元素的个数为 %d\n", target_num);
    }
	
    return; 
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
