/*
 * File:	generator.cpp
 *
 * Description:	This file contains the public and member function
 *		definitions for the code generator for Simple C.
 *
 *		Extra functionality:
 *		- putting all the global declarations at the end
 */

# include <sstream>
# include <iostream>
# include "generator.h"
# include "machine.h"
# include "Tree.h"

using namespace std;


/* This needs to be set to zero if temporaries are placed on the stack. */

# define SIMPLE_PROLOGUE 0


/* This should be set if we want to use the callee-saved registers. */

# define CALLEE_SAVED 0


/* The registers and their related functions */

typedef vector<Register *>Registers;

static Register *ebx = new Register("%ebx", "%bl");
static Register *esi = new Register("%esi");
static Register *edi = new Register("%edi");

# if CALLEE_SAVED
static Registers callee_saved = {ebx, esi, edi};
# else
static Registers callee_saved = {};
# endif


/* CALLER DEFINED REGISTERS */

static Register *eax = new Register("%eax", "%al");

Registers registers = {eax};


/*
 * Function:	align (private)
 *
 * Description:	Return the number of bytes necessary to align the given
 *		offset on the stack.
 */

static int align(int offset)
{
	if (offset % STACK_ALIGNMENT == 0)
	return 0;

	return STACK_ALIGNMENT - (abs(offset) % STACK_ALIGNMENT);
}


/*
 * Function:	operator << (private)
 *
 * Description:	Write an expression to the specified stream.  This function
 *		first checks to see if the expression is in a register, and
 *		if not then uses its operand.
 */

static ostream &operator <<(ostream &ostr, Expression *expr)
{
	if (expr->_register != nullptr)
		return ostr << expr->_register;

	return ostr << expr->_operand;
}


/*
 * Function:	Identifier::generate
 *
 * Description:	Generate code for an identifier.  Since there is really no
 *		code to generate, we simply update our operand.
 */

void Identifier::generate()
{
	stringstream ss;


	if (_symbol->_offset != 0)
	ss << _symbol->_offset << "(%ebp)";
	else
	ss << global_prefix << _symbol->name();

	_operand = ss.str();
}


/*
 * Function:	Integer::generate
 *
 * Description:	Generate code for an integer.  Since there is really no
 *		code to generate, we simply update our operand.
 */

void Integer::generate()
{
	stringstream ss;


	ss << "$" << _value;
	_operand = ss.str();
}


/*
 * Function:	Call::generate
 *
 * Description:	Generate code for a function call expression.
 *
 *		NOT FINISHED: Only guaranteed to work if the argument is
 *		either an integer literal or an integer scalar variable.
 */

void Call::generate()
{
	unsigned bytesPushed = 0;


	/* Compute how many bytes will be pushed on the stack. */

	for (int i = _args.size() - 1; i >= 0; i --) {
		bytesPushed += _args[i]->type().size();

		if (STACK_ALIGNMENT != 4 && _args[i]->_hasCall)
			_args[i]->generate();
	}


	/* Adjust the stack keep it aligned.  We do this by simply subtracting
	   the necessary number of bytes from the stack pointer.  Effectively,
	   we are pushing nonexistent arguments on the stack so that the total
	   number of bytes pushed is a multiple of the alignment. */

	if (align(bytesPushed) > 0) {
		cout << "\tsubl\t$" << align(bytesPushed) << ", %esp" << endl;
		bytesPushed += align(bytesPushed);
	}


	/* Push each argument on the stack. */

	for (int i = _args.size() - 1; i >= 0; i --) {
		if (STACK_ALIGNMENT == 4 || !_args[i]->_hasCall)
			_args[i]->generate();

		cout << "\tpushl\t" << _args[i] << endl;
	}


	/* Call the function and then adjust the stack pointer back. */

	cout << "\tcall\t" << global_prefix << _id->name() << endl;

	if (bytesPushed > 0)
	cout << "\taddl\t$" << bytesPushed << ", %esp" << endl;
}


/*
 * Function:	Assignment::generate
 *
 * Description:	Generate code for an assignment statement.
 *
 *		NOT FINISHED: Only guaranteed to work if the right-hand
 *		side is an integer literal and the left-hand side is an
 *		integer scalar.
 */

void Assignment::generate()
{
	_left->generate();
	_right->generate();
	cout << "\tmovl\t" << _right << ", " << _left << endl;
}


/*
 * Function:	Block::generate
 *
 * Description:	Generate code for this block, which simply means we
 *		generate code for each statement within the block.
 */

