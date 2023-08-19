/*
 * 带头结点循环链表结构: Head->p1->p2>p3->p1->p2->p3.... 头节点与链表分离
 * 为实现这一点，头节点单独开辟了一块内存空间，仅指针域可以操作数据域保留不存放有效数据
 * 头的数据域的透明性通过链表结构和接口的设计保证
 * 提供的所有接口仅可操作头的指针域，无法操作头的数据域，如此来实现头节点与链表分离
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* 是否手动输入相关参数,如不需要手动输入,则注释下面一行 */
#define MANUAL_INPUT

struct LNode
{
    struct LNode *next;
    struct LNode *last;
    char data;
};

void PrintNode(struct LNode *L);
struct LNode *FindElement(struct LNode *L, int num);
struct LNode *LinkListInit(char data);
struct LNode *InsertHead(struct LNode *L, char data);
struct LNode *InsertTail(struct LNode *L, char data);
struct LNode *DelListNumber(struct LNode *L, int num);
#ifdef MANUAL_INPUT
void UserInput();
void DoUserInput(struct LNode **L, int input);
#endif
struct LNode *DelListOrdinal(struct LNode *L);

int main()
{
    struct LNode *Head = NULL;
    /* 链表头节点 */
    Head = (struct LNode *)malloc(sizeof(struct LNode));
    memset(Head, 0, sizeof(struct LNode));

    #ifdef MANUAL_INPUT
    int input = 0;
    UserInput();
    while(input != -1) {
        scanf("%d",&input);
        while (getchar() != '\n');
        DoUserInput(&(Head->next), input);
    }
    #else
    Head->next = InsertHead(Head->next, 'a');
    PrintNode(Head->next);
    Head->next = DelListNumber(Head->next, 3);
    PrintNode(Head->next);
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
    printf("4 删除链表的第X个位置的元素\n");
    printf("5 销毁链表\n");
    printf("#######################################\n");
}

/* 执行用户输入 */
void DoUserInput(struct LNode **L, int input)
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
        PrintNode(*L);
        break;
    case 2:
        printf("在链表的头部插入'X'\n请输入:X\n");
        scanf("%c", &data);
        *L = InsertHead(*L, data);
        printf("节点已插入\n");
        break;
    case 3:
        printf("在链表的尾部插入'X'\n请输入:X\n");
        scanf("%c", &data);
        *L = InsertTail(*L, data);
        printf("节点已插入\n");
        break;
    case 4:
        printf("删除链表的第X个位置的元素\n请输入:X\n");
        scanf("%d", &num);
        *L = DelListNumber(*L, num);
        break;
    case 5:
        *L = DelListOrdinal(*L);
        printf("链表已完全删除\n");
        break;
    default:
        printf("输入参数不合法,请重新输入\n");
        break;
    }
}
#endif

/* 初始化链表首个元素 */
struct LNode *LinkListInit(char data)
{
    struct LNode *Node = NULL;
    Node = (struct LNode *)malloc(sizeof(struct LNode));
    Node->next = Node;
    Node->last = Node;
    Node->data = data;

    return Node;
}

/* 输出链表 */
void PrintNode(struct LNode *L)
{
    if (L == NULL) {
        printf("链表为空\n");
        return;
    }

    struct LNode *plist;
    plist = L;
    do {
        printf("%c ",plist->data);
        plist = plist->next;
    }
    while(plist != L);
    printf("\n");
    return;
}

/* 尾插插入新节点 */
struct LNode *InsertTail(struct LNode *L, char data)
{
    if (L == NULL) {
        return LinkListInit(data);
    }

    struct LNode *plist, *Node;
    plist = L;
    /* 找到链表最后一个节点 */
    while(plist->next != L) {
        plist = plist->next;
    }

    Node = (struct LNode *)malloc(sizeof(struct LNode));
    Node->data = data;

    Node->last = plist;
    Node->next = plist->next;
    plist->next->last = Node;
    plist->next = Node;
    
    return L;
}

/* 头插法新增节点 */
struct LNode *InsertHead(struct LNode *L, char data)
{
    if (L == NULL) {
        return LinkListInit(data);
    }

    struct LNode *Node;
    Node = (struct LNode *)malloc(sizeof(struct LNode));
    Node->data = data;

    L->last->next = Node;
    Node->last = L->last;
    L->last = Node;
    Node->next = L;

    L = Node;
    
    return L;
}

/* 删除链表中的第N个节点 */
struct LNode *DelListNumber(struct LNode *L, int num)
{
    struct LNode *plist;

    plist = FindElement(L, num);
    if (plist == NULL) {
        return L;
    }

    /* 当链表中只有一个元素时，返回NULL */
    if (plist->next == plist->next->next && 
        plist->last == plist->last->last) {
        free(plist);
        plist = NULL;
        printf("节点已删除\n");
        return plist;
    }

    /* 当删除第一个元素时，头节点指向的元素需要改变 */
    if (num == 1) {
        L = L->next;
    }

    plist->last->next = plist->next;
    plist->next->last = plist->last;
    
    free(plist);
    plist->last = NULL;
    plist->next = NULL;

    printf("节点已删除\n");
    return L;
}

/* 找到链表中的第N个节点 */
struct LNode *FindElement(struct LNode *L, int num)
{
    struct LNode *plist;
    plist = L;
    int counter = 1;

    if (plist == NULL) {
        printf("链表为空,不存在此节点\n");
        return NULL;
    }

    /* 找到第N个节点 */
    while(plist->next != L && counter != num) {
        plist = plist->next;
        counter++;
    }
    if (counter < num) {
        printf("链表中不存在此节点\n");
        return NULL;
    }
    return plist;
}

/* 顺序销毁链表 */
struct LNode *DelListOrdinal(struct LNode *L)
{
    struct LNode *plist;
    plist = L;
    while(plist) {
        plist = DelListNumber(plist, 1);
    }
    return plist;
}