#include <cassert>
#include <iostream>

#include "Checker.h"

#include "lexer.h"
#include "tokens.h"

#include "Type.h"
#include "Symbol.h"
#include "Scope.h"

#define E1 "redefinition of ’%s’"
#define E2 "redeclaration of ’%s’"
#define E3 "conflicting types for ’%s’"
#define E4 "’%s’ undeclared"

using namespace std;

static Scope* globalScope = nullptr;
static Scope* currentScope = nullptr;

static int scopeCounter = 0;
static int checker_debug = 1;

extern int lineno;

static vector<string> definedFunctions;

Scope* openScope(){

	currentScope = new Scope(currentScope);
	//currentScope passed as parameter to create a LL of scopes

	if(globalScope == nullptr){
		globalScope = currentScope;
		assert(globalScope->enclosing() == NULL);
	}

	if(checker_debug == 1){
    	cout << "Scope Counter: "<< ++scopeCounter <<". On line: " << lineno << endl;
    }

	return currentScope;
}

Scope* closeScope(){
	Scope* closeScope = currentScope;

	currentScope = currentScope->enclosing();

	if(checker_debug == 1){
    	cout << "Scope Counter: "<< --scopeCounter <<". On line: " << lineno << endl;
    }

	return closeScope;

}

Symbol* decFn(const std::string &name, const Type &type){
	Symbol* sym = globalScope->find(name);

	if(sym == nullptr){ //previously undeclared
		sym = new Symbol(name, type);
		globalScope->insert(sym);
	}
	else if(type != sym->type()){ //previously declared, must be exactly the same type
		delete type.parameters(); //mem leak 1
		report(E3,sym->name());
	}

	return sym;
}

//Note the differences between: . and ->
// (*foo).bar() == foo->bar()
//https://stackoverflow.com/questions/1238613/what-is-the-difference-between-the-dot-operator-and-in-c

Symbol* defFn(const std::string &name, const Type &type){
	Symbol* sym = globalScope->find(name);

	if(sym != nullptr){
		//so previously declared or defined
		
		if( sym->type().isFunction() && sym->type().parameters() ){ //check for re-definition
			report(E1, sym->name());
			delete sym->type().parameters(); //mem leak 2
			
		}
		else if(sym->type() != type){ //should be a function, but more than that should be Type::==
			report(E3, sym->name());
		}

		globalScope->remove(name);
		delete sym;

	}
	//no else since we have to replace past def/dec
	sym = new Symbol(name, type);
	globalScope->insert(sym);

	



	return sym;
}

Symbol* decVar(const std::string &name, const Type &type){
	Symbol* sym = currentScope->find(name);

	if(sym != nullptr){
		//already exists
		if(globalScope != currentScope){ //redeclarations only valid in global scope
			report(E2, name);
		}
		else if(sym->type() != type){
			report(E3, name);
		}
	}
	else{
		sym = new Symbol(name, type);
		currentScope->insert(sym);
	}

	return sym;
}

Symbol* checkID(const std::string &name){
	Symbol* sym = currentScope->lookup(name);

	if(sym == nullptr){
		Type error;
		sym = new Symbol(name, error);
		currentScope->insert(sym);

		report(E4, name);
	}

	return sym;
}











