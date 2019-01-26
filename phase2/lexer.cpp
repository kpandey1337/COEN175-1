/*
 * File:	lexer.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the lexical analyzer for Simple C.
 *
 *		Extra functionality:
 *		- checking for improper integer and floating-point literals
 */

# include <set>
# include <cstdio>
# include <cctype>
# include <cerrno>
# include <string>
# include <cstdlib>
# include <iostream>
# include "lexer.h"

# include "token.h"

using namespace std;
int numerrors, lineno = 1;


/* Later, we will associate token values with each keyword */

static set<string> keywords = {
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "int",
    "long",
    "register",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while",
};


/*
 * Function:	report
 *
 * Description:	Report an error to the standard error prefixed with the
 *		line number.  We'll be using this a lot later with an
 *		optional string argument, but C++'s stupid streams don't do
 *		positional arguments, so we actually resort to snprintf.
 *		You just can't beat C for doing things down and dirty.
 */

void report(const string &str, const string &arg)
{
    char buf[1000];

    snprintf(buf, sizeof(buf), str.c_str(), arg.c_str());
    cerr << "line " << lineno << ": " << buf << endl;
    numerrors ++;
}


/*
 * Function:	lexan
 *
 * Description:	Read and tokenize the standard input stream.  The lexeme is
 *		stored in a buffer.
 */

int lexan(string &lexbuf)
{
    long val;
    static int c = cin.get();

    cout << "Inside lexan" << endl;

    /* The invariant here is that the next character has already been read
       and is ready to be classified.  In this way, we eliminate having to
       push back characters onto the stream, merely to read them again. */

    while (!cin.eof()) {
		lexbuf.clear();


		/* Ignore white space */

		while (isspace(c)) {
		    if (c == '\n')
			lineno ++;

		    c = cin.get();
		}


		/* Check for an identifier or a keyword */

		if (isalpha(c) || c == '_') {
		    do {
			lexbuf += c;
			c = cin.get();
		    } while (isalnum(c) || c == '_');

		    if (keywords.count(lexbuf) > 0){
		    		if(lexbuf == "auto")
		    			return AUTO;
		    		if(lexbuf == "break")
		    			return BREAK;
		    		if(lexbuf == "case")
		    			return CASE;
				    if(lexbuf == "char")
		    			return CHAR;
				    if(lexbuf == "const")
				    	return CONST;
				    if(lexbuf == "continue")
				    	return CONTINUE;
				    if(lexbuf == "default")
				    	return DEFAULT;
				    if(lexbuf == "do")
				    	return DO;
				    if(lexbuf == "double")
				    	return DOUBLE;
				    if(lexbuf == "else")
				    	return ELSE;
				    if(lexbuf == "enum")
				    	return ENUM;
				    if(lexbuf == "extern")
				    	return EXTERN;
				    if(lexbuf == "float")
				    	return FLOAT;
				   	if(lexbuf ==  "for")
				   		return FOR;
				    if(lexbuf == "goto")
				    	return GOTO;
				    if(lexbuf == "if")
				    	return IF;
				    if(lexbuf == "int")
				    	return INT;
				    if(lexbuf == "long")
				    	return LONG;
				    if(lexbuf == "register")
				    	return REGISTER;
				    if(lexbuf == "return")
				    	return RETURN;
				    if(lexbuf == "short")
				    	return SHORT;
				    if(lexbuf == "signed")
				    	return SIGNED;
				    if(lexbuf == "sizeof")
				    	return SIZEOF;
				    if(lexbuf == "static")
				    	return STATIC;
				    if(lexbuf == "struct")
				    	return STRUCT;
				    if(lexbuf == "switch")
				    	return SWITCH;
				    if(lexbuf == "typedef")
				    	return TYPEDEF;
				    if(lexbuf == "union")
				    	return UNION;
				    if(lexbuf == "unsigned")
				    	return UNSIGNED;
				    if(lexbuf == "void")
				    	return VOID;
				    if(lexbuf == "volatile")
				    	return VOLATILE;
				    if(lexbuf == "while")
				    	return WHILE;

		    }

		    cout << "returning ID" << endl;
		    return ID;


		/* Check for a number (integer or real). */

		} else if (isdigit(c)) {
		    do {
			lexbuf += c;
			c = cin.get();
		    } while (isdigit(c));

		    if (c != '.') {
			errno = 0;
			val = strtol(lexbuf.c_str(), NULL, 0);

			if (errno != 0 || val != (int) val)
			    report("integer constant too large");

			return INTEGER;
		    }

		    lexbuf += c;
		    c = cin.get();

		    if (isdigit(c)) {
			do {
			    lexbuf += c;
			    c = cin.get();
			} while (isdigit(c));

			if (c == 'e' || c == 'E') {
			    lexbuf += c;
			    c = cin.get();

			    if (c == '-' || c == '+') {
				lexbuf += c;
				c = cin.get();
			    }

			    if (isdigit(c)) {
				do {
				    lexbuf += c;
				    c = cin.get();
				} while (isdigit(c));
			    } else
				report("missing exponent of floating-point constant");
			}
		    } else
			report("missing fractional part of floating-point constant");

		    errno = 0;
		    strtod(lexbuf.c_str(), NULL);

		    if (errno != 0)
			report("floating-point constant out of range");

		    return REAL;


		/* There must be an easier way to do this.  It might seem stupid at
		   this point to recognize each token separately, but eventually
		   we'll be returning separate token values to the parser, so we
		   might as well do it now. */

		} else {
		    lexbuf += c;

		    switch(c) {


		    /* Check for '||' */

		    case '|':
			c = cin.get();

			if (c == '|') {
			    lexbuf += c;
			    c = cin.get();
			    return OR;
			}

			break;


		    /* Check for '=' and '==' */

		    case '=':
			c = cin.get();

			if (c == '=') {
			    lexbuf += c;
			    c = cin.get();
			    return EQUALS;
			}

			return ASSIGN;


		    /* Check for '&' and '&&' */

		    case '&':
			c = cin.get();

			if (c == '&') {
			    lexbuf += c;
			    c = cin.get();
			    return AND;
			}

			return AMP;


		    /* Check for '!' and '!=' */

		    case '!':
			c = cin.get();

			if (c == '=') {
			    lexbuf += c;
			    c = cin.get();
			    return NE;
			}

			return NOT;


		    /* Check for '<' and '<=' */

		    case '<':
			c = cin.get();

			if (c == '=') {
			    lexbuf += c;
			    c = cin.get();
			    return LTE;
			}

			return LT;


		    /* Check for '>' and '>=' */

		    case '>':
			c = cin.get();

			if (c == '=') {
			    lexbuf += c;
			    c = cin.get();
			    return GTE;
			}

			return GT;


		    /* Check for '-', '--', and '->' */

		    case '-':
			c = cin.get();

			if (c == '-') {
			    lexbuf += c;
			    c = cin.get();
			    return DECREMENT;
			} else if (c == '>') {
			    lexbuf += c;
			    c = cin.get();
			    return ARROW;
			}

			return MINUS;


		    /* Check for '+' and '++' */

		    case '+':
			c = cin.get();

			if (c == '+') {
			    lexbuf += c;
			    c = cin.get();
			    return INCREMENT;
			}

			return PLUS;


		    /* Check for simple, single character tokens */

		    case '*':
		    	c = cin.get();
		    	return ASTERISK;
		    case '%':
		    	c = cin.get();
		    	return PERCENT;
		    case ':':
		    	c = cin.get();
		    	return COLON;
		    case ';':
		    	c = cin.get();
		    	return SEMICOLON;
		    case '(':
		    	c = cin.get();
		    	return LEFTPAREN;
		    case ')':
		    	c = cin.get();
		    	return RIGHTPAREN;
		    case '[':
		    	c = cin.get();
		    	return LEFTBRACKET;
		    case ']':
		    	c = cin.get();
		    	return RIGHTBRACKET;
		    case '{':
		    	c = cin.get();
		    	return LEFTBRACE;
		    case '}':
		    	c = cin.get();
		    	return RIGHTBRACE;
		    case '.':
		    	c = cin.get();
		    	return DOT;
		    case ',':
				c = cin.get();
				return COMMA;


		    /* Check for '/' or a comment */

		    case '/':
			c = cin.get();

			if (c == '*') {
			    c = cin.get();

			    do {
				while (c != '*' && !cin.eof()) {
				    if (c == '\n')
					lineno ++;

				    c = cin.get();
				}

				c = cin.get();
			    } while (c != '/' && !cin.eof());

			    c = cin.get();
			    break;

			} else
			    return SLASH;


		    /* Check for a string literal */

		    case '"':
			c = cin.get();

			while (c != '"' && c != '\n' && !cin.eof()) {
			    lexbuf += c;
			    c = cin.get();
			}

			if (c == '\n' || cin.eof())
			    report("premature end of string literal");

			lexbuf += c;
			c = cin.get();
			return STRING;


		    /* Handle EOF here as well */

		    case EOF:
			return DONE;


		    /* Ignore everything else */

		    default:
			c = cin.get();
			break;
		    }
		}
    }

    return DONE;
}


