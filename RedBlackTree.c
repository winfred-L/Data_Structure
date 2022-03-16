#include "RedBlackTree.h"

#include <stdio.h>

#ifndef ElementType
#define ElementType int
#endif

RBTree CreateEmptyRBT(void){
    return NULL;
}

RBTree CreateRBTNode(ElementType val, Color color,RBTree father){
    RBTree T = (RBTree)malloc(sizeof(struct RBTnode));
    T->value = val;
    T->color = color;
    T->father = father;
    T->child[0] = T->child[1] = NULL;
    return T;
}

RBTree InsertRBT(RBTree T, ElementType val){
    //*insert value*//
    RBTree former; //record the father node
    int dir; // record the direction of child
    if(T==NULL){
        //if T is empty, create a black root
        T = CreateRBTNode(val,black,NULL);
        goto RET;
    }else{
        //find the position to insert
        while(T!=NULL){
            former = T;
            if(val==T->value){
                printf("%d is already in this tree!\n",val);
                goto RET;
            }
            else if(val<T->value){
                T = T->child[0];
                dir = 0;
            }
            else{
                T = T->child[1];
                dir = 1;
            }
        }
        //insert into tree
        if(T==NULL){
            T = CreateRBTNode(val,red,former);
            former->child[dir] = T;
        }
    }
    //now T is the newly inserted node
    
    //*rebalance*//
    RBTree father, uncle, grand;
    int dir_f, dir_t;
    while(1){// T is always the red node in processing
        //case 0 : T is root
        if(T->father==NULL){
            T->color = black;
            break;
        }
        //check balance
        if(T->father->color==black) break;
        //preparation for later processing
        father = T->father;
        grand = father->father;//can't be NULL, or father won't be red
        if(grand->child[0]!=NULL && father->value==grand->child[0]->value) dir_f=0;
        else dir_f=1;
        uncle = grand->child[1-dir_f];
        if(father->child[0]!=NULL && T->value==father->child[0]->value) dir_t=0;
        else dir_t=1;
        //case 1 : uncle of T is black or NIL
        if(uncle==NULL||uncle->color==black){
            if(dir_f==dir_t){//case 1.1 : T is left child
                //change subtree link
                father->father = grand->father;
                if(grand->father!=NULL){
                    if(grand->father->child[0]!=NULL &&
                        grand->father->child[0]->value==grand->value)
                            grand->father->child[0] = father;
                    else if(grand->father->child[1]!=NULL &&
                        grand->father->child[1]->value==grand->value)
                            grand->father->child[1] = father;
                }
                //change father pointer
                grand->father = father;
                if(father->child[1-dir_t]!=NULL) father->child[1-dir_t]->father=grand;
                //change child pointer
                grand->child[dir_f] = father->child[1-dir_t];
                father->child[1-dir_t] = grand;
                //change color
                father->color = black;
                grand->color = red;
                //done
                break;
            }else{//case 1.2 : T is right child
                //change subtree link
                T->father = grand->father;
                if(grand->father!=NULL){
                    if(grand->father->child[0]!=NULL &&
                        grand->father->child[0]->value==grand->value)
                            grand->father->child[0] = T;
                    else if(grand->father->child[1]!=NULL &&
                        grand->father->child[1]->value==grand->value)
                            grand->father->child[1] = T;
                }
                //change father pointer
                if(T->child[dir_t]!=NULL) T->child[dir_t]->father=grand;
                if(T->child[1-dir_t]!=NULL) T->child[1-dir_t]->father=father;
                father->father = grand->father = T;
                //change child pointer
                father->child[dir_t] = T->child[1-dir_t];
                grand->child[dir_f] = T->child[dir_t];
                T->child[dir_t] = grand;
                T->child[1-dir_t] = father;
                //change color
                grand->color = red;
                T->color = black;
                //done
                break;
            }
        }
        //case 2 : uncle of T is red
        else{
            //push the father's black to two children
            grand->color = red;
            father->color = uncle->color = black;
            T = grand;
        }
    }

    //*return the root*//
    RET:
    while(T->father!=NULL) T=T->father;
    return T;
}


