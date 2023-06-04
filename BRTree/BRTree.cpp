// BRTree.cpp : black red tree
//
#include "stdafx.h"
#include "stdlib.h"
#include "time.h"
#include <iostream>
#include <assert.h>
#include <queue>
#include <string>
#include <vector>
#include "util.h"
using namespace std;
using namespace Util;
enum NodeColor {
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
	Node(int value) {
		init();
		this->value = value;
	}

	Node() {
		init();
	}

	void init() {
		node_color = NodeColor::RED;
		left_child = right_child = parent = nullptr;
		is_leaf = false;
		height = order_idx = width = 0;
	}

	~Node() {
		if (left_child != nullptr) {
			delete left_child;
		}
		if (right_child != nullptr) {
			delete right_child;
		}
		left_child = right_child = parent = nullptr;
		value = -1;
	}

	bool IsRoot() {
		return parent == nullptr;
	}

	bool HasSon() {
		Node * ll = left_child->left_child;
		Node * lr = left_child->right_child;
		Node * rl = right_child->left_child;
		Node * rr = right_child->right_child;
		return (ll != nullptr || lr != nullptr || rl != nullptr || rr != nullptr);
	}

	void CreateLeaf() {
		assert(!is_leaf);
		if (left_child == nullptr) {
			Node * left = new Node(-1);
			left->node_color = NodeColor::BLACK;
			left->is_leaf = true;
			left->parent = this;
			left_child = left;
		}
		if (right_child == nullptr) {
			Node * right = new Node(-1);
			right->node_color = NodeColor::BLACK;
			right->is_leaf = true;
			right->parent = this;
			right_child = right;
		}
	}

	void MakeRed() {
		if (is_leaf) {
			return;
		}
		if (node_color == NodeColor::RED) {
			return;
		}
		node_color = NodeColor::RED;
	}

	void MakeBlack() {
		if (node_color == NodeColor::BLACK) {
			return;
		}
		node_color = NodeColor::BLACK;
	}

	void AddLeftChild(Node * node) {
		assert(left_child == nullptr || left_child->is_leaf);
		assert(node->parent == nullptr);
		if (left_child != nullptr && left_child->is_leaf) {
			left_child->RemoveFromParent();
			delete left_child;
		}
		left_child = node;
		node->parent = this;
	}

	void AddRightChild(Node * node) {
		assert(right_child == nullptr || right_child->is_leaf);
		assert(node->parent == nullptr);
		if (right_child != nullptr && right_child->is_leaf) {
			right_child->RemoveFromParent();
			delete right_child;
		}
		right_child = node;
		node->parent = this;
	}

	void RemoveFromParent() {
		if (parent != nullptr) {
			if (parent->left_child == this) {
				parent->left_child = nullptr;
			}
			else {
				parent->right_child = nullptr;
			}
			parent->CreateLeaf();
			parent = nullptr;
		}
	}

	bool HasValueLeftChild() {
		return !left_child->is_leaf;
	}

	bool HasValueRightChild() {
		return !right_child->is_leaf;
	}

	bool ImLeftNode() {
		return parent != nullptr && this == parent->left_child;
	}

	bool ImRightNode() {
		return parent != nullptr && this == parent->right_child;
	}

	bool ImRootNode() {
		return parent == nullptr;
	}

	Node * UncleNode() {
		if (parent == nullptr || parent->parent == nullptr) {
			return nullptr;
		}
		return parent->ImLeftNode() ? parent->parent->right_child : parent->parent->left_child;
	}

	Node * Brother() {
		if (parent == nullptr) {
			return nullptr;
		}
		return ImLeftNode() ? parent->right_child : parent->left_child;
	}

	bool IsBalck() {
		return node_color == NodeColor::BLACK;
	}

