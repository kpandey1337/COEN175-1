/*
 * File:	parser.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the recursive-descent parser for
 *		Simple C.
 */

# include <cstdlib>
# include <iostream>
# include "checker.h"
# include "tokens.h"
# include "lexer.h"

using namespace std;

static int lookahead;
static string lexbuf;

static Type expression(bool& lvalue);
static void statement(Symbol& function);


/*
 * Function:	error
 *
 * Description:	Report a syntax error to standard error.
 */

static void error()
{
    if (lookahead == DONE)
	report("syntax error at end of file");
    else
	report("syntax error at '%s'", lexbuf);

    exit(EXIT_FAILURE);
}


/*
 * Function:	match
 *
 * Description:	Match the next token against the specified token.  A
 *		failure indicates a syntax error and will terminate the
 *		program since our parser does not do error recovery.
 */

static void match(int t)
{
    if (lookahead != t)
	error();

    lookahead = lexan(lexbuf);
}


/*
 * Function:	integer
 *
 * Description:	Match the next token as an integer and return its value.
 */

static unsigned integer()
{
    string buf;


    buf = lexbuf;
    match(INTEGER);
    return strtoul(buf.c_str(), NULL, 0);
}


/*
 * Function:	identifier
 *
 * Description:	Match the next token as an identifier and return its name.
 */

static string identifier()
{
    string buf;


    buf = lexbuf;
    match(ID);
    return buf;
}


/*
 * Function:	isSpecifier
 *
 * Description:	Return whether the given token is a type specifier.
 */

static bool isSpecifier(int token)
{
    return token == CHAR || token == INT || token == DOUBLE;
}


/*
 * Function:	specifier
 *
 * Description:	Parse a type specifier.  Simple C has char, int, and long.
 *
 *		specifier:
 *		  char
 *		  int
 *		  double
 */

static int specifier()
{
    int typespec = ERROR;


    if (isSpecifier(lookahead)) {
	typespec = lookahead;
	match(lookahead);
    } else
	error();

    return typespec;
}


/*
 * Function:	pointers
 *
 * Description:	Parse pointer declarators (i.e., zero or more asterisks).
 *
 *		pointers:
 *		  empty
 *		  * pointers
 */

static unsigned pointers()
{
    unsigned count = 0;


    while (lookahead == '*') {
	match('*');
	count ++;
    }

    return count;
}


/*
 * Function:	declarator
 *
 * Description:	Parse a declarator, which in Simple C is either a scalar
 *		variable or an array, with optional pointer declarators.
 *
 *		declarator:
 *		  pointers identifier
 *		  pointers identifier [ integer ]
 */

static void declarator(int typespec)
{
    unsigned indirection;
    string name;


    indirection = pointers();
    name = identifier();

    if (lookahead == '[') {
	match('[');
	declareVariable(name, Type(typespec, indirection, integer()));
	match(']');

    } else
	declareVariable(name, Type(typespec, indirection));
}


/*
 * Function:	declaration
 *
 * Description:	Parse a local variable declaration.  Global declarations
 *		are handled separately since we need to detect a function
 *		as a special case.
 *
 *		declaration:
 *		  specifier declarator-list ;
 *
 *		declarator-list:
 *		  declarator
 *		  declarator , declarator-list
 */

static void declaration()
{
    int typespec;


    typespec = specifier();
    declarator(typespec);

    while (lookahead == ',') {
	match(',');
	declarator(typespec);
    }

    match(';');
}


/*
 * Function:	declarations
 *
 * Description:	Parse a possibly empty sequence of declarations.
 *
 *		declarations:
 *		  empty
 *		  declaration declarations
 */

static void declarations()
{
    while (isSpecifier(lookahead))
		declaration();
}


/*
 * Function:	primaryExpression
 *
 * Description:	Parse a primary expression.
 *
 *		primary-expression:
 *		  ( expression )
 *		  identifier ( expression-list )
 *		  identifier ( )
 *		  identifier
 *		  string
 *		  integer
 *		  real
 *
 *		expression-list:
 *		  expression
 *		  expression , expression-list
 */

