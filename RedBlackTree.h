#ifndef RedBlackTree_H
#define RedBlackTree_H

#include <stdlib.h>

#ifndef ElementType
#define ElementType int
#endif

enum color_type {black,red};
typedef enum color_type Color;

typedef struct RBTnode* RBTree;
struct RBTnode{
    ElementType value;
    Color color;
    RBTree father;
    RBTree child[2];
};

RBTree CreateEmptyRBT(void);
RBTree CreateRBTNode(ElementType val, Color color,RBTree father);
RBTree Findkey(RBTree T, ElementType val);//same in BST
RBTree InsertRBT(RBTree T, ElementType val);
RBTree DeleteRBT(RBTree T, ElementType val);


void Preorder_Traversal(RBTree T);
void Inorder_Traversal(RBTree T);

#endif ///RedBlackTree_H///