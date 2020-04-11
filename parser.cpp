//Parser function
#include "parser.h"
#include <stdbool.h>
//global variables
struct Token tk;
ifstream* file;
bool lookedAhead = false;

void printError(string expected, string found,int line){
	cout << "Parser expected: '" << expected << "', but found: '" << found << "' On line: '#" << line << "'" << endl;
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
	preorderTraversal(tree,0);
	file->close();
	cout << "Parser completed" << endl;
	return;
}

struct Node* program(){
	//TokenId firstSet[] = {DeclareTk};
	struct Node* tree = createTree(nonterminal("<Program>"));
	addSubtree(tree,vars());
	addSubtree(tree,block());
	getNextToken();
	if(tk.tkId != EofTk){
		printError("End of file",tk.instance,tk.line);
	}
	return tree;
}

struct Node* block(){
	//TokenId firstSet[] = {BeginBlockTk};
	
	struct Node* tree = createTree(nonterminal("<Block>"));
	getNextToken();
	if(tk.instance == "{"){
		//consume 
		//leave out {: tree = insertNode(tree,&tk);
		addSubtree(tree,vars());
		addSubtree(tree,stats());
	        getNextToken();
		if(tk.instance == "}"){
			//add to tree 
			//leave out }: tree = insertNode(tree,&tk);
		}
		else{printError("}",tk.instance,tk.line);}
			
	}else{printError("{",tk.instance,tk.line);}
	
	return tree;
}