void Block::generate()
{
	for (unsigned i = 0; i < _stmts.size(); i ++)
	_stmts[i]->generate();
}


/*
 * Function:	Function::generate
 *
 * Description:	Generate code for this function, which entails allocating
 *		space for local variables, then emitting our prologue, the
 *		body of the function, and the epilogue.
 *
 *		The stack must be aligned at the point at which a function
 *		begins execution.  Since the call instruction pushes the
 *		return address on the stack and each function is expected
 *		to push its base pointer, we adjust our offset by that
 *		amount and then perform the alignment.
 *
 *		On a 32-bit Intel platform, 8 bytes are pushed (4 for the
 *		return address and 4 for the base pointer).  Since Linux
 *		requires a 4-byte alignment, all we need to do is ensure
 *		the stack size is a multiple of 4, which will usually
 *		already be the case.  However, since OS X requires a
 *		16-byte alignment (thanks, Apple, for inventing your own
 *		standards), we will often see an extra amount of stack
 *		space allocated.
 *
 *		On a 64-bit Intel platform, 16 bytes are pushed (8 for the
 *		return address and 8 for the base pointer).  Both Linux and
 *		OS X require 16-byte alignment.
 */

void Function::generate()
{
	int param_offset, offset;


	/* Generate our prologue. */

	param_offset = PARAM_OFFSET + SIZEOF_REG * callee_saved.size();
	offset = param_offset;
	allocate(offset);

	cout << global_prefix << _id->name() << ":" << endl;
	cout << "\tpushl\t%ebp" << endl;

	for (unsigned i = 0; i < callee_saved.size(); i ++)
	cout << "\tpushl\t" << callee_saved[i] << endl;

	cout << "\tmovl\t%esp, %ebp" << endl;

	if (SIMPLE_PROLOGUE) {
	offset -= align(offset - param_offset);
	cout << "\tsubl\t$" << -offset << ", %esp" << endl;
	} else
	cout << "\tsubl\t$" << _id->name() << ".size, %esp" << endl;


	/* Generate the body of this function. */

	_body->generate();


	/* Generate our epilogue. */

	cout << "\tmovl\t%ebp, %esp" << endl;

	for (int i = callee_saved.size() - 1; i >= 0; i --)
	cout << "\tpopl\t" << callee_saved[i] << endl;

	cout << "\tpopl\t%ebp" << endl;
	cout << "\tret" << endl << endl;

	if (!SIMPLE_PROLOGUE) {
	offset -= align(offset - param_offset);
	cout << "\t.set\t" << _id->name() << ".size, " << -offset << endl;
	}

	cout << "\t.globl\t" << global_prefix << _id->name() << endl << endl;
}


/*
 * Function:	generateGlobals
 *
 * Description:	Generate code for any global variable declarations.
 */

void generateGlobals(Scope *scope)
{
	const Symbols &symbols = scope->symbols();

	for (unsigned i = 0; i < symbols.size(); i ++)
		if (!symbols[i]->type().isFunction()) {
			cout << "\t.comm\t" << global_prefix << symbols[i]->name() << ", ";
			cout << symbols[i]->type().size() << endl;
		}
}



/* PEDRO */

Register* getreg(){
	for (unsigned i=0; i < registers.size(); i++){
		if(registers[i]->_node == nullptr){
			return registers[i];
		}
	}

	load(nullptr, registers[0]);
	return registers[0]
}

void assign(Expression *expr, Register *reg){
	if(expr != nullptr){
		if (expr->_register != nullptr)
			expr->_register->_node = nullptr;
		
		expr->_register = reg;
	}

	if (reg != nullptr) {
		if (reg->_node != nullptr)
			reg->_node->_register = nullptr;
		
		reg->_node = expr;
	}
}

void assigntemp(Expression *expr){
	//stringstream ss;

	offset = offset - expr->type().size();
	expr->_operand = offset + "(%ebp)";

	//he uses stringstream here instead

}

void load(Expression *expr, Register *reg){
	if(reg->_node != expr){

		if(reg->_node != nullptr){
			unsigned size = reg->_node->type().size();

			assigntemp(reg->_node);
			cout << "\tmov" << suffix(reg->_node);
			cout << reg->name(size) << ", ";
			cout << reg->_node->_operand << endl;
		}

		if(expr != nullptr){
			unsigned size = expr->type().size();
			cout << "\tmov" << suffix(expr) << expr;
			cout << ", " << reg->name(size) << endl;
		}

		assign(expr, reg);

	}
}
