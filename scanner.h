//this is going to be the scanner source file.
//it will contain a function that when called will
//return 1 token from a string given to it.
#ifndef SCANNER_H
#define SCANNER_H
#include "fsaTable.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
struct Token scanner(ifstream& file);
#endif

