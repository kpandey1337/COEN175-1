/*
 * File:	checker.h
 *
 * Description:	This file contains the public function declarations for the
 *		semantic checker for Simple C.
 */

# ifndef CHECKER_H
# define CHECKER_H
# include "Scope.h"

Scope *openScope();
Scope *closeScope();

Symbol *defineFunction(const std::string &name, const Type &type);
Symbol *declareFunction(const std::string &name, const Type &type);
Symbol *declareVariable(const std::string &name, const Type &type);
Symbol *checkIdentifier(const std::string &name);
Symbol *checkFunction(const std::string &name);

// Phase 4 functions

Type checkFunctionType(const Symbol& sym, Parameters* arguments);

Type checkIdentifierType(const Type& left, bool& lvalue);

Type checkIndex(const Type& left, const Type& right);

Type checkNot(const Type& left);
Type checkNegate(const Type& left);
Type checkDereference(const Type& left);
Type checkAddress(const Type& left, bool& lvalue);
Type checkSizeOf(const Type& left);
Type checkTypeCast(const Type& left, int typespec, unsigned indirection);

Type checkMultiply(const Type& left, const Type& right);
Type checkDivide(const Type& left, const Type& right);
Type checkModulo(const Type& left, const Type& right);

Type checkAdd(const Type& left, const Type& right);
Type checkSub(const Type& left, const Type& right);

Type checkLessThan(const Type& left, const Type& right);
Type checkGreaterThan(const Type& left, const Type& right);
Type checkLessThanEqual(const Type& left, const Type& right);
Type checkGreaterThanEqual(const Type& left, const Type& right);

Type checkEqualityExpression(const Type& left, const Type& right);
Type checkNotEquals(const Type& left, const Type& right);
Type checkEquals(const Type& left, const Type& right);
Type checkLogicalAnd(const Type& left, const Type& right);
Type checkLogicalOr(const Type& left, const Type& right);

# endif /* CHECKER_H */
