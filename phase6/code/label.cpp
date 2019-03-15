#include "label.h"

unsigned Label::_count = 0;

Label::Label(){
	_number = _count++;
}

unsigned Label::number() const{
	return _number;
}

std::ostream &operator <<(std::ostream &ostr, const Label &label) {
	return ostr << ".L" << label.number();
}