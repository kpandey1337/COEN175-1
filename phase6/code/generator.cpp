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
# include <string>
# include <iostream>
# include "generator.h"
# include "machine.h"
# include "Tree.h"
# include "label.h"


using namespace std;

#define FP(expr) ((expr)->type().isReal())
#define BYTE(expr) ((expr)->type().size() == 1)


/* This needs to be set to zero if temporaries are placed on the stack. */

# define SIMPLE_PROLOGUE 0


/* This should be set if we want to use the callee-saved registers. */

# define CALLEE_SAVED 0

int offset; //moved from Function::generate() to global

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


/* CALLER SAVED REGISTERS */

static Register *eax = new Register("%eax", "%al");
static Register *ecx = new Register("%ecx", "%cl");
static Register *edx = new Register("%edx", "%dl");

Registers registers = {eax, ecx, edx};

/* FP REGISTERS */

static Register *xmm0 = new Register("%xmm0");
static Register *xmm1 = new Register("%xmm1");
static Register *xmm2 = new Register("%xmm2");
static Register *xmm3 = new Register("%xmm3");
static Register *xmm4 = new Register("%xmm4");
static Register *xmm5 = new Register("%xmm5");
static Register *xmm6 = new Register("%xmm6");
static Register *xmm7 = new Register("%xmm7");

Registers fp_registers = {xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7};

/* DATA SECTION */

static vector<string> data_section;

Label global_label;

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

	/* PEDRO: Spill all registers */
	for(unsigned i = 0; i < registers.size(); i++){
		load(nullptr, registers[i]);
	}
	for(unsigned i = 0; i < fp_registers.size(); i++){
		load(nullptr, fp_registers[i]);
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

	//get return address
	assign(this, eax);
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
	Expression* child = _left->isDereference();

	if(child == nullptr){
		_left->generate();
		_right->generate();

		if(_right->_register == nullptr)
			load(_right, getreg()); //handle FP here?

		cout << "\tmovl\t" << _right << ",\t" << _left << endl;

		assign(_right, nullptr);
	}
	else{ //is a deRef
		_right->generate();
		child->generate();

		if(_right->_register == nullptr)
			load(_right, getreg()); //handle FP here?
		if(child->_register == nullptr)
			load(child, getreg());

		cout << "\tmovl\t" << _right << ",\t(" << child << ")" << endl;

		assign(child, nullptr);
	}



}


/*
 * Function:	Block::generate
 *
 * Description:	Generate code for this block, which simply means we
 *		generate code for each statement within the block.
 */

void Block::generate()
{
	for (unsigned i = 0; i < _stmts.size(); i ++){
		_stmts[i]->generate();
		release(); 
	}
	
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
	int param_offset;


	/* Generate our prologue. */

	param_offset = PARAM_OFFSET + SIZEOF_REG * callee_saved.size();
	offset = param_offset;
	allocate(offset);

	cout << global_prefix << _id->name() << ":" << endl;
	cout << "#Begin: Prologue" << endl; 
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
	cout << "#Begin: Function Body" << endl; 

	Label exit;
	global_label = exit;

	_body->generate();

	cout << exit << endl;

	/* Generate our epilogue. */
	cout << "#Begin: Epilogue" << endl; 
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

	for(unsigned i = 0; i < data_section.size(); i++){
		cout << data_section[i];
	}
}

/* PEDRO GENERATE */

void arithmetic(Expression* result, Expression* left, Expression* right, string opcode){
	left->generate();
	right->generate();

	cout << "#Begin: " << opcode << endl; 

	if(left->_register == nullptr){
		load(left, FP(left) ? fp_getreg() : getreg());
	}

	cout << "\t" << opcode << suffix(left) << right << ", " << left << endl;

	assign(right, nullptr);
	assign(result, left->_register);
}

void Add::generate(){
	arithmetic(this, _left, _right, "add");
}

void Subtract::generate(){
	arithmetic(this, _left, _right, "sub");
}

void Multiply::generate(){
	arithmetic(this, _left, _right, FP(_left) ? "mul" : "imul");
}

