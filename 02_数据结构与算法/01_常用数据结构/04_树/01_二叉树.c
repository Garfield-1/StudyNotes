/*
 * 二叉树:链表实现
 */

#include <stdio.h>
#include <stdlib.h>

struct BTNode{
    char data;
    struct BTNode *lChild;
    struct BTNode *rChild;
};

// 构造空二叉树T
void InitBiTree(struct BTNode *T)
{
	T = NULL;
}

int CreateBiTree(struct BTNode *T)
{
	char ch;
	scanf("%c", &ch);
    while (getchar() != '\n');
	if (ch == '#') {
        T = NULL;
        return 0;
    } else {
		T = (struct BTNode *)malloc(sizeof(struct BTNode));
		if (!T) {
            return 0;
        }

		T->data = ch;
        printf("请输入左子树节点\n");
		CreateBiTree(T->lChild);
        printf("请输入右子树节点\n");
		CreateBiTree(T->rChild);
	}
    return 0;
}

int main()
{
    struct BTNode T;
    InitBiTree(&T);

    printf("请输入根节点\n");
    CreateBiTree(&T);
    
    return 0;
}