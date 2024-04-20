/* 1.直接插入排序的基本思想是,首先将整个数组分为无序序列和有序序列两部分
 * 2.然后将无序序列中的数据插入到有序的序列中合适的位置
 * 3.在遍历无序序列时，首先拿无序序列中的首元素去与有序序列中的每一个元素比较并插入到合适的位置,一直到无序序列中的所有元素插完为止
 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ARR_MAX 10

void PrintArr(int arr[], int len);
void InsertionSort(int arr[], int len);

int main()
{
    int arr[ARR_MAX], len;
    len = sizeof(arr) / sizeof(*arr);

	srand(time(NULL));
	for (int i = 0; i < len; i++) {
		arr[i] = rand() % 100;
	}

	printf("原始序列: ");
	PrintArr(arr, len);
    
    printf("排序序列: ");
    InsertionSort(arr, len);
    PrintArr(arr, len);
    return 0;
}

void InsertionSort(int arr[], int len)
{
    int i, j, n, num;
    for (i = 1; i < len; i++) {
        num = arr[i];
        j = i-1;
        n = j;
        while(j >= 0) {
            if(arr[n] > num) {
                arr[n+1] = arr[n];
                n--;
            }
            j--;
        }
        arr[n+1] = num;
    }
    return;
}

void PrintArr(int arr[], int len)
{
    int i;
    for(i = 0; i < len; i++) {
        printf("%d ",arr[i]);
    }
    printf("\n");
    return;
}
