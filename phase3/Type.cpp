#include "Type.h"
#include "tokens.h"
#include <cassert>

using namespace std;

//If a genie offers you three wishes and you can't ask for any more,
//you know what to ask for: a genie with short-term memory loss
//A genie with short-term memory loss

bool Type::operator==(const Type& rhs) const{
	if(_kind != rhs._kind )
		return false;

	if(_kind == ERROR)
		//since errors won't have kinds or indirection
		return true;

	//Now we know both kinds are the same and they're not error, so common specifier
	if(_specifier != rhs._specifier)
		return false;

	if(_indirection != rhs._indirection)
		return false;

	if(_kind == SCALAR)
		return true;
		//since we're out of things that need to be checked for scalar's

	if(_kind == ARRAY)
		return _length == rhs._length;

	//There is a difference between a pointer that doesn't point to anything, and a pointer that points to something that happens to be NULL/Empty
	if(!_parameters || !rhs._parameters)
		return true;
		//Or since if either are unspecified, we can't check the parameters

	return *_parameters == *rhs._parameters;
	//needs the * to compare the actual vectors
	//Without the *, you are comparing the pointers

}

bool Type::operator!=(const Type& rhs) const{
	return !operator==(rhs);
}

//Constructors :
//Errors
Type::Type()
: _kind(ERROR)
{}

//Scalars
Type::Type(int specifier, unsigned indirection)
: _specifier(specifier), _indirection(indirection), _kind(SCALAR)
{}

//Arrays
Type::Type(int specifier, unsigned indirection, unsigned length)
: _specifier(specifier), _indirection(indirection), _length(length), _kind(ARRAY)
{}

//Functions
Type::Type(int specifier, unsigned indirection, Parameters* parameters)
: _specifier(specifier), _indirection(indirection), _parameters(parameters), _kind(FUNCTION)
{}


//Accessors :

int Type::specifier() const{
	return _specifier;
}
unsigned Type::indirection() const{
	return _indirection;
}
	
unsigned Type::length() const{
	assert(_kind == ARRAY);
	return _length;
}

Parameters* Type::parameters() const{
	assert(_kind == FUNCTION);
	return _parameters;
}

//OStream :

ostream &operator <<(ostream &ostr, Type &type)
{
	switch(type.specifier()){
		case CHAR:
			ostr << "char";
			break;
		case INT:
			ostr << "int";
			break;
		case DOUBLE:
			ostr << "double";
			break;
		default:
			ostr << "error";
			break;
	}

    if (type.indirection() > 0)
		ostr << " " << string(type.indirection(), '*');

    if (type.isArray())
		ostr << "[" << type.length() << "]";
    else if (type.isFunction())
		ostr << "()";

    return ostr;
}