void int_divide(Expression* result, Expression* left, Expression* right, Register* reg){
	//do I need to generate code for left/right here?

	cout << "#Begin: int_divide, register: " << reg << endl; 

	left->generate();
	right->generate();

	load(left, eax); //put left into eax and spill the contents of eax if necessary
	load(right, ecx); //put right into ecx, spill
	load(nullptr, edx); //spill the contents of edx if necessary

	cout << "\tmovl\t%eax,\t%edx \t#sign extend %eax into %edx" << endl;
	cout << "\tsarl\t$31,\t%edx" << endl;

	cout << "\tidivl\t" << right << "\t\t#%edx:%eax / "<< right << endl; 

	assign(nullptr, eax);
	assign(nullptr, ecx);
	assign(nullptr, edx);
	assign(result, reg);
}

void Divide::generate(){
	if (FP(_left)) arithmetic(this, _left, _right, "div");
	else int_divide(this, _left, _right, eax);
}

void Remainder::generate(){
	int_divide(this, _left, _right, edx);
}

void LogicalOr::generate(){
	Label l1, l2;

	_left->generate();
	_right->generate();

	if(_left->_register == nullptr){
		load(_left, getreg()); //handle FP here?
	}

	cout << "\tcmpl\t$0,\t" << _left << endl;
	cout << "\tjne\t" << l1 << endl;

	assign(_left, nullptr);

	if(_right->_register == nullptr){
		load(_right, getreg());
	}

	cout << "\tcmpl\t$0,\t" << _right << endl;
	cout << "\tjne\t" << l2 << endl;

	cout << l1 << ":" << endl;
	cout << "\tmovl\t$1,\t" << _right << endl;
	cout << l2 << ":" << endl;

	assign(this, _right->_register);
}

void LogicalAnd::generate(){
	Label l1, l2;

	_left->generate();
	_right->generate();

	if(_left->_register == nullptr){
		load(_left, getreg()); //handle FP here?
	}

	cout << "\tcmpl\t$0,\t" << _left << endl;
	cout << "\tje\t" << l1 << endl;

	assign(_left, nullptr);

	if(_right->_register == nullptr){
		load(_right, getreg());
	}

	cout << "\tcmpl\t$0,\t" << _right << endl;
	cout << "\tje\t" << l1 << endl;

	cout << "\tmovl\t$1,\t" << _right << endl;
	cout << l1 << ":" << endl;

	assign(this, _right->_register);

}

void compare(Expression* result, Expression* left, Expression* right, string opcode){
	left->generate();
	right->generate();

	cout << "#Begin: " << opcode << endl; 

	if(left->_register == nullptr){
		load(left, getreg()); //handle FP here?
	}

	cout << "\tcmp" << suffix(left) << right << ",\t" << left << endl;
	cout << "\t" << opcode << "\t" << left->_register->byte() << endl;
	cout << "\tmovzbl\t" << left->_register->byte() << ",\t" << left->_register << endl;

	assign(result, left->_register);
}

void LessThan::generate(){
	compare(this, _left, _right, "setl");
}

void GreaterThan::generate(){
	compare(this, _left, _right, "setg");
}

void LessOrEqual::generate(){
	compare(this, _left, _right, "setle");
}

void GreaterOrEqual::generate(){
	compare(this, _left, _right, "setge");
}

void Equal::generate(){
	compare(this, _left, _right, "sete");
}

void NotEqual::generate(){
	compare(this, _left, _right, "setne");
}

void Not::generate(){
	_expr->generate();

	if(_expr->_register == nullptr){
		load(_expr, getreg());
	}

	cout << "\tcmp" << suffix(_expr) << "$0,\t" << _expr << endl;
	cout << "\tsete\t" << _expr->_register->byte() << endl;
	cout << "\tmovzbl\t" << _expr->_register->byte() << ",\t" << _expr << endl;

	assign(this, _expr->_register);
}

void Negate::generate(){
	_expr->generate();

	if(_expr->_register == nullptr){
		load(_expr, getreg());
	}

	cout << "\tneg" << suffix(_expr) << _expr << endl;

	assign(this, _expr->_register);
}

