#include <cassert>
#include "Scope.h"

Scope::Scope(Scope* enclosing)
	: _enclosing(enclosing)
{
}

void Scope::insert(Symbol* symbol){
	//ensure symbol uniqueness
	assert(find(symbol->name()) == nullptr );

	//C++ equivalent for py's list.append
	//http://www.cplusplus.com/reference/vector/vector/push_back/
	_symbols.push_back(symbol);
}

void Scope::remove(const string& name){
	//http://www.cplusplus.com/reference/vector/vector/erase/
	int l = _symbols.size();

	//find element, if found delete it
	for(int i = 0; i < l; ++i){
		if(name == _symbols[i]->name()){
			_symbols.erase(_symbols.begin() + i);
		}
	}

	//shouldn't I indicate if not found?
	//but was given return type void..
}

Symbol* Scope::find(const string& name) const{
	Symbol* result = nullptr;

	int l = _symbols.size();
	for(int i = 0; i < l; ++i){
		if(_symbols[i]->name() == name){
			result = _symbols[i];
			return result;
		}
	}
	return result;
}

Symbol* Scope::lookup(const string& name) const{

	Symbol* result;
	
	Scope* currentScope;
	currentScope = (Scope*) this;
		
	while (currentScope != nullptr){
		result = currentScope->find(name);
		
		if(result != nullptr){
			return result;
		}

		currentScope = currentScope->_enclosing;

	}
	return nullptr;

}

Scope* Scope::enclosing() const{
	return _enclosing;
}

const Symbols& Scope::symbols() const{
	return _symbols;
}


