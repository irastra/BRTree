// BRTree.cpp : black red tree
#include "stdafx.h"
#include <iostream>
#include "Tree.h"
#include "RedBlackTree.h"
#include "SearchTree.h"
#include "BlanceTree.h"
#include "Test.h"

using namespace std;

int main() {
	int cmd_list[] = { 0,8,0,13,0,10,0,9,0,12,0,0,0,6,0,7,0,11,0,14,0,2,0,1,0,3,0,4,0,5,1,8 };
	//Node* root = MonkeyTestCmdTranslator(cmd_list, sizeof(cmd_list) / sizeof(int), InsertValueToSearchTree, DelNodeFromSearchTree);
	//Node * root = MonkeyTestCmdTranslator(cmd_list, sizeof(cmd_list) / sizeof(int), BRTreeInsert, BRTreeRemove);
	MokeyTest(100000, 100, BRTreeInsert, BRTreeRemove);
	//Node* root = nullptr;
	//root = BlanceTreeInsert(root, 1);
	//PrintTree(root);
	cout << "finished!" << endl;
	return 0;
}

