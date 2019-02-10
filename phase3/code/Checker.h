#ifndef CHECKER_H
#define CHECKER_H

#include "Scope.h"

Scope* openScope();
Scope* closeScope();

Symbol* decFn(
	const std::string &name, 
	const Type &type);

Symbol* defFn(
	const std::string &name, 
	const Type &type);

Symbol* decVar(
	const std::string &name, 
	const Type &type);

Symbol* checkID(const std::string &name);

#endif