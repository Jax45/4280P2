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

struct Token* nonterminal(string str){
	struct Token* tk = new Token();//(struct Token*)malloc(sizeof(struct Token));;
	tk->instance = str;
	tk->tkId = NonterminalTk;
	return tk;

}

void parser(ifstream& fp){
	file = &fp;
	struct Node* tree = program();
	inorderTraversal(tree,0);
	file->close();
	cout << "Parser completed" << endl;
	return;
}

struct Node* program(){
	//TokenId firstSet[] = {DeclareTk};
	struct Node* tree = createTree(nonterminal("<Program>"));
	//addSubtree(tree,vars());
	addSubtree(tree,block());
	return tree;
}

struct Node* block(){
	//TokenId firstSet[] = {BeginBlockTk};
	
	struct Node* tree = createTree(nonterminal("<Block>"));
	
	tk = scanner(*file);
	if(tk.instance == "{"){
		//consume 
		tree = insertNode(tree,&tk);
		addSubtree(tree,vars());
		//addSubtree(tree,stats());
		tk = scanner(*file);
		if(tk.instance == "}"){
			//add to tree 
			tree = insertNode(tree,&tk);
		}
		else{printError("}",tk.instance,tk.line);}
			
	}else{printError("{",tk.instance,tk.line);}
	
	return tree;
}

struct Node* vars(){
	TokenId firstSet[] = {DeclareTk};
	tk = scanner(*file);
	struct Node* tree = createTree(nonterminal("<Vars>"));
	if(tk.tkId == DeclareTk){ // declare id := ; <vars>
		tree = insertNode(tree,&tk);
		tk = scanner(*file);
		if(tk.tkId == IdTk){ //idtoken := Integer ; vars
			tree = insertNode(tree,&tk);//consume idtoken
			tk = scanner(*file);
			if(tk.tkId == AsgTk){
				//consume :=
				tree = insertNode(tree,&tk);
				tk = scanner(*file);
				if(tk.tkId == NumTk){
					//consume integer
					tree = insertNode(tree,&tk);	
					tk = scanner(*file);
					if(tk.instance == ";"){
						//add the semicolon
						tree = insertNode(tree,&tk);
						//add the vars token
						addSubtree(tree,vars());
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
		tree = insertNode(tree,nonterminal("Empty"));
	}
	return tree;
}

void expr(){
	//TokenId firstSet[] = {MultiplyTk,BeginParenTk,IdTk,NumTk};
	return;
}
void N(){
	//TokenId firstSet[] = {MultiplyTk,BeginParenTk,IdTk,NumTk};
	return;
}
void A(){
	//TokenId firstSet[] = {MultiplyTk,BeginParenTk,IdTk,NumTk};
	return;
}
void M(){
	//TokenId firstSet[] = {MultiplyTk,BeginParenTk,IdTk,NumTk};
	return;
}
void R(){
	//TokenId firstSet[] = {BeginParenTk,IdTk,NumTk};
	return;	
}
void stats(){
	//TokenId firstSet[] = {InTk,OutTk,BeginBlockTk,IffyTk,LoopTk,IdTk,GotoTk,LabelTk};

	stat();
	mStat();
	return;
}

void mStat(){
	//TokenId firstSet[] = {InTk,OutTk,BeginBlockTk,IffyTk,LoopTk,IdTk,GotoTk,LabelTk};
	
	//if not first set return empty
	return;
}

void stat(){
	
        //TokenId firstSet[] = {InTk,OutTk,BeginBlockTk,IffyTk,LoopTk,IdTk,GotoTk,LabelTk};
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
 //       TokenId firstSet[] = {InTk};
        return;
}
void out(){
//	TokenId firstSet[] = {OutTk};
	return;		
}
void iffy(){
//	TokenId firstSet[] = {IffyTk};
	return;
}
void assign(){
//	TokenId firstSet[] = {IdTk};
	return;
}
void label(){
//	TokenId firstSet[] = {LabelTk};
	return;
}
void goTo(){
//	TokenId firstSet[] = {GotoTk};
	return;
}
void RO(){
//	TokenId firstSet[] = {LessThanTk,GreaterThanTk,IsEqualTk};
	return;
}
