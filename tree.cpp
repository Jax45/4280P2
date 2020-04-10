//tree.cpp
//This is the source file for the Parse Tree
//it contains all of the functions nessesary to create
//and operate the Parse tree generated from parser.cpp.
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "node.h"
#include "tree.h"
#include <cstring>
#include <iostream>
/*Create the tree with a single node*/
struct Node* createTree(struct Token* token){
	struct Node *temp = new Node();//(struct Node*)malloc(sizeof(struct Node));
	temp->token = token;
	temp->first = temp->second = temp->third = temp->fourth = temp->fifth = temp->sixth = NULL;
	return temp;
}


/*if given a Node and a subtree add the subtree to the given node*/
void addSubtree(struct Node* node, struct Node* subtree){
	
	if (node == NULL){
		//sanity check
		printf("ERROR: TREE.CPP: Expected a node to add subtree to but found NULL");
		exit(1);
	}
	if(subtree == NULL){
		//we have nothing to add
		return;
	}
	
	//add the tree as a node
	if(node->first == NULL){
                node->first = subtree;
        }
        else if(node->second == NULL){
                node->second = subtree;
        }
        else if(node->third == NULL){
                node->third = subtree;
        }
        else if(node->fourth == NULL){
                node->fourth = subtree;
        }
        else if(node->fifth == NULL){
                node->fifth = subtree;
        }
        else if(node->sixth == NULL){
                node->sixth = subtree;
        }
}



/*Add a node to the current level wherever is free*/
struct Node* insertNode(struct Node* node, struct Token* token) 
{ 
	/* If the node is empty, return a new node, end case */
        if (node == NULL){
                return createTree(token);
        }
	//copy the token
	struct Token* temp = new Token();
	temp->instance = token->instance;
	temp->line = token->line;
	temp->tkId = token->tkId;
	//if the first is not empty move to second:
	if(node->first == NULL){
		node->first = createTree(temp);
	}
	else if(node->second == NULL){
		node->second = createTree(temp);
	}
	else if(node->third == NULL){
		node->third = createTree(temp);
	}
	else if(node->fourth == NULL){
		node->fourth = createTree(temp);
	}
        else if(node->fifth == NULL){
                node->fifth = createTree(temp);
        }
        else if(node->sixth == NULL){
                node->sixth = createTree(temp);
        }
	else{
		//something weird happend if we got here
		perror("ERROR: Tree.cpp: For some reason all of the child nodes are full");
		exit(1);
	}
	return node; 
} 


/*Inorder traversal*/
void inorderTraversal(struct Node *node, int depth){
	/*arg depth is to keep track of depth recusively*/
	if(node == NULL){
		return;
	}
	if(depth == 0){
		cout << node->token->instance << endl;
	}
	string depthStr = "";
	//get the depth string of spaces
	for(int i = 0; i < depth+1; i++){
		depthStr += "__|";
	}
	
	//print children
	if(node->first != NULL){
		cout << depthStr << node->first->token->instance << endl;
		if(node->second != NULL){
			cout << depthStr << node->second->token->instance << endl;
			if(node->third != NULL){
				cout << depthStr << node->third->token->instance << endl;
			}
		}
	}
        if(node->fourth != NULL){
                cout << depthStr << node->fourth->token->instance << endl;
                if(node->fifth != NULL){
                        cout << depthStr << node->fifth->token->instance << endl;
                        if(node->sixth != NULL){
                                cout << depthStr << node->sixth->token->instance << endl;
                        }
                }
        }

	inorderTraversal(node->first,depth+1);
	inorderTraversal(node->second,depth+1);
	inorderTraversal(node->third,depth+1);
	inorderTraversal(node->fourth,depth+1);
	inorderTraversal(node->fifth,depth+1);
	inorderTraversal(node->sixth,depth+1);
}

