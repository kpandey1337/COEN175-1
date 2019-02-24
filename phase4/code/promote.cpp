#define E4 "invalid operands to binary %s"

static Type error, integer(INT);

Type Type::promote() const{

	//characters and arrays are promoted

	if(_specifier == CHAR && _kind == SCALAR && _indirection == 0){
		return Type(INT);
	}

	if(_kind == ARRAY){
		return Type(_specifier, _indirection + 1);
	}

	return *this;
}

bool Type::isPredicate() const{
	//return(isNumeric() || isPointer());
	return(_kind == SCALAR || _kind == ARRAY);
}

bool Type::isCompatibleWith(const Type& that) const{
	if(isNumeric() && that.isNumeric){
		return true;
	}
	return(*this == *that && isPredicate());
}

Type checkLogicalOr(const Type& left, const Type& right){

	if(left.isPredicate() && right.isPredicate()){
		return integer;
	}
	report(E4, "||");
	return error;

}

Type checkEqualityExpression(const Type& left, const Type& right){

	if(left.isCompatibleWith(right)){
		return integer;
	}
	report(E4, "==");
	return error;

}









