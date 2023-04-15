#include <iostream>
#include <stdlib.h>
using namespace std;
#define NIL &(node::__NIL)
struct node{
    node(int key = 0, int color = 0, node* left = NIL, node* right = NIL):
    key(key), color(color), left(left), right(right) {}
    int key;
    int color;
    node* left;
    node* right;
    static node __NIL;
};
node node::__NIL(0,1);

node* left_rotate(node* root) {
    node* tmp = root->right;
    root->right = tmp->left;
    tmp->left = root;
    return tmp;
}

node* right_rotate(node* root) {
    node* tmp = root->left;
    root->left = tmp->right;
    tmp->right = root;
    return tmp;
}

node *pinoe(node* root) {
    node* tmp = root->left;
    while(tmp->right != NIL) {
        tmp = tmp->right;
    }
    return tmp;
}

bool has_red_node(node* root) {
    return root->left->color == 0 || root->right->color == 0;
}

node* GetNewNode(int key) {
    return new node(key);
}

node* insert_maintain(node* root) {
    int flag = 0;
    if(root->left == 0 && has_red_node(root->left)) flag = 1;
    if(root->right == 0 && has_red_node(root->right)) flag = 2;
    if(flag == 0) return root;
    if(root->left->color == root->right->color == 0) {
        root->color == 0;   //不能是-= 不可能同时出现两种不匹配的情况
        root->left->color = root->right->color = 1;
        return root;
    }
    if(flag == 1) {
        if(root->left->left->color != 0) {
            root->left = left_rotate(root->left);
        }
        root = right_rotate(root);
        root->color = 0;
        root->left->color = root->right->color = 1;
    } else {
        if(root->right->right->color != 0) {
            root->right = right_rotate(root->right);
        }
        root = left_rotate(root);
        root->color = 0;
        root->left->color = root->right->color = 1;
    }
    return root;
}

node* __insert(node* root, int key) {
    if(root == NIL) return GetNewNode(key);
    if(key < root->key) {
        root->left = __insert(root->left, key);
    } else if (key > root->key) {
        root->right = __insert(root->right, key);
    } else {
        return root;
    }
    return insert_maintain(root);
}

node* insert(node* root, int key) {
    root = __insert(root, key);
    root->color = 1;
    return root;
}

node* erase_maintain(node* root) {
    if(root->left->color != 2 && root->right->color != 2) return root;
    int flag = 0;
    if(has_red_node(root)) {
        flag = root->left->color == 0? 1: 2;
        if(flag == 1) {
             root->color = 0;
            root = right_rotate(root);
            root->color = 1;
            root->right = erase_maintain(root->right);
        } else {
             root->color = 0;
            root = left_rotate(root);
           root->color = 1;
            root->left = erase_maintain(root->left);
        }
    }
    if(root->left->color == 2 && !has_red_node(root->right) || 
    root->right->color == 2 && !has_red_node(root->left)) {
        root->color += 1;
        root->left->color -= 1;
        root->right->color -= 1;
        return root;
    }
    if(root->left->color == 1) {
        root->right->color = 1;
        if(root->left->left->color != 0) {
            root->left->color = 0;
            root->left = left_rotate(root->left);
            root->left->color = 1;
        }
        root->left->color = root->color;
        root = right_rotate(root); 
    } else {
        root->left->color = 1;
        if(root->right->right->color != 0) {
            root->right->color = 0;
            root->right = right_rotate(root->right);
            root->right->color = 1;
        }
        root->right->color = root->color;
        root = left_rotate(root);
    }
    root->left->color = root->right->color = 1;
    return root;
}

node *__erase(node* root, int key) {
    if(root == NIL) return root;
    if(root->key < key) {
        root->right =  __erase(root->right, key);
    } else if (root->key > key) {
        root->left = __erase(root->left, key);
    } else {
        if(root->left == NIL || root->right == NIL) {
            node* tmp = root->left == NIL? root->right : root->left;
            tmp->color += root->color;
            delete root;
            return tmp;
        } else {
            node* tmp = pinoe(root);
            root->key = tmp->key;
            root->left = __erase(root->left, tmp->key);
        }
    }
    return erase_maintain(root);
}

node* erase(node* root, int key) {
    root = __erase(root, key);
    root->color = 1;
    return root;
}

void print(node* root) {
    printf("(%d | %d, %d, %d)\n", root->color, root->key, root->left->key, root->right->key);
}

void output(node* root) {
    if(root == NIL) return;
    print(root);
    output(root->left);
    output(root->right);
}

int main() {
    int op, val;
    node* root = NIL;
    while(cin >> op >> val) {
        switch(op) {
            case 1: 
                root = insert(root, val);
                break;
            case 2:
                root = erase(root, val);
                break;
            default:
                break;
        }
        printf("==== RBTree Print =====\n");
        output(root);
        printf("==== PRINT END ====\n");
    }   

    return 0;
}