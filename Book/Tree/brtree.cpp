#include <iostream>
#include <stdlib.h> 
using namespace std;
#define NIL &(node::__NIL)
struct node {
    node(int key = 0, int color = 0, node *lchild = NIL, node *rchild = NIL):
    key(key),color(color),lchild(lchild),rchild(rchild){}
    int key;
    int color;
    node *lchild;
    node *rchild;
    static node __NIL;
};
node node::__NIL(0, 1);
node *GetNewNode(int key){
    return new node(key);
}
bool has_red_node(node *root){
    return root->lchild->color == 0 || root->rchild->color == 0;
}
node *left_rotate(node *root){
    node *temp = root->rchild;
    root->rchild = temp->lchild;
    temp->lchild = root;
}
node *right_rotate(node *root){
    node *temp = root->lchild;
    root->lchild = temp->rchild;
    temp->rchild = root;
}
node *insert_maintain(node *root){
    int flag = 0;
    if(root->lchild->color == 0 && has_red_node(root->lchild)) flag = 1;
    if(root->rchild->color == 0 && has_red_node(root->rchild)) flag = 2;
    if(flag == 0) return root;
    if(root->lchild->color == 0 && root->rchild->color == 0){
        root->color = 0;
        root->lchild->color = root->rchild->color = 1;
        return root;
    }
    if(flag == 1){
        if(root->lchild->rchild->color == 0){
            root->lchild = left_rotate(root->lchild);
        }
        root = right_rotate(root);
    } else {
        if(root->rchild->lchild->color == 0){
            root->rchild = right_rotate(root->rchild);
        }
        root = left_rotate(root);
    }
    root->color = 0;
    root->lchild->color = root->rchild->color = 1;
    return root;
}
node *__insert(node *root, int key){
    if (root == NIL) return GetNewNode(key);
    if(key == root->key) return root;
    if(key<root->key){
        root->lchild = __insert(root->lchild, key);
    } else {
        root->rchild = __insert(root->rchild, key);
    }
    return insert_maintain(root);
}
node *insert(node *root, int key){
    root = __insert(root, key);
    root->color = 1;
    return root;
}
void print(node *root){
    printf("(%d | %d,%d,%d)\n", root->color, root->key, root->lchild->key, root->rchild->key);
}
void output(node *root){
    if(root == NIL)return ;
    print(root);
    output(root->lchild);
    
    output(root->rchild);
}
node *erase_maintain(node *root){
    if(root->lchild->color != 2 && root->rchild->color != 2) return root;
    int flag = 0;
    if(has_red_node(root)){
        flag = root->lchild->color == 0 ? 1 : 2;
        if(flag == 1){
            root->color = 0;
            root = right_rotate(root);
            root->color = 1;
            root->rchild = erase_maintain(root->rchild);
        } else {
            root->color = 0;
            root = left_rotate(root);
            root->color = 1;
            root->lchild = erase_maintain(root->lchild);
        }
        return root;
    }
    if(root->lchild->color == 1 && !has_red_node(root->lchild) || root->rchild->color == 1 && !has_red_node(root->rchild)){
        root->color += 1;
        root->lchild->color -= 1;
        root->rchild->color -= 1;
        return root;
    }
    //接下来的步骤卡了一下
    if(root->rchild->color == 1){
        root->lchild->color = 1;
        if(root->rchild->lchild->color == 0){
            root->rchild->color = 0;
            root->rchild = right_rotate(root->rchild);
            root->rchild->color = 1;
        }
        root->rchild->color = root->color;
        root = left_rotate(root);
    } else {
        root->lchild->color = 1;
        if(root->lchild->rchild == 0){
            root->lchild->color = 0;
            root->lchild = left_rotate(root->lchild);
            root->lchild->color = 1;
        }
        root->lchild->color = root->color;
        root = right_rotate(root);
    }
    root->lchild->color = root->rchild->color = 1;
    return root;
}
node *pione(node *root){
    node *temp = root->lchild;
    while(temp->rchild != NIL) {
        temp = temp->rchild;
    }
    return temp;
}
node *__erase(node *root, int key){
   if(root == NIL) return root;
    if(root->key < key){
        root->rchild = __erase(root->rchild, key);
    } else if(root->key > key){
        root->lchild = __erase(root->lchild, key);
    }else{
        if(root->lchild == NIL || root->rchild == NIL){
            node *temp = root->lchild == NIL ? root->rchild : root->lchild;
            temp->color += root->color;
            delete root;
            return temp;
        }
        node *temp = pione(root);
        root->key = temp->key;
        root->lchild = __erase(root->lchild,temp->key);
    }
    return erase_maintain(root);
}
node *erase(node *root, int key){
    root = __erase(root, key);
    root->color = 1;
    return root;
}
int main(){
    int op, val;
    node *root = NIL;
    while(cin >> op >> val){
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
        printf("===== RBTree Print =====\n");
        output(root);
        printf("===== PRINT END =====\n");
    }
    return 0;
}