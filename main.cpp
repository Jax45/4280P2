//main.cpp
//Jackson Hoenig
//This main driver is to call the parser with the given
//file from the argument. the file needs to have the suffix
//".SP2020" and has to be in the correct format otherwise the 
//parser will issue an error.
//EXTERNAL FILES:
//fsaTable.h
//scanner.h
//scanner.cpp
//parser.cpp
//parser.h
//tree.h
//node.h
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"
#include "generator.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

//prototype
void printToken(struct Token);

int main(int argc, char** argv){
	/*constants*/
	const char* fileName;
	/*variables*/
	
	/*string to hold the input from the file*/
	string fileString = "";
	//file pointer
	char* copyFile = NULL; /*to hold the file name without the extension*/
	
	if(argc > 2){
		printf("P0 Only takes in 1 argument.\n");  /* handle args*/
		return 1;
	}

	/*if given open the given file, if not given go to stdin.*/
	if(argc == 2){
		fileName = argv[1];
		copyFile = (char*)malloc(strlen(argv[1]));
		strcpy(copyFile,fileName);
		//copyFile[strlen(fileName)-1] = 0;
		strcat(copyFile, ".SP2020\0");
		//fp = fopen(copyFile,"r");
		ifstream file(copyFile);
			
		cout << "Calling Parser" << endl;
		struct Node* parseTree = parser(file);

	        preorderTraversal(parseTree,0);

/*Here I tried to start on Code Generation. It somewhat works,
 * but sometimes it will segmentation fault because i have
 * not implemented <block> <in> and <out> yet since
 * I do not know what the purpose of those are yet besides
 * <block> doing main for the first block.
 *
 * uncomment below if you want to see it in action.
 */
		//generate a file we can compile?
		//print out the declarations(VARS),
		//then the VARS inside <BLOCK>
		
	/* UNCOMMENT HERE:
		string cFile = "generatedCode.c";
                ofstream outFile(cFile);
		generateCode(outFile,parseTree);
		outFile << "\treturn 0;\n";
		outFile << "}";	
	*/
		return 0;	
		
	}
	else{
		printf("Please provide a filename without the .SP2020 prefix as an argument.");
		printf("\nEnding Program\n");
		return 0;
	}
	return 0;
}

void printToken(struct Token token){	
	cout << "token: {";
	switch(token.tkId){
	case GenErrTk:
		cout << "GenErrTk";
		break;
	case AsgErrTk:
		cout << "AsgErrTk";
		break;
	case IdTk:
		cout << "IdTk";
		break;
	case LabelTk:
		cout << "LabelTk";
		break;
	case GotoTk:
		cout << "GotoTk";
		break;
	case LoopTk:
		cout << "LoopTk";
		break;
	case VoidTk:
		cout << "VoidTk";
		break;
	case DeclareTk:
		cout << "DeclareTk";
		break;
	case ReturnTk:
		cout << "ReturnTk";
		break;
	case InTk:
		cout << "InTk";
		break;
	case OutTk:
		cout << "OutTk";
		break;
	case ProgramTk:
		cout << "ProgramTk";
		break;
	case IffyTk:
		cout << "IffyTk";
		break;
	case ThenTk:
		cout << "ThenTk";
		break;
	case AssignTk:
		cout << "AssignTk";
		break;
	case DataTk:
		cout << "DataTk";
		break;
	case NumTk:
		cout << "NumTk";
		break;
	case DelTk:
		cout << "DelTk";
		break;
	case AsgTk:
		cout << "AsgTk";
		break;
	case EofTk:
		cout << "EofTk";
		break;
	case CmtErrTk:
		cout << "CmtErrTk";
		break;
	default:
		cout << "UnknownTk";
	}

 	cout << ", " << token.instance << ", " << token.line << "}" << endl;
}
