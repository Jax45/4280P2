#ifndef PARSER_H
#define PARSER_H
#include "scanner.h"



void parser(ifstream& file);
void program();
void block();
void vars();
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
