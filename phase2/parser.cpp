#include <iostream>
#include "token.h"
#include "lexer.h"

using namespace std;

int lookahead = 0;
int nexttoken = 0;

string lexbuf;
string nextbuf;

int prints_enabled = 0;

/* =-=-=-=-= */
/* Prototypes */
/* =-=-=-=-= */

void error();
void match(int token);
void expr();
void exprDetermineCastPrefix();

/* =-=-=-=-= */
/* Utilities */
/* =-=-=-=-= */

void match(int token){
	if(prints_enabled)
		cout << "\tMatch-- argument: " << token << " lookahead: " << token << endl;

	if(lookahead != token){
		error();
	}
	if(nexttoken != 0){ 
		//handle the cases where we need to peekAhead() (cast vs prefix, ..)
		//such that next token comes from my local buffer before grabbing from lexer again
		lookahead = nexttoken;
		nexttoken = 0;

		lexbuf = nextbuf;
	}
	else{
		lookahead = lexan(lexbuf);

		if(prints_enabled)
		cout << "\tDONE Lexan()" << endl;
	}
		if(prints_enabled)
		cout << "\tDONE Matching-- argument: " << token << " token: " << token << endl;
}

void error(){
    if (lookahead == DONE){
		report("syntax error at end of file");
	}
	else{
		report("syntax error at '%s'", lexbuf);
	}
	exit(0);
}

// ????????????????????????????????????????????????
//What about: CONST, UNSIGNED, SIGNED?
// ????????????????????????????????????????????????

bool isSpecifier(int token){
	switch(token){
		case CHAR:
		case INT:
		case DOUBLE:
			return 1;
		default:
			return 0;

	}
}

int peekAhead(){
	if(nexttoken == 0){ 
		//will make multiple calls to check for valid specifiers; cannot peek more than once from the input stream
		//REMINDER - set nexttoken to 0 when done for desired behavior *******
		nexttoken = lexan(nextbuf);
	}
	return nexttoken;
}

/* =-=-=-=-= */
/* Expressions */
/*

OR -> AND -> EQ/NEQ -> LT, LTE, GT, GTE -> +/- -> *,/,% -> [] -> id, id(), id(expr-list), real, string, integer, (expr)

*/
/* =-=-=-=-= */
void pointers(){
	if(prints_enabled == 1)
		cout << "In pointers .." << endl; //**###
	while(lookahead == ASTERISK){
		match(ASTERISK);
	}
}

void specifier(){
	if(prints_enabled == 1)
		cout << "In specifier .." << endl; //**###
	if(isSpecifier(lookahead)){
		//can't trust match for error collection here
		//need to ensure that specifier comes after cast LEFTPAREN
		//if so match it exactly once, more than one specifier will be caught by pointers()
		match(lookahead);
	}
	else{
		error();
	}
}

void exprList(){
	if(prints_enabled == 1)
		cout << "In exprList .." << endl; //**###
	expr();

	while(lookahead == COMMA){
		match(COMMA);
		expr();
	}
}

void exprResolved(){
	if(prints_enabled == 1)
		cout << "In exprResolved ... lookahead: " << lookahead << endl; //**###
	switch(lookahead){
		case INTEGER:
			match(INTEGER);
			break;
		case REAL:
			match(REAL);
			break;
		case STRING:
			match(STRING);
			break;
		case LEFTPAREN:
			match(LEFTPAREN);
			expr();
			match(RIGHTPAREN);
			break;
		case ID:
			if(prints_enabled == 1)
				cout << "matching ID" << endl;
			match(ID); 
			//have ID
			//must handle cases: ID, ID(), ID(expr-list)

			if(lookahead == LEFTPAREN){
				//have ID(
				match(LEFTPAREN);

				if(lookahead != RIGHTPAREN){
					//have ID( ...
					exprList();
				}

				match(RIGHTPAREN);

			}

			break;
		default:
			error();
			break;
	}

	if(prints_enabled == 1)
		cout << "Leaving exprResolved" << endl;
}

void exprPostfix(){
	if(prints_enabled == 1)
		cout << "In exprPostfix.." << endl; //**###
	exprResolved(); //this has to resolve to an indexable? semantically anyways

	while(lookahead == LEFTBRACKET){
		match(LEFTBRACKET);
		expr();
		match(RIGHTBRACKET);
		cout << "index" << endl;
	}



}


