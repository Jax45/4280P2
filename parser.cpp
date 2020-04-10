//Parser function
#include "parser.h"

//global variables
struct Token tk;
ifstream* file;

void printError(string expected, string found,int line){
	cout << "expected: " << expected << " found: " << found << " On line: " << line << endl;
	file->close();
	exit(1);
}
void parser(ifstream& fp){
	file = &fp;
	program();
	file->close();
	cout << "Parser completed" << endl;
	return;
}

void program(){
	TokenId firstSet[] = {DeclareTk};
	vars();
	block();
	return;
}

void block(){
	TokenId firstSet[] = {BeginBlockTk};
	tk = scanner(*file);
	if(tk.instance == "{"){
		//consume {
		vars();
		stats();
		tk = scanner(*file);
		if(tk.instance == "}"){
			//finished.
		}
		else{printError("}",tk.instance,tk.line);}
			
	}else{printError("{",tk.instance,tk.line);}
	
	return;
}

void vars(){
	TokenId firstSet[] = {DeclareTk};
	tk = scanner(*file);
	if(tk.tkId == DeclareTk){ // declare id := ; <vars>
	
		if(tk.tkId == IdTk){ //idtoken := Integer ; vars
			//consume idtoken
			tk = scanner(*file);
			if(tk.tkId == AsgTk){
				//consume :=
				tk = scanner(*file);
				if(tk.tkId == NumTk){
					//consume integer
					tk = scanner(*file);
					if(tk.instance == ";"){
						vars();
					}
					else{printError(";",tk.instance,tk.line);}		
				}
				else{printError("Integer",tk.instance,tk.line);}
			}
			else{printError(":=",tk.instance,tk.line);}
		}
		else{printError("Identifier",tk.instance,tk.line);}
	}
	else{
		//empty
	}
	return;
}

void expr(){
	TokenId firstSet[] = {MultiplyTk,BeginParenTk,IdTk,NumTk};
	return;
}
void N(){
	TokenId firstSet[] = {MultiplyTk,BeginParenTk,IdTk,NumTk};
	return;
}
void A(){
	TokenId firstSet[] = {MultiplyTk,BeginParenTk,IdTk,NumTk};
	return;
}
void M(){
	TokenId firstSet[] = {MultiplyTk,BeginParenTk,IdTk,NumTk};
	return;
}
void R(){
	TokenId firstSet[] = {BeginParenTk,IdTk,NumTk};
	return;	
}
void stats(){
	TokenId firstSet[] = {InTk,OutTk,BeginBlockTk,IffyTk,LoopTk,IdTk,GotoTk,LabelTk};

	stat();
	mStat();
	return;
}

void mStat(){
	TokenId firstSet[] = {InTk,OutTk,BeginBlockTk,IffyTk,LoopTk,IdTk,GotoTk,LabelTk};
	
	//if not first set return empty
	return;
}

void stat(){
	
        TokenId firstSet[] = {InTk,OutTk,BeginBlockTk,IffyTk,LoopTk,IdTk,GotoTk,LabelTk};
	tk = scanner(*file);
	if(tk.tkId == InTk){
		in();
		semicolon();
		return;
	}
}
void semicolon(){
	tk = scanner(*file);
        if(tk.instance != ";"){
       		printError(";",tk.instance,tk.line);
	}
	return;
}
void in(){
        TokenId firstSet[] = {InTk};
        return;
}
void out(){
	TokenId firstSet[] = {OutTk};
	return;		
}
void iffy(){
	TokenId firstSet[] = {IffyTk};
	return;
}
void assign(){
	TokenId firstSet[] = {IdTk};
	return;
}
void label(){
	TokenId firstSet[] = {LabelTk};
	return;
}
void goTo(){
	TokenId firstSet[] = {GotoTk};
	return;
}
void RO(){
	TokenId firstSet[] = {LessThanTk,GreaterThanTk,IsEqualTk};
	return;
}
