/*
 * File:	generator.h
 *
 * Description:	This file contains the function declarations for the code
 *		generator for Simple C.  Most of the function declarations
 *		are actually member functions provided as part of Tree.h.
 */

# ifndef GENERATOR_H
# define GENERATOR_H
# include "Scope.h"
# include "Tree.h"

void generateGlobals(Scope *scope);

void arithmetic(Expression* result, Expression* left, Expression* right, std::string opcode);
void int_divide(Expression* result, Expression* left, Expression* right, Register* reg);
void compare(Expression* result, Expression* left, Expression* right, std::string opcode);

static std::string suffix(Expression *expr);

Register* getreg();
Register* fp_getreg();

void release();
void assigntemp(Expression *expr);
void assign(Expression *expr, Register *reg);
void load(Expression *expr, Register *reg);


# endif /* GENERATOR_H */
