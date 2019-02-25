/*
 * File:	checker.cpp
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the semantic checker for Simple C.
 *
 *		Extra functionality:
 *		- inserting an undeclared symbol with the error type
 */

# include <set>
# include <iostream>
# include "lexer.h"
# include "checker.h"
# include "tokens.h"
# include "Symbol.h"
# include "Scope.h"
# include "Type.h"

using namespace std;

static set<string> funcdefns;
static Scope *outermost, *toplevel;


static string redefined = "redefinition of '%s'";
static string redeclared = "redeclaration of '%s'";
static string conflicting = "conflicting types for '%s'";
static string undeclared = "'%s' undeclared";

#define E1 "invalid return type"
#define E2 "invalid type for test expression"
#define E3 "lvalue required in expression"
#define E4 "invalid operands to binary '%s'"
#define E5 "invalid operand to unary '%s'"
#define E6 "invalid operand in cast expression"
#define E7 "called object is not a function"
#define E8 "invalid arguments to called function"

static const Type error;
static Type integer(INT);
static Type real(DOUBLE);
static Type character(CHAR);


/*
 * Function:	openScope
 *
 * Description:	Create a scope and make it the new top-level scope.
 */

Scope *openScope()
{
    toplevel = new Scope(toplevel);

    if (outermost == nullptr)
	outermost = toplevel;

    return toplevel;
}


/*
 * Function:	closeScope
 *
 * Description:	Remove the top-level scope, and make its enclosing scope
 *		the new top-level scope.
 */

Scope *closeScope()
{
    Scope *old = toplevel;

    toplevel = toplevel->enclosing();
    return old;
}


/*
 * Function:	defineFunction
 *
 * Description:	Define a function with the specified NAME and TYPE.  A
 *		function is always defined in the outermost scope.  Any
 *		previous declaration is discarded.
 */

Symbol *defineFunction(const string &name, const Type &type)
{
    Symbol *symbol = outermost->find(name);

    if (funcdefns.count(name) > 0) {
	report(redefined, name);
	outermost->remove(name);
	delete symbol->type().parameters();
	delete symbol;
    }

    symbol = declareFunction(name, type);
    funcdefns.insert(name);
    return symbol;
}


/*
 * Function:	declareFunction
 *
 * Description:	Declare a function with the specified NAME and TYPE.  A
 *		function is always declared in the outermost scope.  Any
 *		previous declaration is discarded.
 */

Symbol *declareFunction(const string &name, const Type &type)
{
    Symbol *symbol = outermost->find(name);

    if (symbol != nullptr) {
	if (type != symbol->type())
	    report(conflicting, name);

	if (symbol->type().isFunction())
	    delete symbol->type().parameters();

	outermost->remove(name);
	delete symbol;
    }

    symbol = new Symbol(name, type);
    outermost->insert(symbol);
    return symbol;
}


/*
 * Function:	declareVariable
 *
 * Description:	Declare a variable with the specified NAME and TYPE.  Any
 *		previous declaration is discarded.
 */

Symbol *declareVariable(const string &name, const Type &type)
{
    Symbol *symbol = toplevel->find(name);

    if (symbol != nullptr) {
	if (outermost != toplevel)
	    report(redeclared, name);
	else if (type != symbol->type())
	    report(conflicting, name);

	if (symbol->type().isFunction())
	    delete symbol->type().parameters();

	toplevel->remove(name);
	delete symbol;
    }

    symbol = new Symbol(name, type);
    toplevel->insert(symbol);
    return symbol;
}


/*
 * Function:	checkIdentifier
 *
 * Description:	Check if NAME is declared.  If it is undeclared, then
 *		declare it as having the error type in order to eliminate
 *		future error messages.
 */

Symbol *checkIdentifier(const string &name)
{
    Symbol *symbol = toplevel->lookup(name);

    if (symbol == nullptr) {
	report(undeclared, name);
	symbol = new Symbol(name, error);
	toplevel->insert(symbol);
    }

    return symbol;
}


