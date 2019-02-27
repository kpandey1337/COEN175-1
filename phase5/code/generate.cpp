
#include "Scope.h"
#include "checker.h"
#include "Type.h"

extern Scope* toplevel;

using namespace std;

void generateGlobals(){
	Symbol* symbols = toplevel->symbols();

	for(unsigned i = 0; i < symbols.size(); i++){
		if( !(symbols[i]->type().isFunction() || symbols[i]->type().isError()) ){
			cout << ".comm" << symbols[i]->name() << ", " << symbols[i]->type().size() << endl;
		}
	}
}

void Function::generate() const{

// 1) Assign offsets

	//Get number of parameters from function's Type
	int numParameters = _id->type().parameters()->size();

	//Allocate the first N variable's offsets as positive
	//Allocate the rest of the variables to negative offsets
	Symbols* symbols = _body->declarations()->symbols();
	unsigned i;
	int arguments = 8;
	int locals = 0;

	for(i = 0; i < numParameters; i++){
		if(i < numParameters){
			symbols[i]->_offset = arguments;
			arguments += symbols[i]->type().size();
		}
		else{
			locals -= symbols[i]->type().size();
			symbols[i]->_offset = locals;
		}
	}

// 2) Prologue/Epilogue

	//Prologue
	cout << "pushl %ebp" << endl;
	cout << "movl %esp, %ebp" << endl;
	cout << "addl " << locals << ", %esp" << endl;

	_body->generate(); //Block::generate()

	//Epilogue
	cout << "movl %ebp, %esp" << endl;
	cout << "popl %ebp" << endl;
	cout << "ret" << endl;
}

void Block::generate() const{



}

