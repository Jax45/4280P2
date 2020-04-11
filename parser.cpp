//Parser function
#include "parser.h"
#include <stdbool.h>
//global variables
struct Token tk;
ifstream* file;
bool lookedAhead = false;

void printError(string expected, string found,int line){
	cout << "expected: " << expected << " found: " << found << " On line: " << line << endl;
	file->close();
	exit(1);
}

void unexpectedError(){
	cout << "Error encountered while parsing. Unexpected \"" << tk.instance
		<< "\" found on line:" << tk.line << endl;
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
	debugTraversal(tree,0);
	file->close();
	cout << "Parser completed" << endl;
	return;
}

struct Node* program(){
	//TokenId firstSet[] = {DeclareTk};
	struct Node* tree = createTree(nonterminal("<Program>"));
	addSubtree(tree,vars());
	addSubtree(tree,block());
	return tree;
}

struct Node* block(){
	//TokenId firstSet[] = {BeginBlockTk};
	
	struct Node* tree = createTree(nonterminal("<Block>"));
	if(lookedAhead == false){
		tk = scanner(*file);
	}
	else{
		lookedAhead = false;
	}
	if(tk.instance == "{"){
		//consume 
		tree = insertNode(tree,&tk);
		addSubtree(tree,vars());
		addSubtree(tree,stats());
	        if(lookedAhead == false){
	                tk = scanner(*file);
	        }
	        else{
	                lookedAhead = false;
	        }
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
		//we currently have an unconsumed token in our tk.
		lookedAhead = true;
		tree = insertNode(tree,nonterminal("Empty"));
	}
	return tree;
}

struct Node* expr(){
	//TokenId firstSet[] = {MultiplyTk,BeginParenTk,IdTk,NumTk};
	
	struct Node* tree = createTree(nonterminal("<Expr>"));
	addSubtree(tree,N());
	//   - <expr> or nothing
	getNextToken();
	if(tk.instance == "-"){
		tree = insertNode(tree,&tk);
		addSubtree(tree,expr());
	}
	else{
		lookedAhead = true;
	}
	return tree;
}
struct Node* N(){
	//TokenId firstSet[] = {MultiplyTk,BeginParenTk,IdTk,NumTk};
	struct Node* tree = createTree(nonterminal("<N>"));
        addSubtree(tree,A());
	getNextToken();
	/* / <N> | * <N> | nothing*/
        if(tk.instance == "/" || tk.instance == "*"){
                tree = insertNode(tree,&tk);
		addSubtree(tree,N());
        }
	else{
                lookedAhead = true;
        }	
        return tree;	
}
struct Node* A(){
	//TokenId firstSet[] = {MultiplyTk,BeginParenTk,IdTk,NumTk};
	struct Node* tree = createTree(nonterminal("<A>"));
        addSubtree(tree,M());
	getNextToken();
        if(tk.instance == "+"){
                tree = insertNode(tree,&tk);
                addSubtree(tree,A());
        }
	else{
                lookedAhead = true;
        }
        return tree;
}
struct Node* M(){
	//TokenId firstSet[] = {MultiplyTk,BeginParenTk,IdTk,NumTk};
	struct Node* tree = createTree(nonterminal("<M>"));
        getNextToken();
        if(tk.instance == "*"){
                tree = insertNode(tree,&tk);
                addSubtree(tree,M());
        }
        else{
                lookedAhead = true;
		addSubtree(tree,R());
        }
        return tree;
}
struct Node* R(){
	//TokenId firstSet[] = {BeginParenTk,IdTk,NumTk};
	struct Node* tree = createTree(nonterminal("<R>"));
        getNextToken();
        if(tk.instance == "("){
		//(<expr>)
                tree = insertNode(tree,&tk);
                addSubtree(tree,expr());
		getNextToken();
		if(tk.instance == ")"){
			tree = insertNode(tree,&tk);
		}else{printError(")",tk.instance,tk.line);}
        }
	else if(tk.tkId == IdTk){
                //identifier
        	tree = insertNode(tree,&tk);	
	}
        else if(tk.tkId == NumTk){
         	//Integer
         	tree = insertNode(tree,&tk);       
        }
	else{
		unexpectedError();
	}
        return tree;
}
struct Node* stats(){
	//TokenId firstSet[] = {InTk,OutTk,BeginBlockTk,IffyTk,LoopTk,IdTk,GotoTk,LabelTk};
	struct Node* tree = createTree(nonterminal("<Stats>"));
	addSubtree(tree,stat());
	addSubtree(tree,mStat());
	return tree;
}

struct Node* mStat(){
	set<TokenId> firstSet = {InTk,OutTk,BeginBlockTk,IffyTk,LoopTk,IdTk,GotoTk,LabelTk};
	tk = scanner(*file);
        struct Node* tree = createTree(nonterminal("<MStat>"));	
	if(firstSet.count(tk.tkId) != 0){
		//we looked ahead here
		lookedAhead = true;
	        addSubtree(tree,stat());
	        addSubtree(tree,mStat());
	}
	else{
		//if not first set return empty
		//we currently have an unconsumed token in our tk.
		lookedAhead = true;
		tree = insertNode(tree,nonterminal("Empty"));
	}
	return tree;
}

struct Node* stat(){
	
        //TokenId firstSet[] = {InTk,OutTk,BeginBlockTk,IffyTk,LoopTk,IdTk,GotoTk,LabelTk};
	if(lookedAhead == false){
                tk = scanner(*file);
        }
        else{
                lookedAhead = false;
        }
	struct Node* tree = createTree(nonterminal("<Stat>"));
	if(tk.tkId == InTk){
		addSubtree(tree,in());
	}
	else if(tk.tkId == OutTk){
		addSubtree(tree,out());
	}
        else if(tk.tkId == BeginBlockTk){
                addSubtree(tree,out());
        }
        else if(tk.tkId == IffyTk){
                addSubtree(tree,out());
        }
        else if(tk.tkId == LoopTk){
                addSubtree(tree,out());
        }
        else if(tk.tkId == IdTk){
                addSubtree(tree,out());
        }
        else if(tk.tkId == GotoTk){
                addSubtree(tree,out());
        }
        else if(tk.tkId == LabelTk){
                addSubtree(tree,out());
        }
	else{
		unexpectedError();
	}
	tree = insertNode(tree,semicolon());
        return tree;

}
struct Token* semicolon(){
	getNextToken();
        if(tk.instance != ";"){
       		printError(";",tk.instance,tk.line);
	}
	return &tk;
}
struct Node* in(){
 //       TokenId firstSet[] = {InTk};
	struct Node* tree = createTree(nonterminal("<In>"));
	tree = insertNode(tree,&tk);
	//get identifier
	tk = scanner(*file);
	if(tk.tkId == IdTk){
		tree = insertNode(tree,&tk);
	}else{printError("Identifier",tk.instance,tk.line);}

	return tree;
}
struct Node* out(){
//	TokenId firstSet[] = {OutTk};
	struct Node* tree = createTree(nonterminal("<Out>"));
        tree = insertNode(tree,&tk);
	//get expr
	addSubtree(tree,expr());
        return tree;
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

void getNextToken(){
	if(lookedAhead == false){
                tk = scanner(*file);
        }
        else{
                lookedAhead = false;
        }

}
