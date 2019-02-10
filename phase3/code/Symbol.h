#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include "Type.h"

class Symbol {

	std::string _name;
	Type _type;

public:

	Symbol(const std::string &name, const Type &type);
	const std::string &name(){return _name; }
	const Type &type(){return _type; }

};



#endif