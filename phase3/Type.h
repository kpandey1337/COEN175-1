

#ifndef TYPE_H
#define TYPE_H

#include <vector>
#include <ostream>

typedef std::vector<class Type> Parameters;

class Type{
	//by default all class modifiers are Private

	int _specifier;
	
	unsigned _indirection;
	unsigned _length;
	
	//seems silly to create a vector when not all types require them. 
	//Instead store a pointer that can be allocated when dealing with a function.
	Parameters* _parameters; 

	enum {
		ARRAY,
		ERROR,
		FUNCTION,
		SCALAR
	} kind;

public:

	Type();

	//Scalars
	Type(int specifier, unsigned indirection=0);

	//Arrays
	Type(int specifier, unsigned indirection, unsigned length);

	//Functions
	Type(int specifier, unsigned indirection, Parameters* parameters);

	//Overload operator==
	bool operator==(const Type& rhs) const;
	//LHS is this, no need to pass LHS as parameter
	//Pass by reference to not make a copy
	//Last const prevents changing *this
	bool operator!=(const Type& rhs) const;


	bool isScalar() const;
	bool isArray() const;
	bool isError() const; //we are in the class so no need to specify this->
	bool isFunction() const;

	int specifier() const;
	unsigned indirection() const;
	unsigned length() const;
	Parameters* parameters() const;

};

std::ostream& operator<<(std::ostream& ostr, const Type& type);

#endif /* TYPE_H */