/*
 * Function:	main
 *
 * Description:	Read and tokenize and standard input stream.
 */

/*

int main()
{
    int token;
    string lexbuf, type;

    while ((token = lexan(lexbuf)) != DONE) {
	switch(token) {
	case ID:
	    type = "identifier";
	    break;

	case INTEGER:
	    type = "integer";
	    break;

	case REAL:
	    type = "real";
	    break;

	case STRING:
	    type = "string";
	    break;

	case AUTO:
	case BREAK:
	case CASE:
	case CHAR:
	case CONST:
	case CONTINUE:
	case DEFAULT:
	case DO:
	case DOUBLE:
	case ELSE:
	case ENUM:
	case EXTERN:
	case FLOAT:
	case FOR:
	case GOTO:
	case IF:
	case INT:
	case LONG:
	case REGISTER:
	case RETURN:
	case SHORT:
	case SIGNED:
	case SIZEOF:
	case STATIC:
	case STRUCT:
	case SWITCH:
	case TYPEDEF:
	case UNION:
	case UNSIGNED:
	case VOID:
	case VOLATILE:
	case WHILE:
		type = "keyword";
		break;

	case AMP:
	case AND:
	case ARROW:
	case ASSIGN:
	case ASTERISK:
	case COLON:
	case COMMA:
	case DECREMENT:
	case DOT:
	case EQUALS:
	case GT:
	case GTE:
	case INCREMENT:
	case LEFTBRACE:
	case LEFTBRACKET:
	case LEFTPAREN:
	case LT:
	case LTE:
	case MINUS:
	case NE:
	case NOT:
	case PERCENT:
	case PLUS:
	case RIGHTBRACE:
	case RIGHTBRACKET:
	case RIGHTPAREN:
	case SEMICOLON:
	case SLASH:
	case OR:
		type = "operator";
		break;

	
	}

	cout << type << ":" << lexbuf << endl;
    }

    return 0;
}

*/
