#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ARR_MAX 10

void PrintArr(int arr[], int len);
void SelectionSortUp(int arr[], int len);
void SelectionSortDown(int arr[], int len);

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
    SelectionSortUp(arr, len);
    PrintArr(arr, len);
    return 0;
}

/* 升序排序 */
void SelectionSortUp(int arr[], int len)
{
    int i, j, num;
    for (i = len - 1; i > 0 ; i--) {
        for (j = 0; j < (i - 1); j++) {
            if (arr[j] > arr[i]) {
                num = arr[i];
                arr[i] = arr[j];
                arr[j] = num;
            }
        }
    }
    return;
}

/* 降序排序 */
void SelectionSortDown(int arr[], int len)
{
    int i, j, num;
    for (i = 0; i < len-1 ; i++) {
        for (j = i; j < len; j++) {
            if (arr[j] > arr[i]) {
                num = arr[i];
                arr[i] = arr[j];
                arr[j] = num;
            }
        }
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
