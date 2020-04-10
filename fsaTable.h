//this is to create a class of an fsa table to be used by the scanner.
//It also contains Enums used by the scanner and driver to identify
//tokens.
#ifndef FSA_TABLE
#define FSA_TABLE
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <set>
#define ROWS 6
#define COLS 8
using namespace std;
enum colId {Letter=0, Number=1, Eof=2,Equal=3,Delim=4,Space=5,Colon=6,Extra=7};
enum rowId {S1 = 0, S2=1, S3=2, S4=3, S5=4,S6=5};
enum TokenId {GenErrTk = -1, AsgErrTk = -2, CmtErrTk = -3, 
	EofTk = 1000,DelTk = 1001,
	NumTk = 1002,IdTk = 1004, AsgTk = 1005, LabelTk = 1006,
	GotoTk = 1007, LoopTk = 1008, VoidTk,DeclareTk,ReturnTk,
	InTk,OutTk,ProgramTk,IffyTk,ThenTk,AssignTk,DataTk, 
	BeginBlockTk,BeginParenTk,MultiplyTk,LessThanTk,
	GreaterThanTk,IsEqualTk,
	State1=0,State2=1,State3=2,State4=3,State5=4,State6=5};

//string TokenNames[] = {"EOF","Delimeter","Number","Error","Identifier"};

struct Token {
	TokenId tkId;
	string instance;
	int line;
};

class FSATable{
	private:
	TokenId table[ROWS][COLS];
	