	Node * RightRotation() {
		Node * _parent = parent;
		Node * lr = left_child->right_child;
		Node * l = left_child;
		bool is_left = ImLeftNode();
		left_child->RemoveFromParent();
		lr->RemoveFromParent();
		RemoveFromParent();
		AddLeftChild(lr);
		l->AddRightChild(this);
		if (_parent != nullptr) {
			if (is_left) {
				_parent->AddLeftChild(l);
			}
			else {
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
		if (_parent != nullptr) {
			if (is_left) {
				_parent->AddLeftChild(r);
			}
			else {
				_parent->AddRightChild(r);
			}
			return nullptr;
		}
		return r;
	}

	Node * UpRotationRLSon() {
		bool is_left = ImLeftNode();
		Node * _parent = parent;
		Node * _root = this;
		Node * r = right_child;
		Node * r_son = r->left_child;
		Node * r_son_l = r_son->left_child;
		Node * r_son_r = r_son->right_child;
		_root->RemoveFromParent();
		r_son_l->RemoveFromParent();
		r_son->RemoveFromParent();
		r->RemoveFromParent();
		r_son_r->RemoveFromParent();
		r->AddLeftChild(r_son_r);
		_root->AddRightChild(r_son_l);
		r_son->AddLeftChild(_root);
		r_son->AddRightChild(r);		
		if (_parent != nullptr) {
			if (is_left) {
				_parent->AddLeftChild(r_son);
			}
			else {
				_parent->AddRightChild(r_son);
			}
			return nullptr;
		}
		return r_son;
	}

	Node * UpRotationLRSon() {
		bool is_left = ImLeftNode();
		Node * _parent = parent;
		Node * _root = this;
		Node * l = left_child;
		Node * l_son = l->right_child;
		Node * l_son_l = l_son->left_child;
		Node * l_son_r = l_son->right_child;
		_root->RemoveFromParent();
		l_son_r->RemoveFromParent();
		l_son->RemoveFromParent();
		l->RemoveFromParent();
		l_son_l->RemoveFromParent();
		l->AddRightChild(l_son_l); // ok
		_root->AddLeftChild(l_son_r); // ok
		l_son->AddRightChild(_root); // ok
		l_son->AddLeftChild(l); // ok
		if (_parent != nullptr) {
			if (is_left) {
				_parent->AddLeftChild(l_son);
			}
			else {
				_parent->AddRightChild(l_son);
			}
			return nullptr;
		}
		return l_son;
	}
};

int _RBTreeCheckBlackHeight(Node * root, bool & valid) {
	if (root == nullptr) {
		return 0;
	}
	if (root->is_leaf) {
		valid = valid && root->IsBalck();
		return 1;
	}
	int l_b_h = _RBTreeCheckBlackHeight(root->left_child, valid);
	int r_b_h = _RBTreeCheckBlackHeight(root->right_child, valid);
	valid = valid && (l_b_h == r_b_h);
	if (root->IsBalck()) {
		return l_b_h + 1;
	}
	else {
		valid = valid && root->left_child->IsBalck() && root->right_child->IsBalck();
		if (root->parent == nullptr) {
			//���
			valid = false;
		}
		return l_b_h;
	}
}

bool RBTreeCheckBlackHeight(Node * root) {
	bool ret = true;
	int black_heigt = _RBTreeCheckBlackHeight(root, ret);
	return ret;
}

void RefreshNodePosition(Node* node, int ** last_value) {
	if (node == nullptr) {
		return;
	}
	node->width = 1;
	node->height = 1;
	if (node->left_child != nullptr) {
		RefreshNodePosition(node->left_child, last_value);
		node->width += node->left_child->width;
		node->height += node->left_child->height;
	}
	if (*last_value != nullptr) {
		node->order_idx = **last_value + 1;
	}
	else {
		node->order_idx = 1;
	}
	//cout << node->value << " " << node->order_idx << endl;
	*last_value = &node->order_idx;
	if (node->right_child != nullptr) {
		RefreshNodePosition(node->right_child, last_value);
		node->width += node->right_child->width;
		if (node->right_child->height + 1 > node->height) {
			node->height = node->right_child->height + 1;
		}
	}
}


void RefreshTree(Node * root) {
	int * ptr = nullptr;
	RefreshNodePosition(root, &ptr);
}

void PrintTree(Node * node) {
	Node * root = node;
	bool valid = RBTreeCheckBlackHeight(root);
	if (!valid) {
		cout << " ============== invalid =============" << endl;
		//return;
	}
	queue<Node*> node_que1, node_que2;
	queue<Node*> * cur_node_que_ptr = &node_que1, *back_node_que_ptr = &node_que2;
	queue<Node*> *tmp = nullptr;
	if (node == nullptr) {
		cout << " Empty Tree !" << endl;
		return;
	}
	RefreshTree(node);
	cur_node_que_ptr->push(node);
	while (!cur_node_que_ptr->empty()) {
		int cnt = 0;
		while (!cur_node_que_ptr->empty()) {
			node = cur_node_que_ptr->front();
			cur_node_que_ptr->pop();
			int print_cnt = node->order_idx - cnt - 1;
			for (int i = 0; i < print_cnt; i++) {
				print(" ");
				cnt += 1;
			}
			if (node->is_leaf) {
				print("[*]");
			}
			else {
				if (node->node_color == NodeColor::RED) {
					print("<" + std::to_string(node->value) + ">");
				}
				else {
					print("[" + std::to_string(node->value) + "]");
				}
			}
			cnt += 1;
			if (node->left_child != nullptr) {
				back_node_que_ptr->push(node->left_child);
			}
			if (node->right_child != nullptr) {
				back_node_que_ptr->push(node->right_child);
			}
		}
		cout << endl;
		tmp = cur_node_que_ptr;
		cur_node_que_ptr = back_node_que_ptr;
		back_node_que_ptr = tmp;
	}
	for (int i = 0; i < root->width * 4; i++) {
		cout << "-";
	}
	cout << endl;
}

int TreeValueCnt(Node* root){
	if(root == nullptr || root->is_leaf){
		return 0;
	}
	return TreeValueCnt(root->left_child) + TreeValueCnt((root->right_child)) + 1;
}

void FullBiaryTree(Node* root, int depth, int * idx) {
	if (root == nullptr) {
		return;
	}
	if (depth == 0) {
		return;
	}
	*idx += 1;
	Node * l = new Node(*idx);
	*idx += 1;
	Node * r = new Node(*idx);
	root->AddLeftChild(l);
	root->AddRightChild(r);
	FullBiaryTree(l, depth - 1, idx);
	FullBiaryTree(r, depth - 1, idx);
}

void InsertValueToSearchTree(Node*root, int value) {
	if (root == nullptr) {
		return;
	}
	if (value <= root->value) {
		if (root->left_child != nullptr) {
			InsertValueToSearchTree(root->left_child, value);
		}
		else {
			Node * node = new Node(value);
			root->AddLeftChild(node);
		}
	}
	else {
		if (root->right_child != nullptr) {
			InsertValueToSearchTree(root->right_child, value);
		}
		else {
			Node * node = new Node(value);
			root->AddRightChild(node);
		}
	}
}

void DelNodeByValue(Node* root, int value) {
	if (root == nullptr) {
		return;
	}
	//cout << "find " << root->value << endl;
	if (value == root->value) {
		if (root->left_child == nullptr && root->right_child == nullptr) {
			root->RemoveFromParent();
			delete root;
		}
		else if (root->left_child != nullptr) {
			Node * find_node = root->left_child;
			while (find_node->right_child != nullptr) {
				find_node = find_node->right_child;
			}
			root->value = find_node->value;
			Node * parent = find_node->parent;
			bool is_left_child = parent->left_child == find_node ? true : false;
			find_node->RemoveFromParent();
			Node * f_l = find_node->left_child;
			if (f_l != nullptr) {
				f_l->RemoveFromParent();
				if (is_left_child) {
					parent->AddLeftChild(f_l);
				}
				else {
					parent->AddRightChild(f_l);
				}
			}
			delete find_node;
		}
		else {
			Node * find_node = root->right_child;
			while (find_node->left_child != nullptr) {
				find_node = find_node->left_child;
			}
			root->value = find_node->value;
			Node * parent = find_node->parent;
			bool is_left_child = parent->left_child == find_node ? true : false;
			find_node->RemoveFromParent();
			Node * f_r = find_node->right_child;
			if (f_r != nullptr) {
				f_r->RemoveFromParent();
				if (is_left_child) {
					parent->AddLeftChild(f_r);
				}
				else {
					parent->AddRightChild(f_r);
				}
			}
			delete find_node;
		}
	}
	else if (value < root->value) {
		DelNodeByValue(root->left_child, value);
	}
	else {
		DelNodeByValue(root->right_child, value);
	}
}

void BinarySearchTreeTest() {
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

Node * BRTreeFind(Node * root, int val){
	Node * find_node = root;
	while (find_node != nullptr) {
		if (val < find_node->value) {
			if (find_node->left_child->is_leaf) {
				return nullptr;
			}
			find_node = find_node->left_child;
		}
		else if (val > find_node->value) {
			if (find_node->right_child->is_leaf) {
				return nullptr;
			}
			find_node = find_node->right_child;
		}
		else {
			break;
		}
	}
	if (find_node == nullptr || find_node->is_leaf){
		return nullptr;
	}
	return find_node;
}

Node * RepairInsertTree(Node * node) {
	// escape r, r
	assert(!node->IsBalck());
	Node * parent = node->parent;
	Node * uncle = node->UncleNode();
	Node * grand_parent = parent->parent;
	//cout << node->value << " " << parent->value << " " << uncle->value<< " " << grand_parent->value<< endl;
	if (parent->IsBalck()) {
		return nullptr;
	}
	// r, r, b
	if(uncle->IsBalck()){
		// r, r, b, b
		Node * tmp;
		if(node->ImRightNode() && parent->ImLeftNode()){
			parent->LeftRotation();
			tmp = parent;
			parent= node;
			node = tmp;
		}else if (node->ImLeftNode() && parent->ImRightNode()) {
			parent->RightRotation();
			tmp = parent;
			parent= node;
			node = tmp;
		}
		bool is_left = node->ImLeftNode();
		if((node->ImLeftNode() && parent->ImLeftNode()) || (node->ImRightNode() && parent->ImRightNode()))
		{
			parent->MakeBlack();
			grand_parent->MakeRed();
			if(is_left){
				return grand_parent->RightRotation();
			}else{
				return grand_parent->LeftRotation();;
			}
		}
		assert(true);
		return nullptr;
	}else{
		// r r b r
		parent->MakeBlack();
		uncle->MakeBlack();
		if(grand_parent->IsRoot()){
			return grand_parent;
		}
		grand_parent->MakeRed();
		return RepairInsertTree(grand_parent);
	}
}

Node * BRTreeInsert(Node * root, int val) {
	Node * node = new Node(val);
	node->MakeRed();
	node->CreateLeaf();
	if (root == nullptr) {
		node->MakeBlack();
		return node;
	}
	Node * find_node = root;
	while (find_node != nullptr) {
		if (find_node->value > val && find_node->HasValueLeftChild()) {
			find_node = find_node->left_child;
		}
		else if (find_node->value < val && find_node->HasValueRightChild()) {
			find_node = find_node->right_child;
		}
		else {
			break;
		}
	}
	if (find_node->value > val) {
		find_node->AddLeftChild(node);
	}else if(find_node->value < val) {
		find_node->AddRightChild(node);
	}else {
		find_node->value = val;
		return nullptr;
	}
	if (find_node->IsBalck()) {
		// insert to balck, include inser to root
		return nullptr;
	}
	Node * parent = node->parent;
	Node * grand_parent = node->parent->parent;
	Node * uncle = node->UncleNode();
	if(uncle->is_leaf){
		//cout << "----" << endl;
		grand_parent->MakeRed();
		if(node->ImLeftNode() && parent->ImLeftNode()){
			// gp -> right rot 
			parent->MakeBlack();
			return grand_parent->RightRotation();
		}else if (node->ImRightNode() && parent->ImLeftNode()){
			// gp -> l up rot
			node->MakeBlack();
			return grand_parent->UpRotationLRSon();
		}else if (node->ImRightNode() && parent->ImRightNode()){
			// gp -> left rot
			parent->MakeBlack();
			return grand_parent->LeftRotation();
		}else{
			// gp -> r up rot
			node->MakeBlack();
			return grand_parent->UpRotationRLSon();
		}
	}
	if(!uncle->IsBalck()){
		parent->MakeBlack();
		uncle->MakeBlack();
		if(grand_parent->IsRoot()){
			return nullptr;
		}
		grand_parent->MakeRed();
		return RepairInsertTree(grand_parent);
	}
	assert(true);
}

Node * RepairRemoveTree(Node * node) {
	if (node->IsRoot()) {
		return node;
	}
	Node * brother = node->Brother();
	if (node->parent->IsBalck()) {
		if (node->Brother()->IsBalck()) {
			// b(-1) b b, ���Һڸ���ͬ,�����ƺڽڵ�(n-1)
			if (brother->left_child->IsBalck() && brother->right_child->IsBalck()) {
				// b(-1) b b (b, b)
				node->Brother()->MakeRed();
				return RepairRemoveTree(node->parent);
			}
			else if (brother->left_child->IsBalck()){
				// b(-1) b b (b, r)
				Node * local_root = nullptr;
				if (node->ImLeftNode()) {
					local_root = node->parent->LeftRotation();
					brother->right_child->MakeBlack();
				}
				else {
					local_root = node->parent->RightRotation();
					brother->left_child->MakeBlack();
				}
				return local_root;
			}
			else {
				//b(-1) b b (r, r/b)
				// ����
				Node * local_root = nullptr;
				if (node->ImLeftNode()) {
					local_root = node->Brother()->left_child;
					node->parent->UpRotationRLSon();
				}
				else {
					local_root = node->Brother()->right_child;
					node->parent->UpRotationLRSon();
				}
				local_root->MakeBlack();
				return local_root;
			}
		}
		else {
			// b(-1) b r
			node->Brother()->MakeBlack();
			Node * b_l_l = brother->left_child->left_child;
			Node * b_l_r = brother->left_child->right_child;
			Node * b_r_l = brother->right_child->left_child;
			Node * b_r_r = brother->right_child->right_child;
			Node * local_parent = brother;
			Node * new_target = nullptr;
			Node * n_l = nullptr;
			Node * n_r = nullptr;
			if (node->ImLeftNode()) {
				node->parent->LeftRotation();
				if(b_l_l->IsBalck() && b_l_r->IsBalck()){
					node->Brother()->left_child->MakeRed();
				}else {
					new_target = b_r_l;
					n_l = new_target->left_child;
					n_r = new_target->right_child;
					if(!n_r->IsBalck()){
						local_parent->left_child->LeftRotation();
						n_r->MakeBlack();
					}else {
						new_target->RightRotation();
						new_target->MakeRed();
						n_l->MakeBlack();
					}
				}
			}
			else {
				node->parent->RightRotation();
				if(b_l_l->IsBalck() && b_l_r->IsBalck()){
					node->Brother()->right_child->MakeRed();
				}else {
					new_target = b_l_r;
					n_l = new_target->left_child;
					n_r = new_target->right_child;
					if(!n_l->IsBalck()){
						local_parent->right_child->RightRotation();
						n_l->MakeBlack();
					}else {
						new_target->LeftRotation();
						new_target->MakeRed();
						n_r->MakeBlack();
					}
				}
			}
			return local_parent;
		}
	}
	else {
		// b(-1) r b
		if (node->ImLeftNode()) {
			return node->parent->LeftRotation();
		}
		else {
			return node->parent->RightRotation();
		}
	}
}

Node * UpRotation(Node * root, bool is_left) {
	// ɾ�����γ� l l l, r r r �������� l c r
	//assert(root != nullptr && !root->left_child->is_leaf && !root->right_child->is_leaf);
	Node * parent = root->parent;
	bool root_is_left = root->ImLeftNode();
	Node * l = root->left_child;
	Node * ll = root->left_child->left_child;
	Node * lr = root->left_child->right_child;
	Node * r = root->right_child;
	Node * rl = root->right_child->left_child;
	Node * rr = root->right_child->right_child;
	Node * local_root = nullptr;
	l->RemoveFromParent();
	root->RemoveFromParent();
	r->RemoveFromParent();
	if (is_left) {
		delete l;
		rl->RemoveFromParent();
		local_root = rl;
		local_root->AddLeftChild(root);
		local_root->AddRightChild(r);
	}
	else {
		delete r;
		lr->RemoveFromParent();
		local_root = lr;
		local_root->AddLeftChild(l);
		local_root->AddRightChild(root);
	}
	if (parent != nullptr) {
		if (root_is_left) {
			parent->AddLeftChild(local_root);
		}
		else {
			parent->AddRightChild(local_root);
		}
	}
	local_root->CreateLeaf();
	local_root->left_child->CreateLeaf();
	local_root->right_child->CreateLeaf();
	return local_root;
}

Node * BRTreeRemove(Node * root, int val) {
	if (root == nullptr) {
		return root;
	}
	Node * find_node = BRTreeFind(root, val);
	if (!find_node->left_child->is_leaf && !find_node->right_child->is_leaf) {
		Node * del_node = find_node;
		// # pre link node
		find_node = find_node->left_child;
		while (!find_node->right_child->is_leaf) {
			find_node = find_node->right_child;
		}
		cout << "find node value : " << find_node->value << endl;
		del_node->value = find_node->value;
	}
	// can make delete node just has one child
	Node * parent = find_node->parent;
	if (find_node->IsRoot()) {
		// just has one child, delete root , make child be root
		if (!find_node->left_child->is_leaf) {
			find_node->value = find_node->left_child->value;
			find_node->left_child->RemoveFromParent();
			delete find_node->left_child;
			find_node->CreateLeaf();
			return root;
		}
		else if (!find_node->right_child->is_leaf) {
			find_node->value = find_node->right_child->value;
			find_node->right_child->RemoveFromParent();
			delete find_node->right_child;
			find_node->CreateLeaf();
			return root;
		}
		// delete root and not has any child
		delete find_node;
		return nullptr;
	}
	if (!find_node->IsBalck()) {
		// find_node is red, just delete and make it's child replace this
		bool is_left_child = find_node->ImLeftNode();
		if (find_node->left_child->is_leaf && find_node->right_child->is_leaf) {
			find_node->RemoveFromParent();
			delete find_node;
			return root;
		}else {
			// find not is red and it just has one child, not exist (r b, not blance), (r, r not regular) 
			assert(true);
		}
		assert(true);
	}
	else {
		// find node is balck and just has one red child or not has child 
		Node * l = find_node->left_child;
		Node * r = find_node->right_child;
		bool has_two_child = bool(!l->is_leaf && !r->is_leaf);
		assert(!has_two_child);
		if(!(l->is_leaf && r->is_leaf)){
			if (!l->is_leaf) {
				find_node->value = l->value;
				l->RemoveFromParent();
				delete l;
				return root;
			}else{
				find_node->value = r->value;
				r->RemoveFromParent();
				delete r;
				return root;
			}
		} else {
			// find not is black and , find not not has child
			Node * brother = find_node->Brother();
			bool is_left = find_node->ImLeftNode();
			find_node->RemoveFromParent();
			delete find_node;
			Node * local_root = nullptr;
			// b - r - ?
			bool l_l = is_left && !brother->right_child->is_leaf;
			bool r_r = !is_left && !brother->left_child->is_leaf;
			if (!parent->IsBalck()) { // parent is red b - r  (black height 1)
				if (brother->left_child->is_leaf && brother->right_child->is_leaf) {
					// b - r - b
					parent->MakeBlack();
					brother->MakeRed();
					return root;
				}
				else if (l_l || r_r || (!brother->left_child->is_leaf && !brother->right_child->is_leaf)) {
					// b - r <<r> b <r>>   / b - r <b<r>>  
					local_root = brother;
					if (is_left) {
						parent->LeftRotation();
					}
					else {
						parent->RightRotation();
					}
					local_root->left_child->MakeBlack();
					local_root->right_child->MakeBlack();
					if(!local_root->IsRoot()){
						// keep black height 1
						local_root->MakeRed();
						return root;
					}
					return local_root;
				}
				else {
					local_root = UpRotation(parent, is_left);
					local_root->left_child->MakeBlack();
					local_root->right_child->MakeBlack();
					if(local_root->IsRoot()){
						local_root->MakeBlack();
						return local_root;
					}
					return root;
				}
			}
			else {
				bool l_l = is_left && brother->left_child->is_leaf;
				bool r_r = !is_left && brother->right_child->is_leaf;
				// b - b - r  
				if (!brother->IsBalck()) {
					// b - b - r (b, b)
					Node* b_l_l = brother->left_child->left_child;
					Node* b_l_r = brother->left_child->right_child;
					Node* b_r_l = brother->right_child->left_child;
					Node* b_r_r = brother->right_child->right_child;
					Node * ori_parent = parent;
					local_root = brother;
					local_root->MakeBlack();
					if (is_left) {
						parent->LeftRotation();
					}
					else {
						parent->RightRotation();
					}
					if(is_left && (!b_l_l->is_leaf || !b_l_r->is_leaf)){
						local_root->left_child->LeftRotation();
						// b < <<r>b<r>> r <<r>b><r>>
						if(b_l_l->is_leaf){
							local_root->left_child->left_child->MakeRed();
						}else{
							UpRotation(b_l_l->parent->parent, b_l_l->ImLeftNode());
						}
					}else if(!is_left && (!b_r_l->is_leaf || !b_r_r->is_leaf)){
						// b < <<r>b<r>> r <<r>b><r>>
						local_root->right_child->RightRotation();
						if(b_r_r->is_leaf){
							local_root->right_child->right_child->MakeRed();
						}else {
							UpRotation(b_r_r->parent->parent, b_r_r->ImLeftNode());
						}
					}else{
						// <b> b <<b>r<b>>
						if (is_left){
							local_root->left_child->right_child->MakeRed();
						}else{
							local_root->right_child->left_child->MakeRed();
						}
					}
					//assert(true);
				}else if (brother->left_child->is_leaf && brother->right_child->is_leaf) {
					// <b> b <b>
					brother->MakeRed();
					local_root = RepairRemoveTree(parent);
				}
				else if (l_l || r_r || (!brother->left_child->is_leaf && !brother->right_child->is_leaf)) {
					// <b> b <r> b <r>
					local_root = brother;
					if (is_left) {
						parent->LeftRotation();
					}
					else {
						parent->RightRotation();
					}
					local_root->left_child->MakeBlack();
					local_root->right_child->MakeBlack();
				}
				else {
					// <b> b <r> b <r>
					local_root = UpRotation(parent, is_left);
					local_root->MakeBlack();
				}
				if (local_root != nullptr && local_root->parent == nullptr) {
					return local_root;
				}
				return root;
			}
		}
	}
	return root;
}

Node * CreateSimple7Tree(const int * value_list, int n) {
	vector<Node*> vec;
	for (int i = 0; i < n; i++) {
		int value = value_list[i];
		if (value < 0) {
			vec.push_back(nullptr);
			continue;
		}
		int idx = i + 1;
		Node * node = new Node(idx);
		node->CreateLeaf();
		vec.push_back(node);
		if (idx > 1) {
			bool is_left = idx / 2 * 2 == idx;
			int parent_idx = idx / 2 - 1;
			Node * parent = vec[parent_idx];
			if (parent == nullptr) {
				continue;
			}
			if (!parent->is_leaf) {
				if (is_left) {
					parent->AddLeftChild(node);
				}
				else {
					parent->AddRightChild(node);
				}
			}
		}
	}
	return vec.size() > 0 ? vec[0]  : nullptr;
}

void UpRotationTest() {
	int valuse_list[7] = { 1, 2, 3, -1, 5, -1, -1};
	Node *  root = CreateSimple7Tree(valuse_list, 7);
	Node * rr = new Node(0);
	rr->AddLeftChild(root);
	PrintTree(rr);
	Node * n_root = UpRotation(root, false);
	if (n_root != nullptr) {
		rr = n_root;
	}
	PrintTree(rr);
}

void FullLUpRotationTest() {
	int valuse_list[17] = { 1, 2, 3, -1, -1, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
	Node *  root = CreateSimple7Tree(valuse_list, 15);
	PrintTree(root);
	root = root->UpRotationRLSon();
	PrintTree(root);
}

void FullRUpRotationTest() {
	int valuse_list[17] = { 1, 2, 3, 4, 5, -1, -1, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
	Node *  root = CreateSimple7Tree(valuse_list, 15);
	PrintTree(root);
	root = root->UpRotationLRSon();
	PrintTree(root);
}

void FullRUpRotationTest2() {
	int valuse_list[17] = { 1, -1, 3, 4, 5, 6, -1, 8, 9, 10, -1, -1, -1, 14, 15, 16, 17 };
	Node *  root = CreateSimple7Tree(valuse_list, 15);
	PrintTree(root);
	root = root->UpRotationRLSon();
	PrintTree(root);
}

void FullLUpRotationTest2() {
	int valuse_list[17] = { 1, 2, -1, -1, 5, -1, -1, -1, -1, -1, -1, -1, -1, 14, 15, 16, 17 };
	Node *  root = CreateSimple7Tree(valuse_list, 15);
	PrintTree(root);
	root = root->UpRotationLRSon();
	PrintTree(root);
}

int RandomInt(int a, int b){
	return a + rand() % (b - a); 
}

void PrintCmd(vector<int> cmd){
	cout << "{";
	for(int i = 0; i < cmd.size(); i++){
		cout << cmd[i];
		if(i !=cmd.size() - 1){
			cout << ",";
		}
	}
	cout << "}" << endl;
}

Node * MonkeyTestCmd(Node * root, int opt, int value, bool & res, vector<int> & cmd_vec){
	if (opt == 0){
		Node * find_value = BRTreeFind(root, value);
		if (find_value != nullptr) {
			res = false;
			return root;
		}
		cout << " --- begin --- " << endl;
		cout << "insert : " << value << endl;
		cmd_vec.push_back(opt);
		cmd_vec.push_back(value);
		PrintCmd(cmd_vec);
		Node * n_root = BRTreeInsert(root, value);
		if (n_root != nullptr){
			root = n_root;
		}
	}else{
		Node * find_value = BRTreeFind(root, value);
		if (find_value == nullptr) {
			res = false;
			return root;
		}
		cout << " --- begin --- " << endl;
		cout << "remove : " << value << endl;
		cmd_vec.push_back(opt);
		cmd_vec.push_back(value);
		PrintCmd(cmd_vec);
		Node * n_root = BRTreeRemove(root, value);
		if (n_root != nullptr){
			root = n_root;
		}
	}
	//PrintTree(root);
	return root;
}

Node * MonkeyTestCmdTranslator(const int * cmd_list, int len){
	Node * root = nullptr;
	vector<int> vec;
	for(int i = 0 ; i < len / 2; i++){
		int opt = cmd_list[i*2];
		int value = cmd_list[i*2 + 1];
		bool opt_res;
		cout << opt << " " << value << endl;
		root = MonkeyTestCmd(root, opt, value, opt_res, vec);
	}
	return root;
}

vector<int> num_vec;
void Init_Vect(int max_num){
	num_vec.clear();
	for(int i = 0; i < max_num; i++){
		num_vec.push_back(i);
	}
}

int GetVecNum(int idx){
	assert(num_vec.size()>0);
	idx = idx % num_vec.size();
	int ret = num_vec[idx];
	num_vec.erase(num_vec.begin() + idx);
	return ret;
}

void MokeyTest(){
	vector<int> cmd;
	int test_cnt = 21;
	Node * root = nullptr;
	srand(time(nullptr));
	bool res = 1;
	int max_value = 10;
	Init_Vect(max_value);
	int opt_min = 0, opt_max=1; // insert
	for(int idx = 0; idx < test_cnt; idx++){
		int opt = RandomInt(opt_min, opt_max);
		int value_idx = RandomInt(0, max_value);
		int value = GetVecNum(value_idx);
		bool opt_res = true;
		root = MonkeyTestCmd(root, opt, value, opt_res, cmd);
		res = res && RBTreeCheckBlackHeight(root);
		if(!res){
			break;
		}
		if(max_value == TreeValueCnt(root)){
			cout << "to add >>>>" << endl;
			opt_min = 1;
			opt_max = 2;
			Init_Vect(max_value);
			cmd.clear();
		}else if(0 == TreeValueCnt(root)){
			cout << "to del >>>>" << endl;
			opt_min = 0;
			opt_max = 1;
			Init_Vect(max_value);
			cmd.clear();
		}
	}
	cout << res << endl;
}

void GetNumTest(){
	int max_n = 10;
	Init_Vect(max_n);
	srand(time(nullptr));
	for(int i = 0; i < 10; i++){
		int idx = RandomInt(0, max_n);
		cout << GetVecNum(idx) << endl;
	}
}

int main() {
	//FullRUpRotationTest2();
	//FullLUpRotationTest2();
	int cmd_list [] = {0,7,0,1,0,4,0,14,0,13,1,4,0,9,0,11,0,2,0,10,1,11,0,5,0,11,0,18,1,10,0,16,0,6,0,4,0,10,0,0,0,8,1,2,1,8,0,8,0,17,0,3,0,2,1,18,1,0};
	//Node * root = MonkeyTestCmdTranslator(cmd_list, sizeof(cmd_list) / sizeof(int));
	MokeyTest();
	//UpRotationTest();
	//FullLUpRotationTest();
	//FullRUpRotationTest();
	cout << "finished!" << endl;
	return 0;
}

