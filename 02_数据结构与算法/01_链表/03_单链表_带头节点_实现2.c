/*
 * 带头结点链表，头节点独立于链表，链表结构: Head->p1->p2>p3
 * 为实现这一点，将链表的第一个节点作为头节点，不存放有效数据
 * 此方式实现的单链表，头节点的数据域透明性通过单链表只能单向访问的特性保证
 * 此实现方式仅适用于不循环单链表
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* 是否手动输入相关参数,如不需要手动输入,则注释下面一行 */
#define MANUAL_INPUT

struct LNode
{
    struct LNode *next;
    char data;
};

void PrintNode(struct LNode *L);
void InsertTail(struct LNode *L, char data);
void InsertHead(struct LNode *L, char data);
void ChangeNode(struct LNode *L, char src, char dest);
void DelListNumber(struct LNode *L, int num);
void DelListRecursion(struct LNode *L);
void DelListOrdinal(struct LNode *L);
void InsertNode(struct LNode *L, int num, char data);
#ifdef MANUAL_INPUT
void UserInput();
void DoUserInput(struct LNode *L, int input);
#endif

int main()
{
    struct LNode *Head;

    /* 初始化头节点 */
    Head = (struct LNode *)malloc(sizeof(struct LNode));
    Head->next = NULL;

    #ifdef MANUAL_INPUT
    int input = 0;
    UserInput();
    while(input != -1) {
        scanf("%d",&input);
        while (getchar() != '\n');
        DoUserInput(Head, input);
    }
    #else
    for(int i = 0; i<3 ;i++) {
        InsertTail(Head, 'c');
        InsertHead(Head, 'd');
    }
    PrintNode(Head);
    #endif
    return 0;
}

#ifdef MANUAL_INPUT
/* 输入提示 */
void UserInput()
{    
    printf("#######################################\n");
    printf("-1 退出程序\n");
    printf("0 提示菜单\n");
    printf("1 打印单链表\n");
    printf("2 头插法插入单链表节点\n");
    printf("3 尾插法插入单链表节点\n");
    printf("4 将单链表节点中的'X'全部替换位'Y'\n");
    printf("5 在链表的第X个位置插入'Y'\n");
    printf("6 删除链表的第X个位置的元素\n");
    printf("7 销毁链表\n");
    printf("#######################################\n");
}

/* 执行用户输入 */
void DoUserInput(struct LNode *L, int input)
{
    int num;
    char data, src, dest;
    switch(input) {
    case -1:
        printf("程序结束\n");
        break;
    case 0:
        UserInput();
        break;
    case 1:
        PrintNode(L);
        break;
    case 2:
        printf("在链表的头部插入'X'\n请输入:X\n");
        scanf("%c", &data);
        InsertHead(L, data);
        printf("节点已插入\n");
        break;
    case 3:
        printf("在链表的尾部插入'X'\n请输入:X\n");
        scanf("%c", &data);
        InsertTail(L, data);
        printf("节点已插入\n");
        break;
    case 4:
        printf("将链表中的所有'X'替换为'Y'\n请输入:X Y\n");
        scanf("%c %c", &src, &dest);
        ChangeNode(L, src, dest);
        printf("节点信息已替换\n");
        break;
    case 5:
        printf("在链表的第X个位置插入'Y'\n请输入:X Y\n");
        scanf("%d %c",&num, &data);
        InsertNode(L, num, data);
        printf("节点已插入\n");
        break;
    case 6:
        printf("删除链表的第X个位置的元素\n请输入:X\n");
        scanf("%d", &num);
        DelListNumber(L, num);
        printf("节点已删除\n");
        break;
    case 7:
        DelListOrdinal(L);
        printf("链表已完全删除\n");
        break;
    default:
        printf("输入参数不合法,请重新输入\n");
        break;
    }
}
#endif

/* 输出链表 */
void PrintNode(struct LNode *L)
{
    if(L->next == NULL) {
        printf("当前链表为空\n");
        return;
    }

    /* 跳过头节点 */
    L = L->next;
    while(L != NULL ) {
        printf("%c ",L->data);
        L = L->next;
    }
    printf("\n");
    return;
}

/* 尾插法新增节点 */
void InsertTail(struct LNode *L, char data)
{
    while(L->next != NULL ) {
        L = L->next;
    }

    struct LNode *Node;
    Node = (struct LNode *)malloc(sizeof(struct LNode));
    Node->next = NULL;
    Node->data = data;

    L->next = Node;

    return;
}

/* 头插法新增节点 */
void InsertHead(struct LNode *L, char data)
{
    struct LNode *Node;
    Node = (struct LNode *)malloc(sizeof(struct LNode));
    Node->data = data;

    Node->next = L->next;
    L->next = Node;

    return;
}

/* 在链表第X个位置插入Y */
void InsertNode(struct LNode *L, int num, char data)
{
    struct LNode *Node;
    int counter = 0;

    if (num < 0) {
        printf("非法输入,请输入一个大于零的整数\n");
    }
    
    while(L->next != NULL && counter != num-1) {
        L = L->next;
        counter++;
    }

    Node = (struct LNode *)malloc(sizeof(struct LNode));
    Node->data = data;

    Node->next = L->next;
    L->next = Node;
}

/* 将链表中的src替换为dest */
void ChangeNode(struct LNode *L, char src, char dest)
{
    while(L != NULL) {
        if(L->data == src) {
            L->data = dest;
        }
        L = L->next;
    }
    return;
}

/* 删除链表中的第N个节点 */
void DelListNumber(struct LNode *L, int num)
{
    struct LNode *P;
    int counter = 0;
    while(L->next != NULL && counter != num-1) {
        L = L->next;
        counter++;
    }

    P = L->next;
    L->next = L->next->next;
    free(P);
    P->next = NULL;
}

/* 递归销毁链表 */
void DelListRecursion(struct LNode *L)
{
    if(L == NULL) {
        return;
    }

    DelListRecursion(L->next);
    /* 
     * 1.free不会清理对应内存，需要手动将当前节点的指针域置空，此操作会递归修改整个链表，确保节点之间的联系被切断
     * 2.由于是递归修改，此处的L实际是上一层调用栈中的入参L->next，不能通过一级指针直接修改指针本身，所以L=NULL无意义
     */
    free(L);
    L->next = NULL;
    return;
}

/* 顺序销毁链表 */
void DelListOrdinal(struct LNode *L)
{
    struct LNode *P;
    while(L) {
        P = L;
        L = L->next;
        free(P);
        P->next = NULL;
    }
    return;
}

