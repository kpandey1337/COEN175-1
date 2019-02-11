#ifndef CHECKER_H
#define CHECKER_H

#include "Scope.h"

Scope* openScope();
void closeScope();

bool functionPreviouslyDefined(const std::string &name);

Symbol* defFn(
	const std::string &name, 
	const Type &type);

Symbol* decFn(
	const std::string &name, 
	const Type &type);

Symbol* decVar(
	const std::string &name, 
	const Type &type);

Symbol* checkFn(const std::string &name);

Symbol* checkID(const std::string &name);

#endif