#include "Type.h"
#include <cassert>

using namespace std;

bool Type::operator!=(const Type& rhs) const{
	return !operator==(rhs);
}

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

//Constructor
Type::Type(int specifier, unsigned indirection, Parameters* parameters)
: _specifier(specifier), _indirection(indirection), _parameters(parameters), _kind(FUNCTION){

}

unsigned Type::length() const{
	assert(_kind == ARRAY);
	return _length;
}


