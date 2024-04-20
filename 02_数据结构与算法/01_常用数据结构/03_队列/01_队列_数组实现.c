/*
 * 顺序循环队列:数组实现
 * 牺牲一个单元来区分队空和队满
 * 队头指针指向队头，队尾指针指向 队尾元素的后一个位置（下一个应该插入的位置）
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h> // 包含整数类型的限制常量

#define INVALID_INT INT_MAX + 1 // 无效整数值

#define MaxSize 5	            //定义队列中元素的最大个数

typedef int ElemType;
typedef struct{
	ElemType data[MaxSize];		//存放队列元素
	int front;		            //队头指针,用作数组下标
	int rear;		            //队尾指针,用作数组下标
}SeqQueue;

// 队列中元素的个数
int QueueNum(SeqQueue *q)
{
    int num;

    if (q->rear == q->front) {
        num = 0;
    } else if (q->rear > q->front) {
        num = q->rear - q->front;
    } else if (q->rear < q->front) {
        num = q->rear + MaxSize - q->front;
    }

    return num;
}

// 初始化队列
void InitQueue(SeqQueue *q)
{
    q->front = 0;
    q->rear = 0;

    return;
}

// 判断队列是否为空
bool QueueEmpty(const SeqQueue *q)
{
    if (q->rear == q->front) {
        printf ("The Queue is Empty\n");
        return true;
    } else {
        return false;
    }
}

// 检查队列是否为满
bool checkQueueFull(const SeqQueue *q)
{
    if ((q->rear + 1) % MaxSize == q->front) {
        printf ("The Queue is full\n");
        return true;
    } else {
        return false;
    }
}

// 入队
bool EnQueue(SeqQueue *q, ElemType x)
{
    if (checkQueueFull(q)) {
        return false;
    }

    q->data[q->rear] = x;		            //将x插入队尾
    q->rear = (q->rear + 1) % MaxSize;      //队尾指针后移

    return true;
}

// 获取队头元素
bool GetHead(SeqQueue *q, ElemType *x)
{
    if (QueueEmpty(q)) {
        return false;
    }

    *x = q->data[q->front];

    return true;
}

void printQueue(const SeqQueue *q)
{
    int front, rear;

    if (QueueEmpty(q)) {
        return;
    }

    front = q->front;
    rear = q->rear;

    while (front != rear) {
        printf("Queuenum = %d\n", q->data[front]);
        front = (front + 1) % MaxSize;
    }

    return;
}

// 出队
bool DeQueue(SeqQueue *q, ElemType* x)
{
    if (QueueEmpty(q)) {
        return false;
    }

    x = q->data[q->front];
    q->data[q->front] = INVALID_INT;
    q->front = (q->front + 1) % MaxSize; //队头指针后移

    return true;
}

int main()
{
    bool ret = true;
    int num;

    SeqQueue q;
    InitQueue(&q);

    ret &= EnQueue(&q, 1);
    ret &= EnQueue(&q, 2);
    ret &= EnQueue(&q, 3);

    if (ret) {
        printQueue(&q);
    }

    GetHead(&q, &num);
    printf("\nHeadnum = %d\n", num);

    DeQueue(&q, &num);
    printf("\ndelnum = %d\n", num);

    EnQueue(&q, 1);
    EnQueue(&q, 2);
    EnQueue(&q, 3);

    printQueue(&q);

    return;
}