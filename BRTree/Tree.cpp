#include "stdafx.h"
#include "Tree.h"

int _RBTreeCheckBlackHeight(Node* root, bool& valid) {
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

bool RBTreeCheckFunc(Node* root) {
	bool ret = true;
	int black_heigt = _RBTreeCheckBlackHeight(root, ret);
	return ret;
}

void RefreshNodePosition(Node* node, int** last_value) {
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
	//cout << node->key << " " << node->order_idx << endl;
	*last_value = &node->order_idx;
	if (node->right_child != nullptr) {
		RefreshNodePosition(node->right_child, last_value);
		node->width += node->right_child->width;
		if (node->right_child->height + 1 > node->height) {
			node->height = node->right_child->height + 1;
		}
	}
}


void RefreshTree(Node* root) {
	int* ptr = nullptr;
	RefreshNodePosition(root, &ptr);
}

bool SearchTreeCheckFunc(Node* root) {
	return true;
}

void PrintTree(Node* node) {
	Node* root = node;
	bool valid = RBTreeCheckFunc(root);
	if (!valid) {
		cout << " ============== invalid =============" << endl;
		//return;
	}
	list<Node*> node_que1, node_que2;
	list<Node*>* cur_node_list_ptr = &node_que1, * back_node_list_ptr = &node_que2;
	list<Node*>* tmp = nullptr;
	int lidx = 0, ridx = 0;
	if (node == nullptr) {
		cout << " [Null] !" << endl;
		return;
	}
	RefreshTree(node);
	cur_node_list_ptr->push_back(node);
	while (!cur_node_list_ptr->empty()) {
		int cnt = 0;
		while (!cur_node_list_ptr->empty()) {
			node = cur_node_list_ptr->front();
			cur_node_list_ptr->pop_front();
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
					print("<" + std::to_string(node->key) + ">");
				}
				else {
					print("[" + std::to_string(node->key) + "]");
				}
			}
			cnt += 1;
			if (node->left_child != nullptr) {
				back_node_list_ptr->push_back(node->left_child);
			}
			if (node->right_child != nullptr) {
				back_node_list_ptr->push_back(node->right_child);
			}
		}
		cout << endl;
		int section_print_cnt = 0;
		std::list<Node*>::iterator it = back_node_list_ptr->begin();
		for (; it != back_node_list_ptr->end(); it++) {
			Node* cur_node = *it;
			lidx = cur_node->order_idx;
			ridx = cur_node->Brother()->order_idx - 1;
			for (; section_print_cnt < lidx; section_print_cnt++) {
				print_fill(" ");
			}
			for (; section_print_cnt < ridx; section_print_cnt++) {
				print_fill("-");
			}
		}
		cout << endl;
		tmp = cur_node_list_ptr;
		cur_node_list_ptr = back_node_list_ptr;
		back_node_list_ptr = tmp;
	}
	for (int i = 0; i < root->width * 4; i++) {
		cout << "-";
	}
	cout << endl;
}


int TreeValueCnt(Node* root) {
	if (root == nullptr || root->is_leaf) {
		return 0;
	}
	return TreeValueCnt(root->left_child) + TreeValueCnt((root->right_child)) + 1;
}