RBTree DeleteRBT(RBTree T, ElementType val){
    //*find the node to be deleted*//
    RBTree root = T;
    if(T==NULL){// T is empty
        printf("delete from an empty tree!\n");
        return NULL;
    }else{
        //find the node position
        while(T!=NULL){
            if(val==T->value){
                break;
            }
            else if(val<T->value){
                T = T->child[0];
            }
            else{
                T = T->child[1];
            }
        }
        if(T==NULL){//not find
            printf("%d is not in this tree!\n",val);
            return root;
        }
    }
    //now T is the node to be deleted

    //*check children number*//
    //if T has two children, move it to make it one or no child
    if(T->child[0]!=NULL && T->child[1]!=NULL){
        //get the smallest in the right subtree
        RBTree X = T->child[1];
        while(X->child[0]!=NULL) X=X->child[0];
        //exchange T and X value
        ElementType tem = T->value;
        T->value = X->value;
        X->value = tem;
        //set T to new position
        T = X;
    }

    //*delete and rebalance*//
    RBTree father,sibling;
    int dir;
    //case 0 : T is root
    if(T->father==NULL){
        //only two situations
        //case 0.1 : T has no child
        if(T->child[0]==NULL && T->child[1]==NULL){
            free(T);
            return NULL;
        }
        //case 0.2 : T has a red child, this red node must be leaf
        else{
            T = (T->child[0]!=NULL)? T->child[0] : T->child[1];
            free(T->father);
            T->father = NULL;
            return T;
        }
    }
    //preparation for later processing
    father = T->father;
    if(father->child[0]!=NULL && father->child[0]->value==T->value) dir=0;
    else dir=1;
    //case 1 : T is red
    if(T->color==red){
        //this node must be leaf
        father->child[dir] = NULL;
        free(T);
        T = father;
    }
    //case 2 : T is black and has no child (T becomes double black)
    else if(T->child[0]==NULL && T->child[1]==NULL){
        while(1){
            //preparation for later processing
            father = T->father;
            if(father->child[0]!=NULL && father->child[0]->value==T->value) dir=0;
            else dir=1;
            sibling = father->child[1-dir];
            //case 2.1 : sibling of T is black (can't be NIL)
            if(sibling->color==black){
                //case 2.1.1 : the sibling's children are both black
                if((sibling->child[0]==NULL||sibling->child[0]->color==black)&&
                   (sibling->child[1]==NULL||sibling->child[1]->color==black)){
                    if(T->child[0]==NULL && T->child[1]==NULL){//T is leaf
                        //delete T
                        father->child[dir] = NULL;
                        free(T);
                    }//if T is not leaf, double black becomes black
                    //change sibling's color
                    sibling->color = red;
                    //check done
                    if(father->color==red){//if father is red, done
                        father->color=black;
                        T = father;
                        break;
                    }else if(father->father==NULL){//if father is root(black), done
                        T = father;
                        break;
                    }else{//if father is black but not root
                        T = father;//double black propagate upward
                        //next processing loop
                    }
                }
                //case 2.1.2 : the sibling's right child is red
                else if(sibling->child[1-dir]!=NULL && sibling->child[1-dir]->color==red){
                    //change subtree link
                    sibling->father = father->father;
                    if(father->father!=NULL){
                        if(father->father->child[0]!=NULL &&
                            father->father->child[0]->value==father->value)
                                father->father->child[0] = sibling;
                        else if(father->father->child[1]!=NULL &&
                            father->father->child[1]->value==father->value)
                                father->father->child[1] = sibling;
                    }
                    //change father pointer
                    if(sibling->child[dir]!=NULL) sibling->child[dir]->father = father;
                    father->father = sibling;
                    //change child pointer
                    father->child[1-dir] = sibling->child[dir];
                    sibling->child[dir] = father;
                    //change color
                    sibling->color = father->color;
                    sibling->child[1-dir]->color = black;
                    father->color = black;
                    //if double black is leaf, delete it
                    if(T->child[0]==NULL && T->child[1]==NULL){
                        father->child[dir] = NULL;
                        free(T);
                        T = father;
                    }
                    //done
                    break;
                }
                //case 2.1.3 : the sibling's left child is red
                else{
                    RBTree nephew = sibling->child[dir];
                    //change subtree link
                    nephew->father = father->father;
                    if(father->father!=NULL){
                        if(father->father->child[0]!=NULL &&
                            father->father->child[0]->value==father->value)
                                father->father->child[0] = nephew;
                        else if(father->father->child[1]!=NULL &&
                            father->father->child[1]->value==father->value)
                                father->father->child[1] = nephew;
                    }
                    //change father pointer
                    if(nephew->child[dir]!=NULL) nephew->child[dir]->father = father;
                    if(nephew->child[1-dir]!=NULL) nephew->child[1-dir]->father = sibling;
                    father->father = nephew;
                    sibling->father = nephew;
                    //change child pointer
                    father->child[1-dir] = nephew->child[dir];
                    sibling->child[dir] = nephew->child[1-dir];
                    nephew->child[dir] = father;
                    nephew->child[1-dir] = sibling;
                    //change color
                    nephew->color = father->color;
                    father->color = black;
                    //if double black is leaf, delete it
                    if(T->child[0]==NULL && T->child[1]==NULL){
                        father->child[dir] = NULL;
                        free(T);
                        T = father;
                    }
                    //done
                    break;
                }
            }
            //case 2.2 : sibling of T is red
            else{
                //change subtree link
                sibling->father = father->father;
                if(father->father!=NULL){
                    if(father->father->child[0]!=NULL &&
                        father->father->child[0]->value==father->value)
                            father->father->child[0] = sibling;
                    else if(father->father->child[1]!=NULL &&
                        father->father->child[1]->value==father->value)
                            father->father->child[1] = sibling;
                }
                //change father pointer
                sibling->child[dir]->father = father;
                father->father = sibling;
                //change child pointer
                father->child[1-dir] = sibling->child[dir];
                sibling->child[dir] = father;
                //change color
                sibling->color = father->color;
                father->color = red;
                //now it becomes case 2.1
                //next processing loop
            }
        }
    }
    //case 3 : T is black and has a red child, this red node must be leaf
    else{
        father->child[dir] = (T->child[0]!=NULL)? T->child[0] : T->child[1];
        father->child[dir]->father = father;
        father->child[dir]->color = black;
        free(T);
        T = father;
    }
    
    
    //*return the root*//
    while(T->father!=NULL) T=T->father;
    return T;
}


void Preorder_Traversal(RBTree T){
    if(T==NULL) printf("empty tree\n");
    else{
        printf("%d",T->value);
        if(T->color==black) printf("B ");
        else printf("R ");
	    if(T->child[0]!=NULL) Preorder_Traversal(T->child[0]);
	    if(T->child[1]!=NULL) Preorder_Traversal(T->child[1]);
    }
}
void Inorder_Traversal(RBTree T){
    if(T==NULL) printf("empty tree\n");
    else{
        if(T->child[0]!=NULL) Inorder_Traversal(T->child[0]);
        printf("%d",T->value);
        if(T->color==black) printf("B ");
        else printf("R ");
	    if(T->child[1]!=NULL) Inorder_Traversal(T->child[1]);
    }
}