static Type primaryExpression(bool lparenMatched, bool& lvalue)
{
	Type left; 
	Type right;
	Symbol* sym;

    if (lparenMatched) {
		left = expression(lvalue);
		match(')');

    } else if (lookahead == STRING) {
		left = Type(CHAR, 1, lexbuf.length()-2); //arrayOfChar
		lvalue = false;

		match(STRING);

    } else if (lookahead == INTEGER) {
		match(INTEGER);

		left = Type(INT);
		lvalue = false;

    } else if (lookahead == REAL) {
		match(REAL);

		left = Type(DOUBLE);
		lvalue = false;

    } else if (lookahead == ID) {
		string name = identifier();

		Parameters* arguments = new Parameters();
		Type tmp_type;
		bool tmp_lvalue;

		if (lookahead == '(') {
		    match('(');

		    if (lookahead != ')') {
				tmp_type = expression(tmp_lvalue);
				
				arguments->push_back(tmp_type);

				while (lookahead == ',') {
				    match(',');
				    tmp_type = expression(tmp_lvalue);
				
					arguments->push_back(tmp_type);
				}
		    }

		    sym = checkFunction(name);
		    left = checkFunctionType(*sym, arguments);
		    lvalue = false;

		    match(')');


		} else{
	   		sym = checkIdentifier(name);
	    	left = checkIdentifierType(sym->type(), lvalue);
	    }

    } else
		error();


	return left;
}


/*
 * Function:	postfixExpression
 *
 * Description:	Parse a postfix expression.
 *
 *		postfix-expression:
 *		  primary-expression
 *		  postfix-expression [ expression ]
 */

static Type postfixExpression(bool lparenMatched, bool& lvalue)
{
    Type left = primaryExpression(lparenMatched, lvalue);
    Type right;

    while (lookahead == '[') {
		match('[');
		right = expression(lvalue);
		left = checkIndex(left, right);
		match(']');
    }

    lvalue = true;
    return left;
}


/*
 * Function:	prefixExpression
 *
 * Description:	Parse a prefix expression.
 *
 *		prefix-expression:
 *		  postfix-expression
 *		  ! prefix-expression
 *		  - prefix-expression
 *		  * prefix-expression
 *		  & prefix-expression
 *		  sizeof ( specifier pointers )
 *		  ( specifier pointers ) prefix-expression
 */

static Type prefixExpression(bool& lvalue)
{
	Type left;

    if (lookahead == '!') {
		match('!');
		left = prefixExpression(lvalue);

		left = checkNot(left);
		lvalue = false;

    } else if (lookahead == '-') {
		match('-');
		left = prefixExpression(lvalue);

		left = checkNegate(left);
		lvalue = false;

    } else if (lookahead == '*') {
		match('*');
		left = prefixExpression(lvalue);

		left = checkDereference(left);
		lvalue = true;

    } else if (lookahead == '&') {
		match('&');
		left = prefixExpression(lvalue);

		left = checkAddress(left, lvalue);
		lvalue = false;

    } else if (lookahead == SIZEOF) {
		match(SIZEOF);
		match('(');
		specifier();
		pointers();
		match(')');

		left = checkSizeOf(left);
		lvalue = false;

    } else if (lookahead == '(') {
		match('(');

		if (isSpecifier(lookahead)) {
		    int typespec = specifier();
		    unsigned indirection = pointers();
		    match(')');
		    left = prefixExpression(lvalue);

		    left = checkTypeCast(left, typespec, indirection);
		    lvalue = false;

		} else{
		    left = postfixExpression(true, lvalue);
		    left = left.promote();
		}

    } else{
		left = postfixExpression(false, lvalue);
		left = left.promote();
    }



	return left;
}


/*
 * Function:	multiplicativeExpression
 *
 * Description:	Parse a multiplicative expression.
 *
 *		multiplicative-expression:
 *		  prefix-expression
 *		  multiplicative-expression * prefix-expression
 *		  multiplicative-expression / prefix-expression
 *		  multiplicative-expression % prefix-expression
 */

static Type multiplicativeExpression(bool& lvalue)
{
    Type left = prefixExpression(lvalue);
    Type right;

    while (1) {
	if (lookahead == '*') {
	    match('*');
	    right = prefixExpression(lvalue);

	    left = checkMultiply(left, right);
	    lvalue = false;

	} else if (lookahead == '/') {
	    match('/');
	    right = prefixExpression(lvalue);

	    left = checkDivide(left, right);
	    lvalue = false;

	} else if (lookahead == '%') {
	    match('%');
	    right = prefixExpression(lvalue);

	    left = checkModulo(left, right);
	    lvalue = false;

	} else
	    break;
    }

    return left;
}


/*
 * Function:	additiveExpression
 *
 * Description:	Parse an additive expression.
 *
 *		additive-expression:
 *		  multiplicative-expression
 *		  additive-expression + multiplicative-expression
 *		  additive-expression - multiplicative-expression
 */

