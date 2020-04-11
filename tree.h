//tree.h
#ifndef TREE_H
#define TREE_H
#include <stdbool.h>
#include "node.h"
//function prototypes
struct Node* createTree(struct Token* token);

struct Node* insertNode(struct Node* node, struct Token* token);


void addSubtree(struct Node* node, struct Node* subtree);

void inorderTraversal(struct Node *node,int depth);
void preorderTraversal(struct Node *node, int depth);

#endif
