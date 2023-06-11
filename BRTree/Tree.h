#pragma once
#include <assert.h>
#include <iostream>
#include <list>
#include <string>
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
	Node* left_child;
	Node* right_child;
	Node* parent;
	bool is_leaf;
	int height;
	int order_idx;
	int width;
	int key;
	Node(int key_value) {
		init();
		this->key = key_value;
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
		key = -1;
	}

	bool IsRoot() {
		return parent == nullptr;
	}

	bool HasSon() {
		Node* ll = left_child->left_child;
		Node* lr = left_child->right_child;
		Node* rl = right_child->left_child;
		Node* rr = right_child->right_child;
		return (ll != nullptr || lr != nullptr || rl != nullptr || rr != nullptr);
	}

	void CreateLeaf() {
		assert(!is_leaf);
		if (left_child == nullptr) {
			Node* left = new Node(-1);
			left->node_color = NodeColor::BLACK;
			left->is_leaf = true;
			left->parent = this;
			left_child = left;
		}
		if (right_child == nullptr) {
			Node* right = new Node(-1);
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

	void AddLeftChild(Node* node) {
		assert(left_child == nullptr || left_child->is_leaf);
		assert(node->parent == nullptr);
		if (left_child != nullptr && left_child->is_leaf) {
			left_child->RemoveFromParent();
			delete left_child;
		}
		left_child = node;
		node->parent = this;
	}

	void AddRightChild(Node* node) {
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

	Node* UncleNode() {
		if (parent == nullptr || parent->parent == nullptr) {
			return nullptr;
		}
		return parent->ImLeftNode() ? parent->parent->right_child : parent->parent->left_child;
	}

	Node* Brother() {
		if (parent == nullptr) {
			return nullptr;
		}
		return ImLeftNode() ? parent->right_child : parent->left_child;
	}

	bool IsBalck() {
		return node_color == NodeColor::BLACK;
	}

	Node* RightRotation() {
		Node* _parent = parent;
		Node* lr = left_child->right_child;
		Node* l = left_child;
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

	Node* LeftRotation() {
		Node* _parent = parent;
		Node* rl = right_child->left_child;
		Node* r = right_child;
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

	Node* UpRotationRLSon() {
		bool is_left = ImLeftNode();
		Node* _parent = parent;
		Node* _root = this;
		Node* r = right_child;
		Node* r_son = r->left_child;
		Node* r_son_l = r_son->left_child;
		Node* r_son_r = r_son->right_child;
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

	Node* UpRotationLRSon() {
		bool is_left = ImLeftNode();
		Node* _parent = parent;
		Node* _root = this;
		Node* l = left_child;
		Node* l_son = l->right_child;
		Node* l_son_l = l_son->left_child;
		Node* l_son_r = l_son->right_child;
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

	Node* UpRotation(bool is_left) {
		Node* l = left_child;
		Node* r = right_child;
		Node* r_l = r->left_child;
		Node* l_r = l->right_child;
		if (is_left) {
			UpRotationRLSon();
			return r_l;
		}
		else {
			UpRotationLRSon();
			return l_r;
		}
	}
};

int _RBTreeCheckBlackHeight(Node* root, bool& valid);

bool RBTreeCheckBlackHeight(Node* root);

void RefreshNodePosition(Node* node, int** last_value);

void RefreshTree(Node* root);

void PrintTree(Node* node);

int TreeValueCnt(Node* root);