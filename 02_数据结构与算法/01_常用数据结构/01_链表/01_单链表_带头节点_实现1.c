/*
 * 带头结点链表，头节点独立于链表，链表结构: Head->p1->p2>p3
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
    char data;
};

void PrintNode(struct LNode *L);
struct LNode *LinkNodeInit(char data);
struct LNode *InsertTail(struct LNode *L, char data);
struct LNode *InsertHead(struct LNode *L, char data);
struct LNode *DelListNumber(struct LNode *L, int num);
struct LNode *FindElement(struct LNode *L, int num);
struct LNode *DelListOrdinal(struct LNode *L);
struct LNode *InsertNode(struct LNode *L, int num, char data);
#ifdef MANUAL_INPUT
void UserInput();
void DoUserInput(struct LNode **L, int input);
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
        DoUserInput(&(Head->next), input);
    }
    #else
    for(int i = 0; i<3 ;i++) {
        Head->next = InsertTail(Head->next, 'a');
        Head->next = InsertHead(Head->next, 'b');
    }
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
    printf("4 在链表的第X个位置插入'Y'\n");
    printf("5 删除链表的第X个位置的元素\n");
    printf("6 销毁链表\n");
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
        printf("在链表的第X个位置插入'Y'\n请输入:X Y\n");
        scanf("%d %c",&num, &data);
        *L = InsertNode(*L, num, data);
        break;
    case 5:
        printf("删除链表的第X个位置的元素\n请输入:X\n");
        scanf("%d", &num);
        *L = DelListNumber(*L, num);
        break;
    case 6:
        *L = DelListOrdinal(*L);
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
    if(L == NULL) {
        printf("当前链表为空\n");
        return;
    }

    while(L != NULL ) {
        printf("%c ",L->data);
        L = L->next;
    }
    printf("\n");
    return;
}

/* 初始化元素 */
struct LNode *LinkNodeInit(char data)
{
    struct LNode *Node;
    Node = (struct LNode *)malloc(sizeof(struct LNode));
    Node->next = NULL;
    Node->data = data;

    return Node;
}

/* 尾插法新增节点 */
struct LNode *InsertTail(struct LNode *L, char data)
{
    struct LNode *Node;
    Node = L;

    if (L == NULL) {
        return LinkNodeInit(data);
    }

    while(Node->next != NULL ) {
        Node = Node->next;
    }
    Node->next = LinkNodeInit(data);

    return L;
}

/* 头插法新增节点 */
struct LNode *InsertHead(struct LNode *L, char data)
{
    struct LNode *Node;

    if (L == NULL) {
        return LinkNodeInit(data);
    }

    Node = LinkNodeInit(data);
    Node->next = L;
    L = Node;

    return L;
}

/* 找到链表中的第num-1个节点 */
struct LNode *FindElement(struct LNode *L, int num)
{
    struct LNode *plist = L;
    int counter = 1;
    
    if (num < 0) {
        printf("非法输入,请输入一个大于零的整数\n");
    }
    if (plist == NULL) {
        printf("链表为空,不存在此节点\n");
        return NULL;
    }

    struct LNode *Node = L;
    int listnum = 0;
    while(Node != NULL ) {
        listnum++;
        Node = Node->next;
    }
    if (num > listnum) {
        printf("链表中不存在此节点\n");
        return NULL;
    }

    /* 对num为1的情况需特殊处理 */
    if (num == 1) {
        return L;
    }

    /* 找到第num-1个节点 */
    while(plist != NULL && counter != num-1) {
        plist = plist->next;
        counter++;
    }

    return plist;
}

/* 删除链表中的第num个节点 */
struct LNode *DelListNumber(struct LNode *L, int num)
{
    struct LNode *plist;
    
    plist = FindElement(L, num);
    if (plist == NULL) {
        return L;
    }

    struct LNode *Node;
    if (num == 1) {
        Node = L;
        L = L->next;
        free(Node);
        Node->next = NULL;
        printf("节点已删除\n");
        return L;
    }

    struct LNode *PNode; 
    PNode = plist->next;
    plist->next = plist->next->next;
    free(PNode);
    PNode->next = NULL;

    printf("节点已删除\n");
    return L;
}

/* 在链表第num个位置插入data */
struct LNode *InsertNode(struct LNode *L, int num, char data)
{
    struct LNode *Node, *plist;

    if (num == 1) {
        return InsertHead(L, data);
    }

    plist = FindElement(L, num);
    if (plist == NULL) {
        return L;
    }

    Node = (struct LNode *)malloc(sizeof(struct LNode));
    Node->data = data;

    Node->next = plist->next;
    plist->next = Node;
    printf("节点已插入\n");
    return L;
}

/* 顺序销毁链表 */
struct LNode *DelListOrdinal(struct LNode *L)
{
    struct LNode *P;
    while(L) {
        P = L;
        L = L->next;
        free(P);
        P->next = NULL;
    }
    return NULL;
}