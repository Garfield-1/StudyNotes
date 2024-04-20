/*
 * 栈的C语言实现和常用操作
 * 双指针实现，base指针指向栈底，top指针指向栈顶。可灵活申请内存
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STACK_INIT_SIZE             3       //初始元素数量
#define STACK_INCR_EMRNT            3       //每次增长的元素数量

typedef int ElementType ;   //定义栈元素类型别名

typedef struct 
{
	ElementType *base;      //早栈构造之前和销毁之后,base的值为NULL
	ElementType *top;       //栈顶指针
	int stacksize;          //当前已分配的存储空间,已元素为单位
}SqStack;

static void initStack(SqStack *S);
static bool checkStackEmpty(const SqStack *S);
static void getTop(SqStack *S, ElementType *e);
static void pushStack(SqStack *S, ElementType e);
static void popStack(SqStack *S);

static void initStack(SqStack *S)
{
    //malloc指向分配内存起始地址
	S->base = (ElementType *)malloc(STACK_INIT_SIZE * sizeof(ElementType)); 
	if (!S->base) {
        printf("malloc error\n");
        return;
    }

	S->top = S->base;
	S->stacksize = STACK_INIT_SIZE;

	return;
}

static bool checkStackEmpty(const SqStack *S)
{
	if (S->top == S->base) {
        printf("The Stack is empty\n");
        return true;
    } else {
        return false;
    }
}

//若栈不为空,则用e返回栈顶元素,并返回OK,否则返回ERROR
static void getTop(SqStack *S, ElementType *e)
{
    if (checkStackEmpty(S)) {
        return;
    }

    *e = (ElementType)(*(S->top - 1));
    return;
}

//插入元素e为新的栈顶元素
static void pushStack(SqStack *S, ElementType e)
{
    if ((int)(S->top - S->base) >= S->stacksize)//判断栈是否为满
    {
        S->base = (ElementType *)realloc(S->base, (S->stacksize + STACK_INCR_EMRNT) * sizeof(ElementType));//栈满,追加内存空间
        if (!S->base) {
            printf("realloc fail\n");
            return; 
        }
        S->top = (ElementType *)(S->base + S->stacksize);
        S->stacksize += STACK_INCR_EMRNT;
    }

    *(S->top) = e;
    S->top = (ElementType *)((S->top) + 1);

    return;
}

//若栈不为空,则删除栈顶元素,用e返回其值,并返回OK;否则返回ERROR
static void popStack(SqStack *S)
{
    if (checkStackEmpty(S)) {
        return;
    }

    int num;

    S->top = (ElementType *)((S->top) - 1);
    num = *(S->top);
    printf("The top of the stack element %d is removed\n", num);

    return;
}

static void printStack(SqStack *S)
{
    int len;
    len = (int)(S->top - S->base);

    for(int i = len - 1; i >= 0; i--) {
        printf("value = %d\n", S->base[i]);
    }

    return;
}

int main() 
{
    SqStack list;
    int topvalue;

    list.base = NULL;
    list.top = NULL;

    initStack(&list);
    pushStack(&list, 1);
    pushStack(&list, 2);
    pushStack(&list, 3);
    pushStack(&list, 4);
    pushStack(&list, 5);
    pushStack(&list, 6);
    printStack(&list);
    
    getTop(&list, &topvalue);
    printf("topvalue = %d\n", topvalue);

    popStack(&list);
    printStack(&list);

    getTop(&list, &topvalue);
    printf("topvalue = %d\n", topvalue);
    
    return 0;
}