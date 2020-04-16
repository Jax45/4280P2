#include "generator.h"

void generateCode(ofstream& file, struct Node* tree){
//IMPORTS:
	file << "#include <stdio.h>" << endl << endl;

	struct Node* varsNode = tree->first;
//VARS
	while(varsNode->first != NULL && varsNode->first->token->instance != "Empty"){
		file << "int " << varsNode->first->token->instance;
		file << " = " << varsNode->second->token->instance;
		file << ";\n";
		//get the next vars node.
		varsNode = varsNode->third;
	}
	file << "\n"; //spacing
//BLOCK
  //VARS
	//print the start of the block
	file << "int main(){\n";	
	//program->block->vars
	struct Node* blockVarsNode = tree->second->first;
	while(blockVarsNode->first != NULL && blockVarsNode->first->token->instance != "Empty"){
        	file << "\tint " << blockVarsNode->first->token->instance;
        	file << " = " << blockVarsNode->second->token->instance;
        	file << ";\n";
        	//get the next vars node.
        	blockVarsNode = blockVarsNode->third;
        }
  //STATS
	//program->block->stats:
	struct Node* statsNode = tree->second->second; //STATS
	
    //STAT
    	struct Node* statNode = statsNode->first;     //STATS->STAT
	int depth = 1;
	generateStat(file,statNode,depth);

    //MSTAT
	struct Node* mStatNode = statsNode->second;   //STATS->MSTAT
	//STAT
	while(mStatNode->first != NULL && mStatNode->first->token->instance != "Empty" ){
		generateStat(file,mStatNode->first,depth);
	
	//MSTAT	
		mStatNode = mStatNode->second; //MSTAT->MSTAT->STAT/EMPTY

	}
}
void printTabs(ofstream& file, int depth){
	for(int i = 0; i < depth; i++){
                file << "\t";
        }

}
void generateStat(ofstream& file, struct Node* statNode,int depth){
	string type = statNode->first->token->instance;
	printTabs(file,depth);
	if(type == "<Assign>"){
		file <<  statNode->first->first->token->instance;
		//check for expression being assigned
		if(statNode->first->second->token->instance == "<Expr>"){
			file << " = ";
			generateExpr(file,statNode->first->second);
		}
		else{
			file << " = " << statNode->first->second->token->instance;
		}
		file << ";\n";
	}
	else if(type == "<Iffy>"){
		file << "if (";
		generateExpr(file,statNode->first->first);
		file << " ";
		generateRO(file,statNode->first->second);
		file << " ";
		generateExpr(file,statNode->first->third);
		file << "){\n";
		
		generateStat(file,statNode->first->fourth,depth+1);
		printTabs(file,depth);
		file << "}\n";
	}
	else if(type == "<Goto>"){
		file << "goto " << statNode->first->first->token->instance;
                file << ";\n";
	}
	else if(type == "<Label>"){
		file << statNode->first->first->token->instance;
		file << ":\n";
	}
	else if(type == "<In>"){
		file << "scanf(\"\%d\", &" << statNode->first->first->token->instance;
		file << ");\n";
	}
	else if(type == "<Out>"){
		file << "printf(\"\%d\\n\", ";
		generateExpr(file,statNode->first->first);
                file << ");\n";
	}
	else if(type == "<Loop>"){
		file << "" << "while (";
                generateExpr(file,statNode->first->first);
                file << " ";
                generateRO(file,statNode->first->second);
                file << " ";
                generateExpr(file,statNode->first->third);
                file << "){\n";
                generateStat(file,statNode->first->fourth,depth+1);
                printTabs(file,depth);
		file << "}\n";

	}
	else if(type == "<Block>"){
		//not sure what this is supposed to do...
		
	}
	return;
}

void generateExpr(ofstream& file, struct Node* node){
	generateN(file,node->first);
	if(node->second != NULL){
		file << " - ";
		generateExpr(file,node->third);
	}
}

void generateN(ofstream& file, struct Node* node){
	generateA(file,node->first);
	if(node->second != NULL){
		file << " " << node->second->token->instance << " ";
		generateN(file,node->third);
	}
}
void generateA(ofstream& file, struct Node* node){
	generateM(file,node->first);
	if(node->second != NULL){
		file << " " << node->second->token->instance << " ";
                generateA(file,node->third);
	}
}
void generateM(ofstream& file, struct Node* node){
	if(node->first->token->instance == "*"){
		file << "* ";
		generateM(file,node->second);
	}
	else{
		generateR(file,node->first);
	}
}
void generateR(ofstream& file, struct Node* node){
	if(node->first->token->instance == "("){
		//( <expr> )
		file << "(";
		generateExpr(file,node->second);
		file << ")";
	}
	else{
		file << node->first->token->instance;
	}
}

void generateRO(ofstream& file, struct Node* node){
	//not equal
	if(node->first->token->instance == "<" && node->second->token->instance == ">"){
		file << "!=";
	}
	else{
		//==,<, or >
		file << node->first->token->instance;
		if(node->second != NULL){
			if(node->second->token->instance == ">"){
				file << "=";
			}
			else if(node->second->token->instance == "<"){
				file << "=";
			}
			else{
				cout << "ERROR: Generator.cpp: Found an incorrect Node in the parse tree";
				cout << " when forming the RO." << node->second->token->instance << endl;
			}
		}
	}
}
