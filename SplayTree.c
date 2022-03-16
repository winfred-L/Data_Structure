#include "SplayTree.h"

#include <stdio.h>
#include <stdlib.h>

SplayTree CreateEmptyST(void){
    return NULL;
}
SplayTree CreateSTNode(int val, SplayTree f){
    SplayTree T = (SplayTree)malloc(sizeof(struct Splaynode));
    T->value = val;
    T->father = f;
    T->child[0] = T->child[1] = NULL;
    if(f!=NULL){
        //link father node
        if(val<f->value) f->child[0] = T;
        else f->child[1] = T;
    }
    return T;
}
int GetDirection(SplayTree F, SplayTree T){//F must be the father of T
    if(F->child[0]!=NULL && F->child[0]->value==T->value) return 0;
    else return 1;
}
SplayTree Splay(SplayTree T){
    SplayTree father, grand;
    int dir_t, dir_f, dir_g;
    while(1){
        //case 0 : T is root, done
        if(T->father==NULL){
            break;
        }
        //case 1 : T is the child of root, single rotate
        else if(T->father->father==NULL){
            father = T->father;
            dir_t = GetDirection(father,T);
            //change subtree link
            T->father = NULL;
            //change father pointer
            if(T->child[1-dir_t]!=NULL) T->child[1-dir_t]->father = father;
            father->father = T;
            //change child pointer
            father->child[dir_t] = T->child[1-dir_t];
            T->child[1-dir_t] = father;
            break;
        }
        //case 2 : T has grandfather
        else{
            father = T->father;
            grand = father->father;
            dir_t = GetDirection(father,T);
            dir_f = GetDirection(grand,father);
            //case 2.1 : zig-zag
            if(dir_t!=dir_f){
                //change subtree link
                T->father = grand->father;
                if(grand->father!=NULL){
                    dir_g = GetDirection(grand->father,grand);
                    grand->father->child[dir_g] = T;
                }
                //change father pointer
                if(T->child[dir_t]!=NULL) T->child[dir_t]->father = grand;
                if(T->child[1-dir_t]!=NULL) T->child[1-dir_t]->father = father;
                father->father = grand->father = T;
                //change child pointer
                grand->child[1-dir_t] = T->child[dir_t];
                father->child[dir_t] = T->child[1-dir_t];
                T->child[dir_t] = grand;
                T->child[1-dir_t] = father;
            }
            //case 2.2 : zig-zig
            else{
                //change subtree link
                T->father = grand->father;
                if(grand->father!=NULL){
                    dir_g = GetDirection(grand->father,grand);
                    grand->father->child[dir_g] = T;
                }
                //change father pointer
                if(T->child[1-dir_t]!=NULL) T->child[1-dir_t]->father = father;
                if(father->child[1-dir_t]!=NULL) father->child[1-dir_t]->father = grand;
                father->father = T;
                grand->father = father;
                //change child pointer
                grand->child[dir_t] = father->child[1-dir_t];
                father->child[dir_t] = T->child[1-dir_t];
                father->child[1-dir_t] = grand;
                T->child[1-dir_t] = father;
            }
        }
    }
    //return root
    return T;
}
SplayTree InsertST(SplayTree T, int val){
    //step 1 : insert as BST
    if(T==NULL){
        T = CreateSTNode(val,NULL);
    }else{
        SplayTree father;
        int dir;
        while(T!=NULL){
            father = T;
            if(val==T->value){
                printf("%d is already in tree!\n",val);
                break;
            }else if(val<T->value){
                T = T->child[0];
            }else{
                T = T->child[1];
            }
        }
        if(T==NULL){//insert
            T = CreateSTNode(val,father);
        }
    }
    //step 2 : splay new node
    T = Splay(T);
    //return root
    return T;
}


SplayTree DeleteST(SplayTree T, int val){
    //step 1 : find val to be deleted
    SplayTree root = T;
    if(T==NULL){
        printf("delete from empty tree!\n");
        return NULL;
    }
    while(T!=NULL){
        if(val==T->value){
            break;
        }else if(val<T->value){
            T = T->child[0];
        }else{
            T = T->child[1];
        }
    }
    if(T==NULL){
        printf("delete element %d is not found!\n",val);
        return root;
    }
    //step 2 : splay this node
    T = Splay(T);
    //step 3 : delete this node
    if(T->child[0]==NULL && T->child[1]==NULL){//T has no child
        free(T);
        return NULL;
    }else if(T->child[0]!=NULL && T->child[1]!=NULL){//T has two children
        //delete T
        SplayTree left = T->child[0], right = T->child[1];
        free(T);
        left->father = right->father = NULL;
        //find the largest in left-subtree
        while(left->child[1]!=NULL) left = left->child[1];
        //splay that node
        left = Splay(left);
        //attach right-subtree to that node
        right->father = left;
        left->child[1] = right;
        //set T
        T = left;
    }else{//T has one child
        if(T->child[0]!=NULL) T=T->child[0];
        else T=T->child[1];
        free(T->father);
        T->father = NULL;
    }
    return T;
}


void Preorder_Traversal(SplayTree T){
    if(T==NULL) printf("empty tree\n");
    else{
        printf("%d ",T->value);
	    if(T->child[0]!=NULL) Preorder_Traversal(T->child[0]);
	    if(T->child[1]!=NULL) Preorder_Traversal(T->child[1]);
    }
}
void Inorder_Traversal(SplayTree T){
    if(T==NULL) printf("empty tree\n");
    else{
        if(T->child[0]!=NULL) Inorder_Traversal(T->child[0]);
        printf("%d ",T->value);
	    if(T->child[1]!=NULL) Inorder_Traversal(T->child[1]);
    }
}