struct Node* vars(){
	TokenId firstSet[] = {DeclareTk};
	getNextToken();
	struct Node* tree = createTree(nonterminal("<Vars>"));
	if(tk.tkId == DeclareTk){ // declare id := ; <vars>
		//leave out declare: tree = insertNode(tree,&tk);
		getNextToken();
		if(tk.tkId == IdTk){ //idtoken := Integer ; vars
			tree = insertNode(tree,&tk);//consume idtoken
			getNextToken();
			if(tk.tkId == AsgTk){
				//consume :=
				//leave out ':=' : tree = insertNode(tree,&tk);
				getNextToken();
				if(tk.tkId == NumTk){
					//consume integer
					tree = insertNode(tree,&tk);	
					getNextToken();
					if(tk.instance == ";"){
						//add the semicolon
						//leave out the ';': tree = insertNode(tree,&tk);
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
		printError("Integer,Identifier, or '('",tk.instance,tk.line);
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
	getNextToken();
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
	getNextToken();
	//looked ahead here
	lookedAhead = true;
	struct Node* tree = createTree(nonterminal("<Stat>"));
	if(tk.tkId == InTk){
		addSubtree(tree,in());
	}
	else if(tk.tkId == OutTk){
		addSubtree(tree,out());
	}
        else if(tk.tkId == BeginBlockTk){
                addSubtree(tree,block());
        }
        else if(tk.tkId == IffyTk){
                addSubtree(tree,iffy());
        }
        else if(tk.tkId == LoopTk){
                addSubtree(tree,loop());
        }
        else if(tk.tkId == IdTk){
                addSubtree(tree,assign());
        }
        else if(tk.tkId == GotoTk){
                addSubtree(tree,goTo());
        }
        else if(tk.tkId == LabelTk){
                addSubtree(tree,label());
        }
	else{
		printError("A Statement",tk.instance,tk.line);
	}
	//tree = insertNode(tree,semicolon());
        semicolon();
	return tree;

}
void semicolon(){
	getNextToken();
        if(tk.instance != ";"){
       		printError(";",tk.instance,tk.line);
	}
	return;
}
struct Node* in(){
 //       TokenId firstSet[] = {InTk};
	struct Node* tree = createTree(nonterminal("<In>"));
	getNextToken();
	//leave out the in: tree = insertNode(tree,&tk);
	//get identifier
	getNextToken();
	if(tk.tkId == IdTk){
		tree = insertNode(tree,&tk);
	}else{printError("Identifier",tk.instance,tk.line);}

	return tree;
}
struct Node* out(){
//	TokenId firstSet[] = {OutTk};
	struct Node* tree = createTree(nonterminal("<Out>"));
	getNextToken(); 
        //leave out the out: tree = insertNode(tree,&tk);
	//get expr
	addSubtree(tree,expr());
        return tree;
}
struct Node* iffy(){
//	TokenId firstSet[] = {IffyTk};
	struct Node* tree = createTree(nonterminal("<Iffy>"));
        getNextToken();
	//leave out iffy: tree = insertNode(tree,&tk);
	// [ <expr> <RO> <expr> ] then <stat>
	getNextToken();
	if(tk.instance == "["){
		//leave out [ tree = insertNode(tree,&tk);
		addSubtree(tree,expr());
		addSubtree(tree,RO());
		addSubtree(tree,expr());
		getNextToken();
	        if(tk.instance == "]"){
			//leave out ] tree = insertNode(tree,&tk);
			getNextToken();
		        if(tk.tkId == ThenTk){
				//leave out then: tree = insertNode(tree,&tk);
				addSubtree(tree,stat());
			}else{printError("then",tk.instance,tk.line);}
		}else{printError("]",tk.instance,tk.line);}
	}else{printError("[",tk.instance,tk.line);}

	return tree;
}
struct Node* loop(){
        struct Node* tree = createTree(nonterminal("<Loop>"));
	getNextToken();
        //leave out loop: tree = insertNode(tree,&tk);
	
//[ <expr> <RO> <expr> ] <stat>
	getNextToken();
        if(tk.instance == "["){
                //leave out [: tree = insertNode(tree,&tk);
                addSubtree(tree,expr());
                addSubtree(tree,RO());
                addSubtree(tree,expr());
                getNextToken();
                if(tk.instance == "]"){
                        //leave out ]: tree = insertNode(tree,&tk);
                        addSubtree(tree,stat());
                }else{printError("]",tk.instance,tk.line);}
        }else{printError("[",tk.instance,tk.line);}

        return tree;

}
struct Node* assign(){
//	TokenId firstSet[] = {IdTk};
	struct Node* tree = createTree(nonterminal("<Assign>"));
	getNextToken(); 
       	tree = insertNode(tree,&tk); //consume IdTk
	// := <expr>
	getNextToken();
	if(tk.instance == ":="){
		//leave out the ':=': tree = insertNode(tree,&tk);
		addSubtree(tree,expr());
	}else{printError(":=",tk.instance,tk.line);}
	return tree;
}
struct Node* label(){
//	TokenId firstSet[] = {LabelTk};
        struct Node* tree = createTree(nonterminal("<Label>"));
	getNextToken(); 
       	//leave out the 'label': tree = insertNode(tree,&tk); /*consume label*/
	/*get identifier*/
	getNextToken();
	if(tk.tkId == IdTk){
		tree = insertNode(tree,&tk);
	}else{printError("Identifier",tk.instance,tk.line);}
        return tree;

}
struct Node* goTo(){
//	TokenId firstSet[] = {GotoTk};
	struct Node* tree = createTree(nonterminal("<Goto>"));
	getNextToken();
        //leave out the goto: tree = insertNode(tree,&tk); /*consume goto*/
        /*get identifier*/
        getNextToken();
	if(tk.tkId == IdTk){
                tree = insertNode(tree,&tk);
        }else{printError("Identifier",tk.instance,tk.line);}
        return tree;

}
struct Node* RO(){
//	TokenId firstSet[] = {LessThanTk,GreaterThanTk,IsEqualTk};
	struct Node* tree = createTree(nonterminal("<RO>"));
	getNextToken();
	if(tk.tkId == LessThanTk){
		tree = insertNode(tree,&tk);
		getNextToken();
		if(tk.tkId == LessThanTk){
			tree = insertNode(tree,&tk); // <<
		}
		else if(tk.tkId == GreaterThanTk){
			tree = insertNode(tree,&tk); // <>
		}else{
			lookedAhead = true;    // <
		}
	}

	else if(tk.tkId == GreaterThanTk){
                tree = insertNode(tree,&tk);
                getNextToken();
                if(tk.tkId == GreaterThanTk){
			tree = insertNode(tree,&tk); // >>	
		}
		else{
			lookedAhead = true;		
			// else it is just >
		}
	}

	else if(tk.tkId == IsEqualTk){
                tree = insertNode(tree,&tk);
		// ==
	}
	else{
		printError("Relational Operator",tk.instance,tk.line);
	}
	return tree;
}

void getNextToken(){
	if(lookedAhead == false){
                tk = scanner(*file);
        }
        else{
                lookedAhead = false;
        }

}
