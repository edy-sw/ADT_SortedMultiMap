#pragma once

#include "SortedMultiMap.h"


class SMMIterator{
	friend class SortedMultiMap;
private:
	//DO NOT CHANGE THIS PART
	const SortedMultiMap& map;
	SMMIterator(const SortedMultiMap& map);

	SortedMultiMap::BSTNode** stack;
	int stackSize;
	int stackCapacity;
	SortedMultiMap::BSTNode* currentNode;
	int currentParamIndex;

	void push(SortedMultiMap::BSTNode* node);
	SortedMultiMap::BSTNode* pop();

public:
	void first();
	void next();
	bool valid() const;
   	TElem getCurrent() const;
	~SMMIterator();
};