//Do i need to handle the case where multiple -'s? or &&&&a
void exprPrefix(){
	if(prints_enabled == 1)
		cout << "In exprPrefix .." << endl; //**###
	switch(lookahead){
		case AMP:
			match(AMP);
			exprPrefix();
			cout << "ampersand" << endl;
			break;
		case ASTERISK:
			match(ASTERISK);
			exprPrefix();
			cout << "asterisk" << endl;
			break;
		case NOT:
			match(NOT);
			exprPrefix();
			cout << "not" << endl;
			break;
		case MINUS:
			match(MINUS);
			exprPrefix();
			cout << "minus" << endl;
			break;
		case SIZEOF:
			match(SIZEOF);

			match(LEFTPAREN);
			specifier();
			pointers();
			match(RIGHTPAREN);

			cout << "sizeof" << endl;
			break;
		default:
			exprPostfix();

	}
}

void exprCast(){
	if(prints_enabled == 1)
		cout << "In exprCast .." << endl; //**###
	
	if(lookahead == LEFTPAREN && isSpecifier(peekAhead())){
		match(LEFTPAREN); //handles the LEFTPAREN and moves the token peekAhead() buffered to lookahead
		specifier();
		pointers();
		match(RIGHTPAREN);
		exprDetermineCastPrefix();
		cout << "cast" << endl;
	}
}

void exprDetermineCastPrefix(){

	int x = peekAhead();

	if(prints_enabled == 1)
		cout << "In exprDetermineCastPrefix .. lookahead = "<< lookahead <<" peekAhead = " << x << endl; //**###

	if(lookahead == LEFTPAREN && isSpecifier(peekAhead())){
		exprCast();
	}
	else{
		exprPrefix();
	}

}

void exprMultDivMod(){
	if(prints_enabled == 1)
		cout << "In exprMultDivMod .." << endl; //**###
	exprDetermineCastPrefix();

	while(1){
		if(lookahead == ASTERISK){
			match(ASTERISK);
			exprDetermineCastPrefix();
			cout << "mult" << endl;
		}
		else if(lookahead == SLASH){
			match(SLASH);
			exprDetermineCastPrefix();
			cout << "div" << endl;
		}
		else if(lookahead == PERCENT){
			match(PERCENT);
			exprDetermineCastPrefix();
			cout << "mod" << endl;
		}
		else{
			break;
		}
	}
}

void exprAddSub(){
	if(prints_enabled == 1)
		cout << "In exprAddSub .." << endl; //**###
	exprMultDivMod();

	while(1){
		if(lookahead == PLUS){
			match(PLUS);
			exprMultDivMod();
			cout << "add" << endl;
			continue;
		}
		if(lookahead == MINUS){
			match(MINUS);
			exprMultDivMod();
			cout << "sub" << endl;
			continue;
		}
		else{
			break;
		}
	}
}

void exprQuantComparison(){
	if(prints_enabled == 1)
		cout << "In exprQuantComparison .." << endl; //**###
	exprAddSub();

	while(1){
		if(lookahead == LT){
			match(LT);
			exprAddSub();
			cout << "lessThan" << endl;
		}
		else if(lookahead == LTE){
			match(LTE);
			exprAddSub();
			cout << "lessThanEqual" << endl;
		}
		else if(lookahead == GT){
			match(GT);
			exprAddSub();
			cout << "greaterThan" << endl;
		}
		else if(lookahead == GTE){
			match(GTE);
			exprAddSub();
			cout << "greaterThanEqual" << endl;
		}
		else{
			break;
		}
	}
}

void exprEquality(){
	if(prints_enabled == 1)
		cout << "In exprEquality .." << endl; //**###
	exprQuantComparison();

	while(1){ 
		if(lookahead == EQUALS){
			match(EQUALS);
			exprQuantComparison();
			cout << "equals" << endl;
		}
		else if(lookahead == NE){
			match(NE);
			exprQuantComparison();
			cout << "notEqual" << endl;
		}
		else{
			break;
		}
	}
}

void exprAnd(){
	if(prints_enabled == 1)
		cout << "In exprAnd .." << endl; //**###
	exprEquality();

	while(lookahead == AND){
		match(AND);
		exprEquality();
		cout << "and" << endl;
	}
}

void exprOr(){
	if(prints_enabled == 1)
		cout << "In exprOr .." << endl; //**###
	exprAnd();

	while(lookahead == OR){
		match(OR);
		exprAnd();
		cout << "or" << endl;
	}
}

void expr(){
	if(prints_enabled == 1)
		cout << "In expr .." << endl; //**###
	
	exprOr();

}

/* =-=-=-=-= */

void determineGlobalOrFunction(){
	return;
}


int main(int argc, char* argv[]){

	if(argc == 2)
		prints_enabled = atoi(argv[1]);

	if(prints_enabled == 1)
		cout << "Entering main.." << endl; //**###

	lookahead = lexan(lexbuf);

	if(prints_enabled)
		cout << "Entering while.." << endl;

	while (lookahead != DONE){
		if(prints_enabled == 1)
			cout << "In while .." << endl; //**###
		//determineGlobalOrFunction();
		expr();

	}

	return 0;

}