static Type additiveExpression(bool& lvalue)
{
   	Type left = multiplicativeExpression(lvalue);
   	Type right;

    while (1) {
		if (lookahead == '+') {
		    match('+');
		    right = multiplicativeExpression(lvalue);

		    left = checkAdd(left, right);
			lvalue = false;

		} else if (lookahead == '-') {
		    match('-');
		    right = multiplicativeExpression(lvalue);

		    left = checkSub(left, right);
		    lvalue = false;

		} else
		    break;
    }

    return left;
}


/*
 * Function:	relationalExpression
 *
 * Description:	Parse a relational expression.  Note that Simple C does not
 *		have shift operators, so we go immediately to additive
 *		expressions.
 *
 *		relational-expression:
 *		  additive-expression
 *		  relational-expression < additive-expression
 *		  relational-expression > additive-expression
 *		  relational-expression <= additive-expression
 *		  relational-expression >= additive-expression
 */

static Type relationalExpression(bool& lvalue)
{
    Type left = additiveExpression(lvalue);
    Type right;

    while (1) {
		if (lookahead == '<') {
		    match('<');
		    right = additiveExpression(lvalue);

		    left = checkLessThan(left, right);
		    lvalue = false;

		} else if (lookahead == '>') {
		    match('>');
		    right = additiveExpression(lvalue);

		    left = checkGreaterThan(left,right);
		    lvalue = false;

		} else if (lookahead == LEQ) {
		    match(LEQ);
		    right = additiveExpression(lvalue);

		    left = checkLessThanEqual(left,right);
		    lvalue = false;

		} else if (lookahead == GEQ) {
		    match(GEQ);
		    right = additiveExpression(lvalue);

		    left = checkGreaterThanEqual(left,right);
		    lvalue = false;

		} else
		    break;
    }

    return left;
}


/*
 * Function:	equalityExpression
 *
 * Description:	Parse an equality expression.
 *
 *		equality-expression:
 *		  relational-expression
 *		  equality-expression == relational-expression
 *		  equality-expression != relational-expression
 */

static Type equalityExpression(bool& lvalue)
{
    Type left = relationalExpression(lvalue);
    Type right;

    while (1) {
		if (lookahead == EQL) {
		    match(EQL);
		    right = relationalExpression(lvalue);

		    left = checkEquals(left, right);
		    lvalue = false;

		} else if (lookahead == NEQ) {
		    match(NEQ);
		    right = relationalExpression(lvalue);

		    left = checkNotEquals(left, right);
		    lvalue = false;

		} else{
		    break;
		}
	}

	return left;
}


/*
 * Function:	logicalAndExpression
 *
 * Description:	Parse a logical-and expression.  Note that Simple C does
 *		not have bitwise-and expressions.
 *
 *		logical-and-expression:
 *		  equality-expression
 *		  logical-and-expression && equality-expression
 */

static Type logicalAndExpression(bool& lvalue)
{
    Type left = equalityExpression(lvalue);

    while (lookahead == AND) {
		match(AND);
		Type right = equalityExpression(lvalue);

		left = checkLogicalAnd(left, right);
		lvalue = false;
    }

    return left;
}


/*
 * Function:	expression
 *
 * Description:	Parse an expression, or more specifically, a logical-or
 *		expression, since Simple C does not allow comma or
 *		assignment as an expression operator.
 *
 *		expression:
 *		  logical-and-expression
 *		  expression || logical-and-expression
 */

static Type expression(bool& lvalue)
{
    Type left = logicalAndExpression(lvalue);

    while (lookahead == OR) {
		match(OR);
		Type right = logicalAndExpression(lvalue);

		left = checkLogicalOr(left, right);
		lvalue = false;
    }

    return left;
}


/*
 * Function:	statements
 *
 * Description:	Parse a possibly empty sequence of statements.  Rather than
 *		checking if the next token starts a statement, we check if
 *		the next token ends the sequence, since a sequence of
 *		statements is always terminated by a closing brace.
 *
 *		statements:
 *		  empty
 *		  statement statements
 */

static void statements(Symbol& function)
{
    while (lookahead != '}')
		statement(function);
}


/*
 * Function:	statement
 *
 * Description:	Parse a statement.  Note that Simple C has so few
 *		statements that we handle them all in this one function.
 *
 *		statement:
 *		  { declarations statements }
 *		  return expression ;
 *		  while ( expression ) statement
 *		  if ( expression ) statement
 *		  if ( expression ) statement else statement
 *		  expression = expression ;
 *		  expression ;
 */

