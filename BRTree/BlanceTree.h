#pragma once
#include "stdafx.h"
#include "Tree.h"
#include <algorithm>
#include <math.h>

Node* BlanceTreeFind(Node* root, int val, Node** ret_parent);

int BlanceTreeHeight(Node* node);

int RefreshBlanceTreeHeight(Node* root, int height_step);

Node* RepairBlanceTree(Node* node);

Node* BlanceTreeInsert(Node* root, int val);

bool BlanceTreeCheckFunc(Node* root);