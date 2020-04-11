#ifndef GENERATOR_H
#define GENERATOR_H
#include "parser.h"

void generateCode(ofstream& file, struct Node* tree);
void generateStat(ofstream& file, struct Node* statNode,int depth);
void generateExpr(ofstream& file, struct Node* statNode);
void generateN(ofstream& file, struct Node* statNode);
void generateA(ofstream& file, struct Node* statNode);
void generateM(ofstream& file, struct Node* statNode);
void generateR(ofstream& file, struct Node* statNode);
void generateRO(ofstream& file, struct Node* statNode);

#endif