static void statement(Symbol& function)
{
	bool lvalue, lvalue_preserved;
	Type left, right;

    if (lookahead == '{') {
		match('{');
		openScope();
		declarations();
		statements(function);
		closeScope();
		match('}');

    } else if (lookahead == RETURN) {
		match(RETURN);
		left = expression(lvalue);

		left = checkReturnType(left, function);
		match(';');

    } else if (lookahead == WHILE) {
		match(WHILE);
		match('(');
		left = expression(lvalue);

		left = checkWhile(left);
		match(')');
		statement(function);

    } else if (lookahead == IF) {
		match(IF);
		match('(');
		left = expression(lvalue);

		left = checkIf(left);
		match(')');
		statement(function);

	if (lookahead == ELSE) {
	    match(ELSE);
	    statement(function);
	}

    } else {
		left = expression(lvalue);
		lvalue_preserved = lvalue;

		if (lookahead == '=') {
		    match('=');
		    right = expression(lvalue);

		    left = checkAssignment(left, right, lvalue_preserved);
		}

		match(';');
    }
}


/*
 * Function:	parameter
 *
 * Description:	Parse a parameter, which in Simple C is always a scalar
 *		variable with optional pointer declarators.
 *
 *		parameter:
 *		  specifier pointers ID
 */

static Type parameter()
{
    int typespec;
    unsigned indirection;
    string name;


    typespec = specifier();
    indirection = pointers();
    name = identifier();

    Type type = Type(typespec, indirection);
    declareVariable(name, type);
    return type;
}


/*
 * Function:	parameters
 *
 * Description:	Parse the parameters of a function, but not the opening or
 *		closing parentheses.
 *
 *		parameters:
 *		  void
 *		  parameter-list
 *
 *		parameter-list:
 *		  parameter
 *		  parameter , parameter-list
 */

static Parameters *parameters()
{
    Parameters *params;


    openScope();
    params = new Parameters();

    if (lookahead == VOID)
	match(VOID);

    else {
	params->push_back(parameter());

	while (lookahead == ',') {
	    match(',');
	    params->push_back(parameter());
	}
    }

    return params;
}


/*
 * Function:	globalDeclarator
 *
 * Description:	Parse a declarator, which in Simple C is either a scalar
 *		variable, an array, or a function, with optional pointer
 *		declarators.
 *
 *		global-declarator:
 *		  pointers identifier
 *		  pointers identifier ( parameters )
 *		  pointers identifier [ integer ]
 */

static void globalDeclarator(int typespec)
{
    unsigned indirection;
    string name;


    indirection = pointers();
    name = identifier();

    if (lookahead == '(') {
	match('(');
	declareFunction(name, Type(typespec, indirection, parameters()));
	closeScope();
	match(')');

    } else if (lookahead == '[') {
	match('[');
	declareVariable(name, Type(typespec, indirection, integer()));
	match(']');

    } else
	declareVariable(name, Type(typespec, indirection));
}


/*
 * Function:	remainingDeclarators
 *
 * Description:	Parse any remaining global declarators after the first.
 *
 * 		remaining-declarators
 * 		  ;
 * 		  , global-declarator remaining-declarators
 */

static void remainingDeclarators(int typespec)
{
    while (lookahead == ',') {
	match(',');
	globalDeclarator(typespec);
    }

    match(';');
}


/*
 * Function:	globalOrFunction
 *
 * Description:	Parse a global declaration or function definition.
 *
 * 		global-or-function:
 * 		  specifier pointers identifier remaining-decls
 * 		  specifier pointers identifier [ integer ] remaining-decls
 * 		  specifier pointers identifier ( parameters ) remaining-decls 
 * 		  specifier pointers identifier ( parameters ) { ... }
 */

static void globalOrFunction()
{
    int typespec;
    unsigned indirection;
    Parameters *params;
    string name;

    Symbol* function;

    typespec = specifier();
    indirection = pointers();
    name = identifier();

    if (lookahead == '[') {
		match('[');
		declareVariable(name, Type(typespec, indirection, integer()));
		match(']');
		remainingDeclarators(typespec);

    } else if (lookahead == '(') {
		match('(');
		params = parameters();
		match(')');

		if (lookahead == '{') {
		    function = defineFunction(name, Type(typespec, indirection, params));
		    match('{');
		    declarations();
		    statements(*function);
		    closeScope();
		    match('}');

		} else {
		    closeScope();
		    declareFunction(name, Type(typespec, indirection, params));
		    remainingDeclarators(typespec);
		}

    } else {
		declareVariable(name, Type(typespec, indirection));
		remainingDeclarators(typespec);
    }
}


/*
 * Function:	main
 *
 * Description:	Analyze the standard input stream.
 */

int main()
{
    openScope();
    lookahead = lexan(lexbuf);

    while (lookahead != DONE)
	globalOrFunction();

    closeScope();
    exit(EXIT_SUCCESS);
}
