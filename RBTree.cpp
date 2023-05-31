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
        assert(left_child==nullptr or right_child==nullptr);
        if (left_child == nullptr){
            Node * left = new Node();
            left->node_color = NodeColor::BLACK;
            left->is_leaf = true;
            left->parent = this;
            left_child = left;
        }
        if(right_child == nullptr){
            Node * right = new Node();
            right->node_color = NodeColor::BLACK;
            right->is_leaf = true;
            right->parent = this;
            right_child = right;
        }
    }

    void MakeRed(){
        if (node_color == NodeColor::RED){
            return ;
        }
        node_color = NodeColor::RED;
    }

    void MackBlack(){
        if (node_color == NodeColor::BLACK){
            return;
        }
        node_color = NodeColor::BLACK;
    }

    void AddLeftChild(Node * node){
        assert(left_child == nullptr || left_child->is_leaf);
        assert(node->parent == nullptr);
        if (left_child != nullptr && left_child->is_leaf){
            left_child->RemoveFromParent();
            delete left_child;
        }
        left_child = node;
        node->parent = this;
    }

    void AddRightChild(Node * node){
        assert(right_child == nullptr || right_child->is_leaf);
        assert(node->parent == nullptr);
        if (right_child!= nullptr && right_child->is_leaf){
            right_child->RemoveFromParent();
            delete right_child;
        }
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

    bool HasValueLeftChild(){
        return !left_child->is_leaf;
    }

    bool HasValueRightChild(){
        return !right_child->is_leaf;
    }

    bool ImLeftNode(){
        return parent != nullptr && this == parent->left_child;
    }

    bool ImRightNode() {
        return parent !=nullptr && this == parent->right_child;
    }

    bool ImRootNode() {
        return parent == nullptr;
    }

    Node * UncleNode() {
        if (parent == nullptr || parent->parent == nullptr){
            return nullptr;
        }
        return ImLeftNode() ? parent->parent->right_child : parent->parent->left_child;
    }

    Node * Brother() {
        if (parent == nullptr){
            return nullptr;
        }
        return ImLeftNode()? parent->right_child : parent->left_child;
    }

    bool IsBalck(){
        return node_color == NodeColor::BLACK;
    }

    Node * RightRotation(){
        Node * _parent = parent;
        Node * lr = left_child->right_child;
        Node * l = left_child;
        bool is_left = ImLeftNode();
        left_child->RemoveFromParent();
        lr->RemoveFromParent();
        RemoveFromParent();
        AddLeftChild(lr);
        l->AddRightChild(this);
        if(_parent != nullptr){
            if (is_left){
                _parent->AddLeftChild(l);
            }else{
                _parent->AddRightChild(l);
            }
            return nullptr;
        }
        return l;
    }

    Node * LeftRotation() {
        Node * _parent = parent;
        Node * rl = right_child->left_child;
        Node * r = right_child;
        bool is_left = ImLeftNode();
        right_child->RemoveFromParent();
        rl->RemoveFromParent();
        RemoveFromParent();
        AddRightChild(rl);
        r->AddLeftChild(this);
        if (parent != nullptr){
            if (is_left){
                _parent->AddLeftChild(r);
            }else{
                _parent->AddRightChild(r);
            }
            return nullptr;
        }
        return r;
    }
};


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

