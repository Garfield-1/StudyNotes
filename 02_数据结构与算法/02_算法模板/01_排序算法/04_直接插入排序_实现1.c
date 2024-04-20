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
    int i, j, num;
    /* 
     * 1.以i为分界，i之前为有序序列，i之后为无序序列
     * 2.特殊的arr[0]默认属于有序序列
     */
    for (i = 1; i < len; i++) {
        /* 将无序序列的第一个元素取出赋值给num */
        num = arr[i];
        /* 
         * 1.从后向前遍历有序序列,即区间[0,i-1]
         * 2.依次比较有序序列中的值与num的值,将大于num的元素后移一位,即进行升序排序
         * 3.后移元素会覆盖有序序列后的无序序列第一个元素的位置,即arr[i]
         * 4.由于后移操作为简单的复制,而非元素交换,所以后的同时有序序列中会产生一个重复值,即arr[j]
         * 6.循环结束后,此时有序序列中所有大于num的元素全部后移一位,原本num的位置被覆盖,arr[j]的位置为向有序列表中新插入值的位置
         */
        for (j = i-1; j >= 0 && arr[j] > num; j--) {
            arr[j+1] = arr[j];
        }
        /*
         * 1.在此前循环中j--,故此处需要j+1
         * 2.将原本的num值放入,有序序列长度+1,无序序列长度-1
         */
        arr[j+1] = num;
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