/*
 * Function:	checkFunction
 *
 * Description:	Check if NAME is a previously declared function.  If it is
 *		undeclared, then implicitly declare it.
 */

Symbol *checkFunction(const string &name)
{
    Symbol *symbol = toplevel->lookup(name);

    if (symbol == nullptr)
	   symbol = declareFunction(name, Type(INT, 0, nullptr));

    return symbol;
}




















/*  Phase 4  */

//primaryExpression

Type checkFunctionType(const Symbol& sym, Parameters* arguments){
    if(sym.type().isFunction()){

        Parameters* params = sym.type().parameters();
        //Check if parameters exists
        if(params != nullptr){


            //if defined and parameters match, return type of function
            if(params->size() == arguments->size() && *params == *arguments){ 

                //function returning T -> T (where T is defined by specifier and indirection)
                return( Type(sym.type().specifier(), sym.type().indirection() ) );
            }
            else{
                report(E8);
                return error;
            }
        }
        else{
            //If function not defined, return implicit function return type
            return(Type(INT, 0, nullptr));            
        }
    }
    report(E7);
    return error;
}

Type checkIdentifierType(const Type& left, bool& lvalue){
    if(left == error){
        return error;
    }
    
    if(left.isScalar()){
        lvalue = true;
    }
    else{
        lvalue = false;
    }
    return left;
}


//postfixExpression

Type checkIndex(const Type& left, const Type& right){
    if(left == error || right == error){
        return error;
    }
    
    Type lt = left.promote();
    Type rt = right.promote();

    if(lt.isPointer() && rt.isInteger()){
        return Type(lt.specifier(), lt.indirection()-1);
    }
    report(E4, "[]");
    return error;
}

//prefixExpression

Type checkNot(const Type& left){
    if(left == error){
        return error;
    }
    
    if(left.isPredicate()){
        return integer;
    }
    report(E5, "!");
    return error;
}

Type checkNegate(const Type& left){
    if(left == error){
        return error;
    }
    
    if(left.isNumeric()){
        return left;
    }
    report(E5, "-");
    return error;
}

Type checkDereference(const Type& left){
    if(left == error){
        return error;
    }
    
    if(left.isPointer()){
        return Type(left.specifier(), left.indirection()-1);
    }
    report(E5, "*");
    return error;
}

Type checkAddress(const Type& left, bool& lvalue){
    if(left == error){
        return error;
    }

    if(lvalue == true){
        return Type(left.specifier(), left.indirection()+1);
    }
    report(E3, "&");
    return error;
}

Type checkSizeOf(const Type& left){
    if(left == error){
        return error;
    }
    
    return integer;
}

Type checkTypeCast(const Type& left, int typespec, unsigned indirection){
    if(left == error){
        return error;
    }

    Type result = Type(typespec, indirection);

    if(result.isNumeric() && left.isNumeric()){
        return result;
    }
    else if(result.isPointer() && left.isPointer()){
        return result;
    }
    else if(result.isPointer() && left.isInteger()){
        return result;
    }
    else if(left.isPointer() && result.isInteger()){
        return result;
    }
    report(E6);
    return error;
}



//multiplicativeExpression

Type checkMultiply(const Type& left, const Type& right){
    if(left == error || right == error){
        return error;
    }

    if(left.isNumeric() && right.isNumeric()){
        if(left.specifier() == DOUBLE || right.specifier() == DOUBLE){
            return real;
        }
        else{
            return integer;
        }
    }
    report(E4, "*");
    return error;
}

Type checkDivide(const Type& left, const Type& right){
    if(left == error || right == error){
        return error;
    }

    if(left.isNumeric() && right.isNumeric()){
        if(left.specifier() == DOUBLE || right.specifier() == DOUBLE){
            return real;
        }
        else{
            return integer;
        }
    }
    report(E4, "/");
    return error;
}

Type checkModulo(const Type& left, const Type& right){
    if(left == error || right == error){
        return error;
    }
    Type lt = left.promote();
    Type rt = right.promote();

    if(lt.isInteger() && rt.isInteger()){
        return integer;
    }
    report(E4, "%");
    return error;
}