void PrintTree(Node * node){
    Node * root = node;
    queue<Node*> node_que1, node_que2;
    queue<Node*> * cur_node_que_ptr = &node_que1, *back_node_que_ptr= &node_que2;
    queue<Node*> *tmp=nullptr;
    if(node==nullptr){
        cout << " Empty Tree !" << endl;
    }
    RefreshTree(node);
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
                print("[*]");
            }else{
                if (node->node_color == NodeColor::RED){
                    print("<"+std::to_string(node->value)+">");
                }else {
                    print("["+std::to_string(node->value)+"]");
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
    for (int i = 0; i < root->width * 4; i++){
        cout << "-";
    }
    cout << endl;
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

void BinarySearchTreeTest(){
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
    PrintTree(root);
}

Node * RepairInsertTree(Node * node){
    assert(!node->IsBalck());
    cout << "0" << endl;
    Node * parent = node->parent;
    Node * uncle = node->UncleNode();
    Node * grand_parent = parent->parent;
    //cout << node->value << " " << parent->value << " " << uncle->value<< " " << grand_parent->value<< endl;
        
    if (parent->IsBalck()){
        cout << "1" << endl;
        return nullptr;
    }
    if (!parent->IsBalck() && !uncle->IsBalck()){
        parent->MackBlack();
        uncle->MackBlack();
        if(grand_parent->IsRoot()){
            cout << "2" << endl;
            return nullptr;
        }
        cout << "3" << endl;
        grand_parent->MakeRed();
        return RepairInsertTree(grand_parent);
    }
    grand_parent->MakeRed();
    parent->MackBlack();
    if (parent->ImLeftNode()){
        if (node->ImRightNode()){
            cout << "4" << endl;
            parent->LeftRotation();
        }
        cout << "5" << endl;
        return grand_parent->RightRotation();
    }
    else{
        if (node->ImLeftNode()){
            cout << "6" << endl;
            parent->RightRotation();
        }
        cout << "7" << endl;
        return grand_parent->LeftRotation();
    }
}

Node * BRTreeInsert(Node * root, int val){
    Node * node = new Node(val);
    node->MakeRed();
    node->CreateLeaf();
    if (root == nullptr){
        node->MackBlack();
        return node;
    }
    Node * find_node = root;
    while(find_node != nullptr){
        if (find_node->value > val && find_node->HasValueLeftChild()){
            find_node = find_node->left_child;
        }else if(find_node->value < val && find_node->HasValueRightChild()){
            find_node = find_node->right_child;
        }else{
            break;
        }
    }
    if (find_node->value > val){
        find_node->AddLeftChild(node);
    }else {
        find_node->AddRightChild(node);
    }
    if(find_node->parent == nullptr){
        // 只有根节点一层
        return root;
    }
    Node * new_root = RepairInsertTree(node);
    return  new_root == nullptr ? root : new_root; 
}

Node * RepairRemoveTree(Node * node){
    if (node->IsRoot()){
        if(node->right_child->is_leaf){
            delete node;
            return nullptr;
        }else{
            Node * r = node->right_child;
            r->RemoveFromParent();
            delete node;
            return r;
        }
    }
    return nullptr;
}


Node * BRTreeRemove(Node * root, int val){
    if (root == nullptr){
        return root;
    }
    Node * find_node = root;
    while(find_node != nullptr){
        if (val < find_node->value){
            if(find_node->left_child->is_leaf){
                return root;
            }
            find_node = find_node->left_child;
        }else if (val > find_node->value){
            if(find_node->right_child->is_leaf){
                return root;
            }
            find_node = find_node->right_child;
        }else {
            break;
        }
    }
    if(!find_node->left_child->is_leaf && !find_node->right_child->is_leaf){
        Node * del_node = find_node;
        // # 找前驱
        find_node = find_node->left_child;
        while(!find_node->right_child->is_leaf){
            find_node = find_node->right_child;
        }
        del_node->value = find_node->value;
    }
    Node * parent = find_node->parent;
    Node * brother = find_node->Brother();
    if(parent != nullptr && find_node->parent->parent == nullptr){
        find_node->RemoveFromParent();
        delete find_node;
        if (!brother->is_leaf and brother->IsBalck()){
            brother->MakeRed();
        }
        parent->CreateLeaf();
        return root;
    }
    if(!find_node->IsBalck()){
        bool is_left_child = find_node->ImLeftNode();
        if (find_node->left_child->is_leaf && find_node->right_child->is_leaf){
            find_node->RemoveFromParent();
            if (parent != nullptr){
                parent->CreateLeaf();
            }
            delete find_node;
            return root;
        }else if(find_node->left_child->is_leaf){
            Node * rc = find_node->right_child;
            find_node->RemoveFromParent();
            if (parent != nullptr){
                parent->CreateLeaf();
            }
            delete find_node;
            is_left_child ? parent->AddLeftChild(rc) : parent->AddRightChild(rc);
            return root;
        }else if(find_node->right_child->is_leaf){
            Node * lc = find_node->left_child;
            find_node->RemoveFromParent();
            if (parent != nullptr){
                parent->CreateLeaf();
            }
            delete find_node;
            is_left_child ? parent->AddLeftChild(lc) : parent->AddRightChild(lc);
            return root;
        }
        // 不可能存在删除双子树的情况
        assert(true);
    }else{
        // 删除黑节点
        return RepairRemoveTree(find_node);
    }
    return root;
}

void BRTreeTest1(){
    Node * root = BRTreeInsert(nullptr, 7);
    PrintTree(root);
    BRTreeInsert(root, 5);
    PrintTree(root);
    BRTreeInsert(root, 9);
    PrintTree(root);
    BRTreeInsert(root, 3);
    PrintTree(root);
    BRTreeInsert(root, 6);
    PrintTree(root);
    BRTreeInsert(root, 8);
    PrintTree(root);
    BRTreeInsert(root, 10);
    PrintTree(root);
}

int main(){
    int max_val = 3;
    Node * root = BRTreeInsert(nullptr, max_val);
    PrintTree(root);
    for(int i = 1; i < max_val; i++){
        root = BRTreeInsert(root, max_val - i);
        cout << "root:" << root << endl;
        PrintTree(root);
    }
    root = BRTreeRemove(root, 2);
    PrintTree(root);
    root = BRTreeRemove(root, 1);
    PrintTree(root);
    root = BRTreeRemove(root, 3);
    PrintTree(root);
    return 0;
}