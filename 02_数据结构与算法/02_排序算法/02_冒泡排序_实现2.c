/* 
 * 冒泡排序的核心逻辑
 * 1.遍历数组，每一次都将数组的最大或最小值放到数组最后
 * 2.控制这个循环的最大值，第一次等于len-1，第二次等于len-2，直到等于1
 * 3.构造符合上述条件的一个输入，将每一次的输入作为核心循环的最大值
 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ARR_MAX 10

void bubbleSort(int arr[], int len);
void printArr(int arr[], int len);

int main()
{
    int arr[ARR_MAX], len;
    len = sizeof(arr) / sizeof(*arr);

	srand(time(NULL));
	for (int i = 0; i < len; i++) {
		arr[i] = rand() % 100;
	}

	printf("原始序列: ");
	printArr(arr, len);
    
    printf("排序序列: ");
    bubbleSort(arr, len);
    printArr(arr, len);
    return 0;
}

void bubbleSort(int arr[], int len)
{
    int i, j, num;
    i = len - 1;
    while (i != 0) {
        for (j = 0; j < i; j++) {
            if (arr[j] > arr[j+1]) {
                num = arr[j+1];
                arr[j+1] = arr[j];
                arr[j] = num;
            }
        }
        i--;
    }
    return;
}

void printArr(int arr[], int len)
{
    int i;
    for(i = 0; i < len; i++) {
        printf("%d ",arr[i]);
    }
    printf("\n");
    return;
}



