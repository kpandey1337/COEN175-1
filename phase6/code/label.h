#pragma once

#include <iostream>

class Label {

	static unsigned _count;
	unsigned _number;

public:
	Label();
	unsigned number() const;

};

std::ostream &operator <<(std::ostream &ostr, const Label &label);