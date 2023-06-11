#include "stdafx.h"
#include "Tree.h"
#include <algorithm>
#include <math.h>
using namespace std;

Node* BlanceTreeFind(Node* root, int val, Node** ret_parent) {
	*ret_parent = nullptr;
	Node* find_node = root;
	while (find_node != nullptr) {
		if (find_node->key == val) {
			break;
		}
		else if (find_node->key > val) {
			*ret_parent = find_node;
			find_node = find_node->left_child;
		}
		else {
			*ret_parent = find_node;
			find_node = find_node->right_child;
		}
	}
	return find_node;
}

int BlanceTreeHeight(Node* node) {
	if (node == nullptr) {
		return 0;
	}
	return node->height;
}

int RefreshBlanceTreeHeight(Node* root, int height_step) {
	if (height_step <= 0) {
		return BlanceTreeHeight(root);
	}
	int l = RefreshBlanceTreeHeight(root->left_child, height_step - 1);
	int r = RefreshBlanceTreeHeight(root->right_child, height_step - 1);
	return max(l, r) + 1;
}

Node* RepairBlanceTree(Node* node) {
	if (node == nullptr) {
		return node;
	}
	Node* parent = node->parent;
	Node* brother = node->Brother();
	bool is_left = node->ImLeftNode();
	int node_height = BlanceTreeHeight(node);
	int brother_height = BlanceTreeHeight(brother);
	int blance_factor = node_height - brother_height;
	if (parent == nullptr) {
		return node;
	}
	int old_parent_height = parent->height;
	int parent_hieght = 1 + max(node_height, brother_height);
	if (fabs(blance_factor) <= 1) {
		if (parent_hieght == old_parent_height) {
			return node;
		}
		else {
			parent->height = parent_hieght;
			return RepairBlanceTree(parent);
		}
	}
	Node* new_parent = nullptr;
	if (brother == nullptr) {
		if (is_left && node->left_child != nullptr) {
			new_parent = parent->RightRotation();
		}
		else if (!is_left && node->right_child != nullptr) {
			new_parent = parent->LeftRotation();
		}
		else if (is_left && node->left_child == nullptr) {
			new_parent = parent->UpRotationLRSon();
		}
		else if (!is_left && node->right_child == nullptr) {
			new_parent = parent->UpRotationRLSon();
		}
		RefreshBlanceTreeHeight(new_parent, 2);
		return new_parent;
	}
	if (blance_factor > 0) {
		return RepairBlanceTree(brother);
	}
	int b_blance_factor = BlanceTreeHeight(brother->left_child);
	if (is_left) {
		if (b_blance_factor <= 0) {
			new_parent = parent->LeftRotation();
			RefreshBlanceTreeHeight(new_parent, 2);
			if (b_blance_factor < 0) {
				new_parent = RepairBlanceTree(parent);
			}
		}
		else {
			new_parent = parent->UpRotationRLSon();
			RefreshBlanceTreeHeight(new_parent, 2);
			new_parent = RepairBlanceTree(parent);
			return new_parent;
		}
	}
	else {
		if (b_blance_factor <= 0) {
			new_parent = parent->RightRotation();
			RefreshBlanceTreeHeight(new_parent, 2);
			if (b_blance_factor < 0) {
				new_parent = RepairBlanceTree(parent);
			}
		}
		else {
			new_parent = parent->UpRotationLRSon();
			RefreshBlanceTreeHeight(new_parent, 2);
			new_parent = RepairBlanceTree(parent);
			return new_parent;
		}
	}
	return nullptr;
}

Node* BlanceTreeInsert(Node* root, int val) {
	Node* new_node = new Node(val);
	new_node->height = 1;
	if (root == nullptr) {
		return new_node;
	}
	Node* find_node;
	Node** insert_parent = nullptr;
	find_node = BlanceTreeFind(root, val, insert_parent);
	assert(find_node == nullptr);
	find_node = *insert_parent;
	if (val < find_node->key) {
		find_node->AddLeftChild(new_node);
	}
	else {
		find_node->AddRightChild(new_node);
	}
	Node* new_root = RepairBlanceTree(new_node);
	return new_root;
}