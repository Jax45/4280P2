//This is the node for the Parse tree. since the
//maximum number of nodes we will need is 4 we have 
//four children pointer nodes to more nodes.
//each node also contains a token structure that
//is stored in fsaTable.h

#ifndef NODE_H
#define NODE_H
#include "fsaTable.h"
struct Node {
	struct Token *token = NULL;
	struct Node *first, *second, *third, *fourth;		
};


#endif
