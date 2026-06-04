#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <iostream>
#include <vector>
#include <exception>
using namespace std;

// Best Case: Theta(1)
// Worst Case: Theta(1)
// Total Complexity: Theta(1)
SortedMultiMap::SortedMultiMap(Relation r) {
	root = nullptr;
	numElements = 0;
	rel = r;
}

// Best Case: Theta(1)
// Worst Case: Theta(1)
// Total Complexity: Theta(1)
SortedMultiMap::BSTNode* SortedMultiMap::createNode(TKey c, TValue v) {
	BSTNode* node = new BSTNode;
	node->key = c;
	node->capacity = 5;
	node->numValues = 1;
	node->values = new TValue[node->capacity];
	node->values[0] = v;
	node->left = nullptr;
	node->right = nullptr;
	return node;
}

// Best Case: Theta(1)
// Worst Case: Theta(1)
// Total Complexity: Theta(1)
void SortedMultiMap::destroyNode(BSTNode* node) {
	if (node) {
		delete[] node->values;
		delete node;
	}
}

// Best Case: Theta(1)
// Worst Case: Theta(n)
// Total Complexity: O(n)
void SortedMultiMap::destroyTree(BSTNode* node) {
	if (node) {
		destroyTree(node->left);
		destroyTree(node->right);
		destroyNode(node);
	}
}

// Best Case: Theta(1) 
// Worst Case: Theta(n + k)
// Total Complexity: O(n + k)
bool SortedMultiMap::removeNode(BSTNode* node, BSTNode* parent) {
	if (node->left == nullptr || node->right == nullptr) {
		BSTNode* replacement = (node->left != nullptr) ? node->left : node->right;
		if (node == root) {
			root = replacement;
		} else if (parent->left == node) {
			parent->left = replacement;
		} else {
			parent->right = replacement;
		}
		destroyNode(node);
	} else {

		BSTNode* succ = node->right;
		BSTNode* succParent = node;
		while (succ->left != nullptr) {
			succParent = succ;
			succ = succ->left;
		}

		node->key = succ->key;
		delete[] node->values;
		node->capacity = succ->capacity;
		node->numValues = succ->numValues;
		node->values = new TValue[node->capacity];
		for (int i = 0; i < node->numValues; i++) {
			node->values[i] = succ->values[i];
		}

		if (succParent->left == succ) {
			succParent->left = succ->right;
		} else {
			succParent->right = succ->right;
		}
		destroyNode(succ);
	}
	return true;
}

// Best Case: Theta(1) (empty tree)
// Worst Case: Theta(n + k) (degenerate tree where U is unique nodes + array reallocation)
// Total Complexity: O(n + k)
void SortedMultiMap::add(TKey c, TValue v) {
	if (root == nullptr) {
		root = createNode(c, v);
		numElements++;
		return;
	}

	BSTNode* current = root;
	BSTNode* parent = nullptr;
	bool isLeft = false;

	while (current != nullptr) {
		if (current->key == c) {
			if (current->numValues == current->capacity) {
				current->capacity *= 2;
				TValue* newValues = new TValue[current->capacity];
				for (int i = 0; i < current->numValues; ++i) {
					newValues[i] = current->values[i];
				}
				delete[] current->values;
				current->values = newValues;
			}
			current->values[current->numValues++] = v;
			numElements++;
			return;
		}

		parent = current;
		if (rel(c, current->key)) {
			current = current->left;
			isLeft = true;
		} else {
			current = current->right;
			isLeft = false;
		}
	}

	BSTNode* newNode = createNode(c, v);
	if (isLeft) {
		parent->left = newNode;
	} else {
		parent->right = newNode;
	}
	numElements++;
}

// Best Case: Theta(1) (key found at root)
// Worst Case: Theta(n + k) (degenerate tree where U is unique nodes and k is number of values to copy)
// Total Complexity: O(n + k)
vector<TValue> SortedMultiMap::search(TKey c) const {
	BSTNode* current = root;
	while (current != nullptr) {
		if (current->key == c) {
			vector<TValue> res;
			for (int i = 0; i < current->numValues; ++i) {
				res.push_back(current->values[i]);
			}
			return res;
		}
		if (rel(c, current->key)) {
			current = current->left;
		} else {
			current = current->right;
		}
	}
	return vector<TValue>();
}

// Best Case: Theta(1) (key found at root, element found at beginning of values)
// Worst Case: Theta(n + k) (degenerate tree where U is unique nodes, shifting array of k elements)
// Total Complexity: O(n + k)
bool SortedMultiMap::remove(TKey c, TValue v) {
	BSTNode* current = root;
	BSTNode* parent = nullptr;

	while (current != nullptr) {
		if (current->key == c) {
			for (int i = 0; i < current->numValues; ++i) {
				if (current->values[i] == v) {
					for (int j = i; j < current->numValues - 1; ++j) {
						current->values[j] = current->values[j+1];
					}
					current->numValues--;
					numElements--;

					if (current->numValues == 0) {
						removeNode(current, parent);
					}
					return true;
				}
			}
			return false;
		}
		parent = current;
		if (rel(c, current->key)) {
			current = current->left;
		} else {
			current = current->right;
		}
	}
	return false;
}

// Best Case: Theta(1)
// Worst Case: Theta(1)
// Total Complexity: Theta(1)
int SortedMultiMap::size() const {
	return numElements;
}

// Best Case: Theta(1)
// Worst Case: Theta(1)
// Total Complexity: Theta(1)
bool SortedMultiMap::isEmpty() const {
	return numElements == 0;
}

// Best Case: Theta(1)
// Worst Case: Theta(1)
// Total Complexity: Theta(1)
SMMIterator SortedMultiMap::iterator() const {
	return SMMIterator(*this);
}

// Best Case: Theta(1) 
// Worst Case: Theta(n)
// Total Complexity: O(n)
SortedMultiMap::~SortedMultiMap() {
	destroyTree(root);
}
