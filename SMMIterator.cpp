#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <exception>

// Best Case: Theta(1) (empty tree)
// Worst Case: Theta(n) (where n is the number of nodes, pushing left path)
// Total Complexity: O(h) or O(n) worst case
SMMIterator::SMMIterator(const SortedMultiMap& d) : map(d){
	stackSize = 0;
	stackCapacity = 10;
	stack = new SortedMultiMap::BSTNode*[stackCapacity];
	currentNode = nullptr;
	currentParamIndex = 0;
	first();
}

// Best Case: Theta(1) (stack has space)
// Worst Case: Theta(k) (reallocation of stack of capacity k)
// Total Complexity: amortized Theta(1), worst O(h)
void SMMIterator::push(SortedMultiMap::BSTNode* node) {
	if (stackSize == stackCapacity) {
		stackCapacity *= 2;
		auto newStack = new SortedMultiMap::BSTNode*[stackCapacity];
		for (int i = 0; i < stackSize; ++i) {
			newStack[i] = stack[i];
		}
		delete[] stack;
		stack = newStack;
	}
	stack[stackSize++] = node;
}

// Best Case: Theta(1)
// Worst Case: Theta(1)
// Total Complexity: Theta(1)
SortedMultiMap::BSTNode* SMMIterator::pop() {
	if (stackSize == 0) return nullptr;
	return stack[--stackSize];
}

// Best Case: Theta(1) (empty tree)
// Worst Case: Theta(n) (degenerate tree pushing all nodes to left)
// Total Complexity: O(h) or O(n) worst case
void SMMIterator::first(){
	stackSize = 0;
	SortedMultiMap::BSTNode* node = map.root;
	while (node != nullptr) {
		push(node);
		node = node->left;
	}
	if (stackSize > 0) {
		currentNode = stack[stackSize - 1];
	} else {
		currentNode = nullptr;
	}
	currentParamIndex = 0;
}

// Best Case: Theta(1) (node has more array values or node just pops and has no right child)
// Worst Case: Theta(n) (node pops, has right child, and pushes left track from there)
// Total Complexity: amortized O(1), worst O(h) or O(n)
void SMMIterator::next(){
	if (!valid()) {
		throw std::exception();
	}
	currentParamIndex++;
	if (currentParamIndex >= currentNode->numValues) {
		SortedMultiMap::BSTNode* node = pop();
		if (node->right != nullptr) {
			node = node->right;
			while (node != nullptr) {
				push(node);
				node = node->left;
			}
		}
		if (stackSize > 0) {
			currentNode = stack[stackSize - 1];
		} else {
			currentNode = nullptr;
		}
		currentParamIndex = 0;
	}
}

// Best Case: Theta(1)
// Worst Case: Theta(1)
// Total Complexity: Theta(1)
bool SMMIterator::valid() const{
	return currentNode != nullptr;
}

// Best Case: Theta(1)
// Worst Case: Theta(1)
// Total Complexity: Theta(1)
TElem SMMIterator::getCurrent() const{
	if (!valid()) {
		throw std::exception();
	}
	return TElem(currentNode->key, currentNode->values[currentParamIndex]);
}

// Best Case: Theta(1)
// Worst Case: Theta(1)
// Total Complexity: Theta(1)
SMMIterator::~SMMIterator() {
	delete[] stack;
}


