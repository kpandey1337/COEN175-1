#ifndef CHECKER_H
#define CHECKER_H

#include "Scope.h"

<<<<<<< HEAD
void openScope();
void closeScope();
=======
Scope* openScope();
Scope* closeScope();
>>>>>>> master

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