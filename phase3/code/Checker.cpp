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

static int checker_debug = 1;

static Scope* globalScope = nullptr;
static Scope* currentScope = nullptr;

static vector<string> definedFunctions;

static int scopeCount = 0;
extern int lineno;


Scope* openScope(){

	currentScope = new Scope(currentScope);
	//currentScope passed as parameter to create a LL of scopes

	if(checker_debug == 1){
		cout << "\tOPEN SC: " << ++scopeCount << ". Line: "<< lineno << endl;
	}

	if(globalScope == nullptr){
		globalScope = currentScope;
		assert(globalScope->enclosing() == NULL);
	}

	return currentScope;
}

void closeScope(){
	Scope* deleteScope = currentScope;

	currentScope = currentScope->enclosing();

	delete deleteScope;

	if(checker_debug == 1){
		cout << "\tCLOS SC: " << scopeCount-- << ". Line: "<< lineno << endl;
	}

	return;

}

bool functionPreviouslyDefined(const std::string &name){
	int l = definedFunctions.size();
	for(int i = 0; i < l; i++){
		if(name == definedFunctions[i]){
			return true;
		}
	}
	return false;
}

Symbol* decFn(const std::string &name, const Type &type){
	Symbol* sym = globalScope->find(name);

	if(sym == nullptr){ //previously undeclared
		sym = new Symbol(name, type);
		globalScope->insert(sym);
	}
	else if(type != sym->type()){ //previously declared, must be exactly the same type
		report(E3,sym->name());

		// "Any subsequent declaration or definition always replaces any previous declaration or definition, even it erroneous"
		// remove the old symbol and replace it with new one
		currentScope->remove(sym->name());
		currentScope->insert(new Symbol(name, type));
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
		
		if( sym->type().isFunction() && functionPreviouslyDefined(name) && sym->type().parameters() ){ //check for re-definition
			delete sym->type().parameters(); //mem leak 2
			report(E1, sym->name());
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

	//add name to functions that have been defined
	definedFunctions.push_back(name);

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

Symbol* checkFn(const std::string &name){
	Symbol* sym = globalScope->find(name);

	if(sym == nullptr){
		//case where implicit declaration needs to happen
		decFn(name, Type(INT, 0, nullptr));

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











