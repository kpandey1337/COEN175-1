//If you can't write Symbol..

#ifndef SCOPE_H
#define SCOPE_H

#include "Symbol.h"
#include <vector>
//Could use a map here instead of a vector

//bad idea to open the standard namespace here

typedef std::vector<Symbol*> Symbols;

class Scope{
	typedef std::string string;
	//Now the class can use "string" instead of including the scoping, without polluting the namespaces outside

	Scope* _enclosing;
	Symbols _symbols;

public:
	Scope(Scope* enclosing=nullptr);

	void insert(Symbol* symbol);
	void remove(const string& name); //recall not to pass objects by value-- in this case, str is an object so pass its reference + const
	Symbol* find(const string& name) const;
	Symbol* lookup(const string& name) const;

	Scope* enclosing() const;
	const Symbols& symbols() const; //you're returning a reference to a private member, so you should make it const in order to ensure that all modifications go through your interface
	
};

#endif