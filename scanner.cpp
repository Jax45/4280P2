//this is going to be the scanner source file.
//it will contain a function that when called will
//return 1 token from the vector of strings given to it.
//it will keep track of its spot in the file with static variables
//and keep returning tokens.
#include "scanner.h"
struct Token scanner(ifstream& file){
	//static bool eofReached = false;
	char ch;
	//static bool firstRun = true;
	static int lineNum = 0;
	//get the table class
	FSATable table = FSATable();
	//grab the first char
	//if(firstRun){
	//	firstRun = false;
		if(!(file.get(ch))){
			return table.getEOFTk(lineNum);
		}
	//}
	//if(eofReached){
	//	return table.getEOFTk(lineNum);
	//}
	//filter out comments,newline,ws,tabs
	while(ch == '#' || ch == '\t' || ch == '\n' || ch == ' '){
		if(ch == '#'){
			file.get(ch);
			while(ch != '#'){
				if(ch == '\n'){
					lineNum++;
				}
				if(!(file.get(ch))){
                        		return table.getCmtErrTk(lineNum);
                		}
			}
			if(!(file.get(ch))){
        	                return table.getEOFTk(lineNum);
	                }
		}
		else{
	                if(ch == '\n'){
	                        lineNum++;
                        }
			if(!(file.get(ch))){
	                        return table.getEOFTk(lineNum);
	                }
		}
	}
		
	//get a token with that char.
	struct Token tk = table.getToken(ch,State1,"",lineNum);
	//while it is not an accepting state
	while(tk.tkId < 1000 && tk.tkId > 0){
                if(ch == '\n'){
                	lineNum++;
                }
		if(!(file.get(ch))){
	//		eofReached = true;
			return table.getEOFTk(lineNum);
                }
		tk = table.getToken(ch,tk.tkId,tk.instance,lineNum);
	}
	/*if the token is an error but not an assignment error, then
 * 	throw out the next token.*/
	if(tk.tkId < 0 && tk.tkId != AsgErrTk){
		file.get(ch);
	}
	/*Only unget the character if it is
 * 	 not a double delimiter. like := and == */
	if(tk.instance.compare(":=") != 0 && tk.instance.compare("==") != 0){
		file.unget();
	}
	return tk;
}

