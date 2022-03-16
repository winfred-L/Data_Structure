#ifndef SplayTree_H
#define SplayTree_H

typedef struct Splaynode* SplayTree;
struct Splaynode{
	int value;
    SplayTree father;
	SplayTree child[2];
};

SplayTree CreateEmptyST(void);
SplayTree CreateSTNode(int val, SplayTree f);
int GetDirection(SplayTree F, SplayTree T);
SplayTree Findkey(SplayTree T, int val);//same in BST
SplayTree Splay(SplayTree T);
SplayTree InsertST(SplayTree T, int val);
SplayTree DeleteST(SplayTree T, int val);

void Preorder_Traversal(SplayTree T);
void Inorder_Traversal(SplayTree T);


#endif
