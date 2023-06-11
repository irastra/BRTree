#pragma once
#include "Tree.h"

Node * InsertValueToSearchTree(Node* root, int key_value) {
	if (root == nullptr) {
		Node* node = new Node(key_value);
		return node;
	}
	if (key_value <= root->key) {
		if (root->left_child != nullptr) {
			InsertValueToSearchTree(root->left_child, key_value);
		}
		else {
			Node* node = new Node(key_value);
			root->AddLeftChild(node);
		}
	}
	else {
		if (root->right_child != nullptr) {
			InsertValueToSearchTree(root->right_child, key_value);
		}
		else {
			Node* node = new Node(key_value);
			root->AddRightChild(node);
		}
	}
	return root;
}

Node* DelNodeFromSearchTree(Node* root, int key_value) {
	if (root == nullptr) {
		return nullptr;
	}
	if (key_value == root->key) {
		if (root->left_child == nullptr && root->right_child == nullptr) {
			root->RemoveFromParent();
			delete root;
			return nullptr;
		}
		else if (root->left_child != nullptr) {
			Node* find_node = root->left_child;
			while (find_node->right_child != nullptr) {
				find_node = find_node->right_child;
			}
			root->key = find_node->key;
			Node* parent = find_node->parent;
			bool is_left_child = parent->left_child == find_node ? true : false;
			find_node->RemoveFromParent();
			Node* f_l = find_node->left_child;
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
			Node* find_node = root->right_child;
			while (find_node->left_child != nullptr) {
				find_node = find_node->left_child;
			}
			root->key = find_node->key;
			Node* parent = find_node->parent;
			bool is_left_child = parent->left_child == find_node ? true : false;
			find_node->RemoveFromParent();
			Node* f_r = find_node->right_child;
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
		return root;
	}
	else if (key_value < root->key) {
		return DelNodeFromSearchTree(root->left_child, key_value);
	}
	else {
		return DelNodeFromSearchTree(root->right_child, key_value);
	}
}