//additiveExpression

Type checkAdd(const Type& left, const Type& right){
    if(left == error || right == error){
        return error;
    }

    Type lt = left.promote();
    Type rt = right.promote();

    if(lt.isNumeric() && rt.isNumeric()){
        
        if(lt.specifier() == DOUBLE || rt.specifier() == DOUBLE){
            return real;
        }
        else{
            return integer;
        }
       
    }
    else if(lt.isPointer() && rt.isInteger()){ 
        return lt;
    }
    else if(lt.isInteger() && rt.isPointer()){
        return rt;
    }

    report(E4, "+");
    return error;

}

Type checkSub(const Type& left, const Type& right){
    if(left == error || right == error){
        return error;
    }

    Type lt = left.promote();
    Type rt = right.promote();

    if(lt.isNumeric() and rt.isNumeric()){

        if(lt.specifier() == DOUBLE || rt.specifier() == DOUBLE){
            return real;
        }
        else{
            return integer;
        }
        
        
    }
    else if(lt.isPointer() && right.isInteger()){ 
        return lt;
    }
    else if(lt.isPointer() && rt.isPointer() && lt.specifier() == rt.specifier()){ //do I have to check that pointer types are the same? last argument
        return integer;
    }

    report(E4, "-");
    return error;

}

//relationalExpression

Type checkLessThan(const Type& left, const Type& right){
    if(left == error || right == error){
        return error;
    }

    if(left.isCompatibleWith(right)){
        return integer;
    }
    report(E4, "<");
    return error;

}

Type checkLessThanEqual(const Type& left, const Type& right){
    if(left == error || right == error){
        return error;
    }

    if(left.isCompatibleWith(right)){
        return integer;
    }
    report(E4, "<=");
    return error;

}

Type checkGreaterThan(const Type& left, const Type& right){
    if(left == error || right == error){
        return error;
    }

    if(left.isCompatibleWith(right)){
        return integer;
    }
    report(E4, ">");
    return error;

}

Type checkGreaterThanEqual(const Type& left, const Type& right){
    if(left == error || right == error){
        return error;
    }

    if(left.isCompatibleWith(right)){
        return integer;
    }
    report(E4, ">=");
    return error;

}

//equalityExpression

Type checkEquals(const Type& left, const Type& right){
    if(left == error || right == error){
        return error;
    }

    if(left.isCompatibleWith(right)){
        return integer;
    }
    report(E4, "==");
    return error;

}

Type checkNotEquals(const Type& left, const Type& right){
    if(left == error || right == error){
        return error;
    }

    if(left.isCompatibleWith(right)){
        return integer;
    }
    report(E4, "!=");
    return error;

}

//logicalAndExpression

Type checkLogicalAnd(const Type& left, const Type& right){
    if(left == error || right == error){
        return error;
    }

    if(left.isPredicate() && right.isPredicate()){
        return integer;
    }
    report(E4, "&&");
    return error;

}

//logicalOrExpression

Type checkLogicalOr(const Type& left, const Type& right){
    if(left == error || right == error){
        return error;
    }

    if(left.isPredicate() && right.isPredicate()){
        return integer;
    }
    report(E4, "||");
    return error;

}

//Statements

Type checkReturnType(const Type& left, Symbol& function){
    if(left == error){
        return error;
    }
    
    if(left == function.type().specifier()){
        return left;
    }
    report(E1);
    return error;
}

Type checkWhile(const Type& left){
    if(left == error){
        return error;
    }

    if(left.isPredicate()){
        return left;
    }
    report(E2);
    return error;
}

Type checkIf(const Type& left){
    if(left == error){
        return error;
    }

    if(left.isPredicate()){
        return left;
    }
    report(E2);
    return error;
}

Type checkAssignment(const Type& left, const Type& right, bool& left_lvalue){

    if(left == error || right == error){
        return error;
    }
    
    if(left_lvalue != true){
        report(E3);
        return error;
    }
    if(left.isCompatibleWith(right) == false){
        report(E4, "=");
        return error;
    }
    return left;
}





