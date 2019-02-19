#ifndef CHECKER_H
#define CHECKER_H

#include "Scope.h"

Scope* openScope();
Scope* closeScope();

<<<<<<< HEAD

=======
>>>>>>> c2f69d1e17f6f36a4d1388f8cf904979b9376c83
bool functionPreviouslyDefined(const std::string &name);

void decFn(
	const std::string &name, 
	const Type &type);

void defFn(
	const std::string &name, 
	const Type &type);

void decVar(
	const std::string &name, 
	const Type &type);

void checkFn(const std::string &name);

void checkID(const std::string &name);

#endif