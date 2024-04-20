/*
 * 基数排序基本思想：将无序序列中的元素每次取出个位的值，然后根据取出的值进行排序，然后在此基础上取十位的值再次排序，直到最高位数结束
 * 由于每次排序是根据某个位数进行排序的，所以可以避免计数排序的一些弊端，可将基数排序看作是计数排序的一种改良
 */

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define ARR_MAX 10

void PrintArr(int arr[], int len);
int ArrMaxBit(int data[], int len);
void RadixSort(int arr[], int len);

int main()
{
    int arr[ARR_MAX], len;
    len = sizeof(arr) / sizeof(*arr);

    /* 填充随机值 */
	srand(time(NULL));
	for(int i = 0; i < len; i++) {
		arr[i] = rand() % 100;
	}

	printf("原始序列: ");
	PrintArr(arr, len);

    RadixSort(arr, len);
    printf("排序序列: ");
    PrintArr(arr, len);

    return 0;
}

void RadixSort(int arr[], int len)
{
    int BucketLen, MaxBit;
    int bucket[10] = {0};
    int *SortedArr;
    /* 新建一个与arr长度相同的数组，动态申请内存 */
    SortedArr = (int *)malloc(sizeof(int *) * len);
    BucketLen = sizeof(bucket) / sizeof(*bucket); 
    MaxBit = ArrMaxBit(arr, len);
    
    int bits = 1;
    int j;
    for(int i = 0; i < MaxBit; i++) {
        memset(bucket, 0, (sizeof(*bucket) * BucketLen));

        /* 记录每一位上，每个数字出现的次数 */
        int BucketNum;
        for(j = 0; j < len; j++) {
            BucketNum = (arr[j] / bits) % 10;
            bucket[BucketNum]++;
        }

        /* 算出每个元素在有序序列中的序号 */
        for(j = 1; j < BucketLen; j++) {
            bucket[j] += bucket[j - 1];
        }

        /* 将无序序列中的元素依照已经计算出的序号放入新数组中，同时减少桶中对应的数量 */
        /* 逆序遍历防止出现大元素在前小元素在后的情况 */
        for(j = len -1 ; j >= 0; j--) {
            BucketNum = (arr[j] / bits) % 10;
            SortedArr[bucket[BucketNum] - 1] = arr[j];
            bucket[BucketNum]--;
	    }

        for(int p = 0; p < len; p++) {
            arr[p] = SortedArr[p];
        }

        bits = bits * 10;
    }
    free(SortedArr);

    return;
}

/* 求数组最大元素有几位 */
int ArrMaxBit(int data[], int len)
{
	int MaxNum;
    MaxNum = data[0];
	for (int i = 1; i < len; i++) {
		if(MaxNum < data[i]) {
            MaxNum = data[i];
        }
	}

	int d=1;
	while (MaxNum >= 10) {
		MaxNum /= 10;
		d++;
	}

	return d;
}

void PrintArr(int arr[], int len)
{
    for(int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }

    printf("end\n");
    return;
}