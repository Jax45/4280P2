#ifndef PARSER_H
#define PARSER_H
#include "scanner.h"
#include "tree.h"


void parser(ifstream& file);
struct Node* program();
struct Node* block();
struct Node* vars();
struct Node* expr();
struct Node* N();
struct Node* A();
struct Node* M();
struct Node* R();
struct Node* stats();
struct Node* mStat();
struct Node* stat();
struct Token* semicolon();
struct Node* in();
struct Node* out();
void iffy();
void assign();
void label();
void goTo();
void RO();

void getNextToken();

#endif
