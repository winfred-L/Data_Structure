#include "AVLTree.h"

#include <stdio.h>
#include <stdlib.h>

AVLTree CreateEmptyAVL(void){
	return NULL;
}
AVLTree CreateAVLNode(int val, int h, AVLTree f){
	AVLTree T = (AVLTree)malloc(sizeof(struct AVLnode));
	T->value = val;
	T->height = h;
	T->father = f;
	T->child[0] = T->child[1] = NULL;
	if(f!=NULL){
		//link father node
        if(val<f->value) f->child[0] = T;
        else f->child[1] = T;
	}
	return T;
}
int Height(AVLTree T){
	return (T==NULL)? -1 : T->height;
}
int FreshHeight(AVLTree T){
	return Height(T->child[0])>Height(T->child[1]) ? Height(T->child[0])+1 : Height(T->child[1])+1;
}
AVLTree InsertAVL(AVLTree T, int val){
	//step 1 : insert as BST
	AVLTree root = T;
    if(T==NULL){
        T = CreateAVLNode(val,0,NULL);
		return T;
    }else{
        AVLTree father;
        int dir;
        while(T!=NULL){
            father = T;
            if(val==T->value){
                printf("%d is already in tree!\n",val);
                return root;
            }else if(val<T->value){
                T = T->child[0];
            }else{
                T = T->child[1];
            }
        }
        if(T==NULL){//insert
            T = CreateAVLNode(val,0,father);
        }
    }
	//step 2 : refresh height and rebalance
	while(T!=NULL){
		T->height = FreshHeight(T);
		if(Height(T->child[0])-Height(T->child[1])==2){
			if(Height(T->child[0]->child[0])-Height(T->child[0]->child[1])==1){//LL case
				T = SingleRotation(T,0);
			}else{//LR case
				T = DoubleRotation(T,0);
			}
		}else if(Height(T->child[1])-Height(T->child[0])==2){
			if(Height(T->child[1]->child[0])-Height(T->child[1]->child[1])==1){//RL case
				T = DoubleRotation(T,1);
			}else{//RR case
				T = SingleRotation(T,1);
			}
		}
		T->height = FreshHeight(T);
		T = T->father;
	}
	//return root
	while(root->father!=NULL) root=root->father;
	return root;
}


AVLTree DeleteAVL(AVLTree T, int val){
	//step 1 : find as BST
    AVLTree root = T;
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
    }//now T is the node to be deleted
	
	//step 2 : delete as BST
	//if T has two children, move it to make it one or no child
	if(T->child[0]!=NULL && T->child[1]!=NULL){
        //get the smallest in the right subtree
        AVLTree X = T->child[1];
        while(X->child[0]!=NULL) X=X->child[0];
        //exchange T and X value
        int tem = T->value;
        T->value = X->value;
        X->value = tem;
        //set T to new position
        T = X;
    }
	//delete
	AVLTree father = T->father;
	if(T->child[0]==NULL && T->child[1]==NULL){//T has no child
		if(father==NULL){//T is root
			free(T);
			return NULL;
		}else{
			if(father->child[0]!=NULL&&father->child[0]->value==T->value) father->child[0]=NULL;
			else father->child[1]=NULL;
			free(T);
			T = father;
		}
	}else{//T has one child
		int dir;
		if(T->child[0]!=NULL) dir=0;
		else dir=1;
		if(father!=NULL){
			if(father->child[0]!=NULL&&father->child[0]->value==T->value) father->child[0]=T->child[dir];
			else father->child[1]=T->child[dir];
		}
		T = T->child[dir];//T becomes the child
		free(T->father);
		T->father = father;
	}

	//step 3 : refresh height and rebalance
	root = T;//old root may be deleted, here just save a node
	while(T!=NULL){
		T->height = FreshHeight(T);
		if(Height(T->child[0])-Height(T->child[1])==2){
			if(Height(T->child[0]->child[0])-Height(T->child[0]->child[1])==1){//LL case
				T = SingleRotation(T,0);
			}else{//LR case
				T = DoubleRotation(T,0);
			}
		}else if(Height(T->child[1])-Height(T->child[0])==2){
			if(Height(T->child[1]->child[0])-Height(T->child[1]->child[1])==1){//RL case
				T = DoubleRotation(T,1);
			}else{//RR case
				T = SingleRotation(T,1);
			}
		}
		T->height = FreshHeight(T);
		T = T->father;
	}
	//return root
	while(root->father!=NULL) root=root->father;
	return root;
}


void Preorder_Traversal(AVLTree T){
    if(T==NULL) printf("empty tree\n");
    else{
        printf("%d  ",T->value);
	    if(T->child[0]!=NULL) Preorder_Traversal(T->child[0]);
	    if(T->child[1]!=NULL) Preorder_Traversal(T->child[1]);
    }
}
void Inorder_Traversal(AVLTree T){
    if(T==NULL) printf("empty tree\n");
    else{
        if(T->child[0]!=NULL) Inorder_Traversal(T->child[0]);
        printf("%d ",T->value);
	    if(T->child[1]!=NULL) Inorder_Traversal(T->child[1]);
    }
}



/*    T
     / \
    S   c
   / \
  a   b
*/
AVLTree SingleRotation(AVLTree T, int dir){
	AVLTree S = T->child[dir];//son
	//change subtree link
	S->father = T->father;
	if(T->father!=NULL){
		if(T->father->child[0]!=NULL&&T->father->child[0]->value==T->value) T->father->child[0]=S;
		else T->father->child[1]=S;
	}
	//change father pointer
	if(S->child[1-dir]!=NULL) S->child[1-dir]->father = T;
	T->father = S;
	//change child pointer
	T->child[dir] = S->child[1-dir];
	S->child[1-dir] = T;
	//change height
	T->height = FreshHeight(T);
	S->height = FreshHeight(S);
	return S;
}

/*    T
     / \
    S   d
   / \
  a   G
     / \
    b   c
*/
AVLTree DoubleRotation(AVLTree T, int dir){
	AVLTree S = T->child[dir];//son
	AVLTree G = S->child[1-dir];//grandson
	//change subtree link
	G->father = T->father;
	if(T->father!=NULL){
		if(T->father->child[0]!=NULL&&T->father->child[0]->value==T->value) T->father->child[0]=G;
		else T->father->child[1]=G;
	}
	//change father pointer
	if(G->child[dir]!=NULL) G->child[dir]->father = S;
	if(G->child[1-dir]!=NULL) G->child[1-dir]->father = T;
	S->father = T->father = G;
	//change child pointer
	S->child[1-dir] = G->child[dir];
	T->child[dir] = G->child[1-dir];
	G->child[dir] = S;
	G->child[1-dir] = T;
	//change height
	T->height = FreshHeight(T);
	S->height = FreshHeight(S);
	G->height = FreshHeight(G);
	return G;
}





