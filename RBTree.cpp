#include <iostream>
#include <assert.h>
#include <queue>
#include <string>
#include "util/util.h"
using namespace std;
using namespace Util;
enum NodeColor{
    RED,
    BLACK
};

class Node {
    public:
    NodeColor node_color;
    Node * left_child;
    Node * right_child;
    Node * parent;
    int black_height;
    bool is_leaf;
    int height;
    int order_idx;
    int width;
    int value;
    Node(int value){
        init();
        this->value = value;
    }

    Node(){
        init();
    }

    void init(){
        node_color = NodeColor::RED;
        left_child = right_child = parent = nullptr;
        is_leaf = false;
        black_height = 0;
        height = order_idx = width = 0 ;
    }

    ~Node() {
        if(left_child != nullptr){
            delete left_child;
        }
        if(right_child != nullptr){
            delete right_child;
        }
        left_child = right_child = parent = nullptr;
        value = -1;
    }

    bool IsRoot(){
        return parent == nullptr;
    }

    void CreateLeaf() {
        assert(!is_leaf);
        assert(left_child==nullptr && right_child==nullptr);
        Node * left = new Node();
        left->node_color = NodeColor::BLACK;
        left->is_leaf = true;
        left->black_height = 1;
        left->parent = this;
        left_child = left;
        Node * right = new Node();
        right->node_color = NodeColor::BLACK;
        right->is_leaf = true;
        right->black_height = 1;
        right->parent = this;
        right_child = right;
        RefreshHeight();
    }

    void RefreshHeight() {
        assert (left_child != nullptr && right_child != nullptr);
        assert (left_child->black_height == right_child->black_height);
        black_height = node_color == NodeColor::BLACK ? left_child->black_height + 1 : left_child->black_height;
    }

    void MakeRed(){
        if (node_color == NodeColor::RED){
            return ;
        }
        node_color = NodeColor::RED;
        black_height -= 1;
    }

    void MakeBalck(){
        if (node_color == NodeColor::BLACK){
            return;
        }
        node_color = NodeColor::BLACK;
        black_height += 1;
    }

    void AddLeftChild(Node * node){
        assert(left_child == nullptr);
        assert(node->parent == nullptr);
        left_child = node;
        node->parent = this;
    }

    void AddRightChild(Node * node){
        assert(right_child == nullptr);
        assert(node->parent == nullptr);
        right_child = node;
        node->parent = this;
    }

    void RemoveFromParent(){
        if(parent != nullptr){
            if(parent->left_child == this){
                parent->left_child = nullptr;
            }else {
                parent->right_child = nullptr; 
            }
            parent = nullptr;
        }
    }

    bool operator > (const Node & other){
        return value > other.value;
    }

    bool operator < (const Node & other){
        return value < other.value;
    }

    bool operator == (const Node & other){
        return value == other.value;
    }
};

void PrintTree(Node * node){
    queue<Node*> node_que1, node_que2;
    queue<Node*> * cur_node_que_ptr = &node_que1, *back_node_que_ptr= &node_que2;
    queue<Node*> *tmp=nullptr;
    if(node==nullptr){
        cout << " Empty Tree !" << endl;
    }
    cur_node_que_ptr->push(node);
    while(!cur_node_que_ptr->empty()){
        int cnt = 0;
        while(!cur_node_que_ptr->empty()){
            node = cur_node_que_ptr->front();
            cur_node_que_ptr->pop();
            int print_cnt =  node->order_idx - cnt - 1;
            for (int i = 0; i < print_cnt; i++){
                print(" ");
                cnt += 1;
            }
            if (node->is_leaf){
                print("*");
            }else{
                if (node->node_color == NodeColor::RED){
                    print("<"+std::to_string(node->value)+">");
                }else {
                    print("<"+std::to_string(node->value)+">");
                }
            }
            cnt += 1;
            if (node->left_child!=nullptr){
                back_node_que_ptr->push(node->left_child);
            }
            if (node->right_child!=nullptr){
                back_node_que_ptr->push(node->right_child);
            }
        }
        cout << endl;
        tmp = cur_node_que_ptr;
        cur_node_que_ptr = back_node_que_ptr;
        back_node_que_ptr = tmp;
    }
}

void InsertNodeTree(Node * root, Node * node){
    if (root == nullptr){
        node->MakeBalck();
        node->CreateLeaf();
    }
}

