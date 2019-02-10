#ifndef CHECKER_H
#define CHECKER_H

#include "Scope.h"

Scope* openScope();
void closeScope();

Symbol* decFunction(const std::string &name, const Type &type);
Symbol* defFunction(const std::string &name, const Type &type);
Symbol* decVariable(const std::string &name, const Type &type);
Symbol* existsIdentifier(const std::string &name);

#endif