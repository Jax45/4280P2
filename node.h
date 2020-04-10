//node.h this is to be used in the binary search tree.
//it is a simple struct with a pointer to a left, and right node.
//as well as a key of the node's word.
#ifndef NODE_H
#define NODE_H
#include "fsaTable.h"
struct Node {
	struct Token *token = NULL;
	struct Node *first, *second, *third, *fourth, *fifth, *sixth;		
};


#endif