void Address::generate(){
	if(_expr->isDereference() == nullptr){
		_expr->generate();
		assign(this, getreg());
		cout << "\tlea" << suffix(_expr) << _expr << ",\t" << this << endl;
	}
	else{
		_expr->generate();
		if(_expr->_register == nullptr)
			load(_expr, getreg());
		assign(this, _expr->_register);
	}
}

void Dereference::generate(){
	if(_expr->lvalue() == false){
		if(_expr->_register == nullptr){
			load(_expr, getreg()); //handle FP here?
		}

		cout << "\tmov" << suffix(_expr) << "(" << _expr << "),\t" << _expr << endl;
	}
}

void String::generate(){
	Label l;
	stringstream ss, tmp;

	//create a std::string for operand
	tmp << l;
	_operand = tmp.str();

	//load the string into a register
	//load(this, getreg());

	//create a label for it and put it in the data_section vector for the future
	ss << l << ":\t.asciz\t" << _value << endl;
	data_section.push_back(ss.str());
}



void If::generate(){
	Label skip, exit;

	_expr->generate();

	cout << "\tcmp\t$0,\t" << _expr <<endl;
	cout << "\tje\t" << skip << endl;

	_thenStmt->generate();
	release();

	if(_elseStmt != nullptr)
		cout << "\tjmp\t" << exit << endl;

	cout << skip << ":" << endl;

	if(_elseStmt != nullptr){
		_elseStmt->generate();
		release();
		cout << exit << ":" << endl;
	}
}

void While::generate(){
	Label loop, exit;
	
	cout << loop << ":" << endl;
	_expr->generate();
	cout << "\tcmp\t$0,\t" << _expr <<endl;
	cout << "\tje\t" << exit << endl;
	_stmt->generate();
	release();
	cout << "\tjmp\t" << loop << endl;
	cout << exit << ":" << endl;

}

void Return::generate(){
	_expr->generate();

	load(_expr, eax);

	cout << "\tjmp\t" << global_label << endl;
}


















void Expression::test(const Label &label, bool ifTrue){
	//generates code for expression.
	//compares result against zero.
	//branches to the given label depending on the status of the ifTrue parameters

	generate();

	if(_register == nullptr)
		load(this, getreg()); //Doesn't this need to handle FP??

	cout << "\tcmpl\t$0, " << this << endl;
	cout << (ifTrue ? "\tjne\t" : "\tje\t") << label << endl;

	assign(this, nullptr);
}





/* PEDRO HELPERS */

static string suffix(Expression *expr){
	return FP(expr) ? "sd\t" : (BYTE(expr) ? "b\t" : "l\t");
}

Register* getreg(){
	for (unsigned i=0; i < registers.size(); i++){
		if(registers[i]->_node == nullptr){
			return registers[i];
		}
	}

	load(nullptr, registers[0]);
	return registers[0];
}

Register* fp_getreg(){
	for(unsigned i=0; i < fp_registers.size(); i++){
		if(fp_registers[i]->_node == nullptr)
			return fp_registers[i];
	}

	load(nullptr, fp_registers[0]);
	return fp_registers[0];
}

void release(){
	for(unsigned i = 0; i < registers.size(); i++){
		assign(nullptr, registers[i]);
	}
}

void assigntemp(Expression *expr){
	stringstream ss;

	offset = offset - expr->type().size();
	ss << offset << "(%ebp)";

	expr->_operand = ss.str();
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

void load(Expression *expr, Register *reg){
	if(reg->_node != expr){

		if(reg->_node != nullptr){
			unsigned size = reg->_node->type().size();

			assigntemp(reg->_node);
			cout << "\tmov" << suffix(reg->_node);
			cout << reg->name(size) << ",\t";
			cout << reg->_node->_operand << "\t#Spill!" << endl;
		}

		if(expr != nullptr){
			unsigned size = expr->type().size();
			cout << "\tmov" << suffix(expr) << expr;
			cout << ",\t" << reg->name(size) << endl;
		}

		assign(expr, reg);

	}
}
