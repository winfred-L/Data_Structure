#ifndef AVLTree_H
#define AVLTree_H

typedef struct AVLnode* AVLTree;
struct AVLnode{
	int value;
    int height;//leaf is 0
    AVLTree father;
	AVLTree child[2];
};
//maybe use father pointer can avoid recursion in Insert

AVLTree CreateEmptyAVL(void);
AVLTree CreateAVLNode(int val, int h, AVLTree f);
int Height(AVLTree T);
int FreshHeight(AVLTree T);
AVLTree Findkey(AVLTree T, int val);//same in BST
AVLTree InsertAVL(AVLTree T, int val);
AVLTree DeleteAVL(AVLTree T, int val);
AVLTree SingleRotation(AVLTree T, int dir);
AVLTree DoubleRotation(AVLTree T, int dir);

void Preorder_Traversal(AVLTree T);
void Inorder_Traversal(AVLTree T);

#endif