void RefreshNodePosition(Node* node, int ** last_value){
    if (node == nullptr){
        return;
    }
    node->width = 1;
    node->height = 1;
    if (node->left_child != nullptr){
        RefreshNodePosition(node->left_child, last_value);
        node->width += node->left_child->width;
        node->height += node->left_child->height;
    }
    if(*last_value!=nullptr){
        node->order_idx = **last_value + 1;
    }else {
        node->order_idx = 1;
    }
    //cout << node->value << " " << node->order_idx << endl;
    *last_value = &node->order_idx;
    if(node->right_child != nullptr){
        RefreshNodePosition(node->right_child, last_value);
        node->width += node->right_child->width;
        if(node->right_child->height + 1 > node->height){
            node->height = node->right_child->height +1;
        }
    }
}

void RefreshTree(Node * root){
    int * ptr = nullptr;
    RefreshNodePosition(root, &ptr);
}

void FullBiaryTree(Node* root, int depth, int * idx){
    if(root==nullptr){
        return;
    }
    if(depth==0){
        return ;
    }
    *idx+=1;
    Node * l = new Node(*idx);
    *idx+=1;
    Node * r = new Node(*idx);
    root->AddLeftChild(l);
    root->AddRightChild(r);
    FullBiaryTree(l, depth-1, idx);
    FullBiaryTree(r, depth-1, idx);
}

void InsertValueToSearchTree(Node*root, int value){
    if(root==nullptr){
        return;
    }
    if(value <= root->value){
        if(root->left_child!=nullptr){
            InsertValueToSearchTree(root->left_child, value);
        }else {
            Node * node = new Node(value);
            root->AddLeftChild(node);
        }
    }else{
        if(root->right_child!=nullptr){
            InsertValueToSearchTree(root->right_child, value);
        }else {
            Node * node = new Node(value);
            root->AddRightChild(node);
        }
    }
}

void DelNodeByValue(Node* root, int value){
    if(root==nullptr){
        return;
    }
    cout << "find " << root->value << endl;
    if(value == root->value){
        if(root->left_child == nullptr && root->right_child == nullptr){
            root->RemoveFromParent();
            delete root;
        }else if(root->left_child != nullptr){
            Node * find_node = root->left_child;
            while(find_node->right_child != nullptr){
                find_node = find_node->right_child;
            }
            root->value = find_node->value;
            Node * parent = find_node->parent;
            bool is_left_child = parent->left_child == find_node ? true : false;
            find_node->RemoveFromParent();
            Node * f_l = find_node->left_child;
            if (f_l!=nullptr){    
                f_l->RemoveFromParent();
                if (is_left_child){
                    parent->AddLeftChild(f_l);
                }else {
                    parent->AddRightChild(f_l);
                }
            }
            delete find_node;
        }else{
            Node * find_node = root->right_child;
            while(find_node->left_child != nullptr){
                find_node = find_node->left_child;
            }
            root->value = find_node->value;
            Node * parent = find_node->parent;
            bool is_left_child = parent->left_child == find_node ? true : false;
            find_node->RemoveFromParent();
            Node * f_r = find_node->right_child;
            if (f_r!=nullptr){
                f_r->RemoveFromParent();
                if (is_left_child){
                    parent->AddLeftChild(f_r);
                }else {
                    parent->AddRightChild(f_r);
                }
            }
            delete find_node;
        }
    }else if(value < root->value){
        DelNodeByValue(root->left_child, value);
    }else{
        DelNodeByValue(root->right_child, value);
    }
}

int main(){
    Node * root = new Node(0);
    //FullBiaryTree(root, 4, &idx);
    InsertValueToSearchTree(root, 8);
    InsertValueToSearchTree(root, 4);
    InsertValueToSearchTree(root, 12);
    InsertValueToSearchTree(root, 5);
    InsertValueToSearchTree(root, 1);
    InsertValueToSearchTree(root, 3);
    InsertValueToSearchTree(root, 10);
    InsertValueToSearchTree(root, 11);

    RefreshTree(root);
    PrintTree(root);
    DelNodeByValue(root, 8);
    DelNodeByValue(root, 5);
    DelNodeByValue(root, 4);
    DelNodeByValue(root, 0);
    DelNodeByValue(root, 1);
    DelNodeByValue(root, 3);
    DelNodeByValue(root, 12);
    DelNodeByValue(root, 10);
    DelNodeByValue(root, 11);
    RefreshTree(root);
    PrintTree(root);
    return 0;
}