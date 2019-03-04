
#include "Scope.h"
#include "checker.h"
#include "Type.h"
#include "generate.h"
#include <iostream>
#include <sstream>

using namespace std;

static ostream& operator <<(ostream& strm, Expression* expr){
	return strm << expr->_operand;
}

void generateGlobals(Scope* toplevel){
	Symbols symbols = toplevel->symbols();

	cout << "#globals" << endl;

	for(unsigned i = 0; i < symbols.size(); i++){
		if( !(symbols[i]->type().isFunction() || symbols[i]->type().isError()) ){
			cout << ".comm\t" << symbols[i]->name() << ",\t" << symbols[i]->type().size() << endl;
		}
		else if (symbols[i]->type().isFunction()){
			cout << ".globl\t" << symbols[i]->name() << endl;
		}
	}

}

void Function::generate(){

// 1) Assign offsets

	//Get number of parameters from function's Type
	unsigned numParameters = _id->type().parameters()->size();

	//Allocate the first N variable's offsets as positive
	//Allocate the rest of the variables to negative offsets
	Symbols symbols = _body->declarations()->symbols();
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
	//Label function_label;
	cout << "." << _id->name() << ": " << endl;
	cout << "#prologue" << endl;
	

	cout << "\tpushl %ebp" << endl;
	cout << "\tmovl %esp, %ebp" << endl;
	cout << "\tsubl " << -locals << ", %esp" << endl;


	cout << "#begin function body" << endl;
	_body->generate(); //Block::generate()
	cout << "#end function body" << endl;
	
	//Epilogue
	cout << "#epilogue" << endl;
	cout << "\tmovl\t%ebp,\t%esp" << endl;
	cout << "\tpopl\t%ebp" << endl;
	cout << "\tret" << endl;
}

void Block::generate(){
	for(unsigned i = 0; i < _stmts.size(); i++){
		_stmts[i]->generate();
	}
}

void Assignment::generate(){
	_left->generate();
	_right->generate();

	cout << "\tmovl\t" << _right << ",\t" << _left << endl;

}

void Call::generate(){

	int i;
	for(i = _args.size()-1; i >= 0; i--){
		_args[i]->generate();
	}

	cout << "\tcall\t" << _id->name() << endl;
	//CALL 
	cout << "\taddl\t" << _args.size() * 4 << ",\t" << "%esp" << endl;

	//Push arguments onto stack
	//Push return address onto stack
	//CALL .
}

void Integer::generate(){
	stringstream ss;

	ss << "$" << _value;

	_operand = ss.str();
}

void Identifier::generate(){
	stringstream ss;

	if(_symbol->_offset == 0){ //Then global variable
		ss << _symbol->name();
	}
	else{
		ss << _symbol->_offset << "(%ebp)";
	}

	_operand = ss.str();
}


