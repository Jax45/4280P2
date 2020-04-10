#ifndef PARSER_H
#define PARSER_H
#include "scanner.h"
#include "tree.h"


void parser(ifstream& file);
struct Node* program();
struct Node* block();
struct Node* vars();
void expr();
void N();
void A();
void M();
void R();
void stats();
void mStat();
void stat();
void semicolon();
void in();
void out();
void iffy();
void assign();
void label();
void goTo();
void RO();


#endif
