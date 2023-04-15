#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
using namespace std;
#define NIL &(node::__NIL)
struct node{
    node(int key=0,int color=0,node *lchild=NIL,node *rchild=NIL):key(key),color(color),lchild(lchild),rchild(rchild){}
    int color;
    int key;
    node *lchild;
    node *rchild;
    static node __NIL;
};
node node::__NIL(0, 1);

bool has_red_child(node *root){
    return !root->lchild->color || !root->rchild->color;
}
node *left_rotate(node *root){
    node *temp = root->rchild;
    root->rchild = temp->lchild;
    temp->lchild = root;
    return temp;
}
node *right_rotate(node *root){
    node *temp = root->lchild;
    root->lchild = temp->rchild;
    temp->rchild = root;
    return temp;
}
node *insert_maintain(node *root){
    int flag = 0;
    if(root->lchild->color == 0 && has_red_child(root->lchild)) flag = 1;//L
    if(root->rchild->color == 0 && has_red_child(root->rchild)) flag = 2;//R 
    if(flag == 0) return root;
    if(root->lchild->color==0 && root->rchild->color==0){
        root->color = 0;
        root->lchild->color = 1;
        root->rchild->color = 1;
       
    }
    else if(flag == 1){
        node *temp = root->lchild->lchild->color == 0 ? root->lchild->lchild : root->lchild->rchild;
        if(temp == root->lchild->rchild){
            root->lchild = left_rotate(root->lchild);
        }
        root = right_rotate(root);
       
    }
    else if(flag == 2){
        node *temp = root->rchild->rchild->color == 0 ? root->rchild->rchild : root->rchild->lchild;
        if(temp == root->rchild->lchild){
            root->rchild = right_rotate(root->rchild);
        }
        root = left_rotate(root);
       
    }
    root->color = 0;
    root->lchild->color = 1;
    root->rchild->color = 1;
    return root;
    
}
node *GetNewNode(int key){
    return new node(key);
}
node *__insert(node *root, int key){
    if(root == NIL) return GetNewNode(key);
    if(root->key == key) return root;
    if(root->key < key){
       root->rchild = __insert(root->rchild, key);
    }else{
       root->lchild = __insert(root->lchild, key);
    }
    return insert_maintain(root);
}
    node *insert(node *root, int key)
{
    root = __insert(root, key);
    root->color = 1;
    return root;
}
void print(node *root){
    printf("( %d | %d, %d, %d)\n",
    root->color,root->key,root->lchild->key,root->rchild->key);
    return;
}
void output(node *root){
    if(root == NIL) return;
    print(root);//采用先序方便观看树的形状
    output(root->lchild);
    
    output(root->rchild);
}

node *predecessor(node *root){  //获得前驱。
    node *temp = root->lchild;
    while(temp->rchild != NIL){
        temp = temp->rchild;
    }
    return temp;
}
node *erase_maintain(node *root){
    if(root->lchild->color != 2 && root->rchild->color !=2 ) return root;
    int flag = 0;
    if(has_red_child(root)){
        root->color = 0;
        if(root->lchild->color == 0){
            root = right_rotate(root);
            flag = 1;
        }else{
            root = left_rotate(root);
            flag = 2;
        }
        root->color = 1;
        if(flag == 1){
            root->rchild = erase_maintain(root->rchild);
        }else{
            root->lchild = erase_maintain(root->lchild);
        }
        return root;
     }
    if(root->lchild->color == 1 && !has_red_child(root->lchild) || 
        root->rchild->color == 1 && !has_red_child(root->rchild)){
         root->color += 1;  //细节点，黑色往上递归，并不是一次解决好。
         root->lchild->color -= 1;
         root->rchild->color -= 1;
         return root;
     }
    if(root->lchild->color == 1){
        root->rchild->color = 1;
        if(root->lchild->lchild->color != 0){
            root->lchild->color = 0;
            root->lchild = left_rotate(root->lchild);
            root->lchild->color = 1;
        }
        root->lchild->color = root->color;
        root = right_rotate(root);
        } else {
        root->lchild->color = 1;
       
        if(root->rchild->rchild->color != 0){
            root->rchild->color = 0;
            root->rchild = right_rotate(root->rchild);
            root->rchild->color = 1;
        }
         root->rchild->color = root->color;
         root = left_rotate(root);
    }
    root->lchild->color = root->rchild->color = 1;
    return root;
}
node *__erase(node *root, int key){
    if (root == NIL) return root;
   
    if (root->key < key){
        root->rchild = __erase(root->rchild, key);
    } else if (root->key > key){
        root->lchild = __erase(root->lchild, key);
    } else {
        if(root->lchild == NIL || root->rchild == NIL){
            node *temp = root->lchild == NIL ? root->rchild : root->lchild;
            temp->color += root->color;// 细节点
            delete root;
            return temp;
        } else {
            node *temp = predecessor(root);
            root->key = temp->key;
            root->lchild = __erase(root->lchild, temp->key);
        }
    }
    return erase_maintain(root);
}
node *erase(node *root, int key){
    root = __erase(root, key);
    root->color = 1;
    return root;
}
void clear(node *root){
    if(root == NIL) return;
    clear(root->lchild);
    clear(root->rchild);
    delete (root);
    return;
}
int main(){
    int op, val;
    node *root = NIL;
    while(cin >> op >>val){
        switch (op)
        {
        case 1:
            root = insert(root,val);
            break;
        case 2:
            root = erase(root, val);
        default:
            break;
        }
        cout << endl <<"===== rbtree  print =====" << endl;
        output(root);
        cout << "==== rbtree print done ====" << endl;
    }
    clear(root);
    return 0;
}