	public:
	FSATable(){
		//set all cells to scanner error to start
		for(int r = 0; r < ROWS; r++){
			for( int c = 0; c < COLS; c++){

				table[r][c] = GenErrTk;
			}

		}

		//set the initial state row
		table[S1][Letter] = State3;
		table[S1][Number] = State2;
		table[S1][Eof] = EofTk;
		table[S1][Equal] = State4;
		table[S1][Delim] = State5;
		table[S1][Colon] = State6;

		//set the number row
		table[S2][Letter] = NumTk;
		table[S2][Number] = State2;
		table[S2][Eof] = NumTk;
		table[S2][Equal] = NumTk;
		table[S2][Delim] = NumTk;
		table[S2][Space] = NumTk;
		table[S2][Colon] = NumTk;
		
		//set the identifier row
		table[S3][Letter] = State3;
		table[S3][Number] = State3;
		table[S3][Eof] = IdTk;
		table[S3][Equal] = IdTk;
		table[S3][Delim] = IdTk;
		table[S3][Space] = IdTk;	
		table[S3][Colon] = IdTk;

		//set the equal row
		table[S4][Letter] = AsgErrTk;
	        table[S4][Number] = AsgErrTk;
	        table[S4][Eof] = AsgErrTk;
	        table[S4][Delim] = AsgErrTk;
	        table[S4][Space] = AsgErrTk;
		table[S4][Equal] = DelTk;
		table[S4][Colon] = AsgErrTk;

		//set the delim row
		table[S5][Letter] = DelTk;
                table[S5][Number] = DelTk;
                table[S5][Eof] = DelTk;
                table[S5][Equal] = DelTk;
                table[S5][Delim] = DelTk;
                table[S5][Space] = DelTk;
		table[S5][Colon] = DelTk;

		//set the colon row	
		table[S6][Letter] = DelTk;
                table[S6][Number] = DelTk;
                table[S6][Eof] = DelTk;
                table[S6][Equal] = AsgTk;
                table[S6][Delim] = DelTk;
                table[S6][Space] = DelTk;
		table[S6][Colon] = DelTk;	
	}
	struct Token getCmtErrTk(int lineNum){
		struct Token tk;
		tk.line = lineNum;
		tk.tkId = CmtErrTk;
		tk.instance = "Comment Error Token";
		return tk;
	}
	struct Token getEOFTk(int lineNum){
		struct Token tk;
                tk.line = lineNum;
		tk.tkId = EofTk;
		tk.instance = "EOF";
		return tk;
	}
	struct Token getToken(char ch, TokenId state,string instance,int line){
		if(ch == ' ' || ch == '\n' || ch == '\t'){
			return getState(state,Space,instance,line,ch);
		}
		if(ch == '#'){
			//we need to ignore this and just take the token before.
			return getState(state,Space,instance,line,ch);
		}
		if(isalpha(ch)){
			return getState(state,Letter,instance,line,ch);
		}
		if(isdigit(ch)){
			return getState(state,Number,instance,line,ch);
		}
		std::set<char> delimeters = {'<','>','+','-','*','/','%','.','(',')',',','{','}',';',']','['};
		if(delimeters.count(ch) != 0){
			return getState(state,Delim,instance,line,ch);
		}
		if(ch == '='){
			return getState(state,Equal,instance,line,ch);
		}
		if(ch == ':'){
			return getState(state,Colon,instance,line,ch);
		}
		//else just call with other column for error.
		struct Token tk;
		tk.tkId = table[state][Extra];
		tk.instance = instance + ch;
		tk.line = line;
		return tk;
	}
	struct Token getState(int row, int col, string instance, int line,char ch){
		struct Token tk;
		//std::cout << "table:" << row << ", " << col << std::endl;
		tk.tkId = table[row][col];
		if((tk.tkId > 1000 || ch == '\n' || tk.tkId == AsgErrTk) && tk.tkId != AsgTk && ch != '='){
			tk.instance = instance;
		}
		else{
			tk.instance = instance + ch;
		}
		if(tk.tkId == IdTk){
                        /*Check instance is not a keyword.
 			*label goto loop void declare
 			*return in out program iffy then assign data*/
                        if(tk.instance.compare("label") == 0){
                                tk.tkId = LabelTk;
                        }
			else if(tk.instance.compare("goto") == 0){
				tk.tkId = GotoTk;
			}
			else if(tk.instance.compare("loop") == 0){
                                tk.tkId = LoopTk;
                        }
                        else if(tk.instance.compare("void") == 0){
                                tk.tkId = VoidTk;
                        }
                        else if(tk.instance.compare("declare") == 0){
                                tk.tkId = DeclareTk;
                        }
                        else if(tk.instance.compare("return") == 0){
                               tk.tkId = ReturnTk;
                        }
                        else if(tk.instance.compare("in") == 0){
                                tk.tkId = InTk;
                        }
                        else if(tk.instance.compare("out") == 0){
                                tk.tkId = OutTk;
                        }
                        else if(tk.instance.compare("iffy") == 0){
                                tk.tkId = IffyTk;
                        }
                        else if(tk.instance.compare("then") == 0){
                                tk.tkId = ThenTk;
                        }
                        else if(tk.instance.compare("assign") == 0){
                                tk.tkId = AssignTk;
                        }
                        else if(tk.instance.compare("data") == 0){
                                tk.tkId = DataTk;
                        }
                        else if(tk.instance.compare("program") == 0){
                                tk.tkId = ProgramTk;
                        }
                }
		else if(tk.tkId == DelTk){
			if(tk.instance.compare("{") == 0){
                                tk.tkId = BeginBlockTk;
                        }
			else if(tk.instance.compare("(") == 0){
                                tk.tkId = BeginParenTk;
                        }
                        else if(tk.instance.compare("*") == 0){
                                tk.tkId = MultiplyTk;
                        }
                        else if(tk.instance.compare("<") == 0){
                                tk.tkId = LessThanTk;
                        }
                        else if(tk.instance.compare(">") == 0){
                                tk.tkId = GreaterThanTk;
                        }
                        else if(tk.instance.compare("==") == 0){
                                tk.tkId = IsEqualTk;
                        }

	
              	}


		tk.line = line;
		return tk;
	}
	void print(){
		for(int i = 0; i < ROWS; i++){
			cout << endl;
			for(int j = 0; j < COLS; j++){
				cout << table[i][j] << ", ";
				//printf("%d",table[i][j]);
			}
		}
	
	}

};
#endif
