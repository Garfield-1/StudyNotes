// 顺序循环队列
// 方案1：牺牲一个单元来区分队空和队满
// 队尾指针指向 队尾元素的后一个位置（下一个应该插入的位置）

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MaxSize 50	            //定义队列中元素的最大个数

typedef int ElemType;
typedef struct{
	ElemType data[MaxSize];		//存放队列元素
	int front;		            //队头指针
	int rear;		            //队尾指针
}SeqQueue;

// 初始化队列
void InitQueue(SeqQueue* q)
{
	q->front = q->rear = 0;
}

// 判断队列是否为空
bool QueueEmpty(SeqQueue* q)
{
	if (q->rear == q->front)	//队空条件
		return true;
	else
		return false;
}

// 入队
bool EnQueue(SeqQueue* q, ElemType x)
{
	if ((q->rear+1) % MaxSize == q->front) {
        return false;		                //队列满则报错
    }

	q->data[q->rear] = x;		            //将x插入队尾
	q->rear = (q->rear + 1) % MaxSize;      //队尾指针后移
	return true;
}

// 出队
bool DeQueue(SeqQueue* q, ElemType* x)
{
	if (q->rear == q->front)
		return false;	//队空则报错

	x = q->data[q->front];
	q->front = (q->front + 1) % MaxSize; //队头指针后移
	return true;
}

// 获取队头元素
bool GetHead(SeqQueue* q, ElemType* x)
{
	if (q->rear == q->front)
		return false;	//队空则报错
	
	x = q->data[q->front];
	return true;
}

// 队列中元素的个数
int QueueNum(SeqQueue* q)
{
	return (q->rear - q->front + MaxSize) % MaxSize;
}

int main()
{
    SeqQueue q;
    InitQueue(&q);
    EnQueue(&q, 10);
    int num = QueueNum(&q);
    int num_1;

    printf("%d\n", num);

    GetHead(&q, &num_1);
    printf("%d\n", num_1);
    return;
}