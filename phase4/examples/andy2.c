int i, *pi, *pi2, ai[10], *api[10], **ppi;
char c, *pc, ac[10], *apc[10], **ppc;
double d, *pd, ad[10], *apd[10], **ppd;

int foo(char a);
int *bar(int j, char k, double l, int *m, char *n, double *o);

int foo(char a) {
	return 352;
}

int main(void)
{
    /* Logical Expressions - Anything can be compared except functions (int, double, ptr)*/
    i || i && c;
    foo && d || i; /* invalid operands to binary && */
    i || d || apc && ppi && ppd || d;
    d || foo && i || pc; /* invalid operands to binary && */
    d || i || pc;
    d || i || bar && pc; /* invalid operands to binary && */
    d || i && pc;
    d || i || bar || pc; /* invalid operands to binary || */
    d && pi && c && d || pd;
    foo || foo || foo; /* invalid operands to binary || */

    /* Equality Expressions - Comparison between compatible types */
    i != c;
    pi == d; /* invalid operands to binary == */
    pi == pi;
    foo == foo; /* invalid operands to binary == */
    c == d;
    bar != foo; /* invalid operands to binary != */
    d != c;
    pi == foo; /* invalid operands to binary == */
    i == d;
    c == bar; /* invalid operands to binary == */

    /* Relational Expressions - Comparison between compatible types */
    i < c;
    pi <= d; /* invalid operands to binary <= */
    pi > pi;
    foo >= foo; /* invalid operands to binary >= */
    c <= d;
    bar < foo; /* invalid operands to binary < */
    d >= c;
    pi > foo; /* invalid operands to binary > */
    i < d;
    c > bar; /* invalid operands to binary > */

    /* Additive Expressions */
    c + i;
    c + ac;
	i + ac;
	i + foo; /* invalid operands to binary + */
	i + 3.14159265;
	foo - i; /* invalid operands to binary - */
	i - 3.14159265;
	i + d;
	i - d;
	pi + d; /* invalid operands to binary + */
    pi + i;
	ac + d; /* invalid operands to binary + */
	ac + i;
	d + pi; /* invalid operands to binary + */
	i + pi;
	pi - pd; /* invalid operands to binary - */
	pi - pi;
	pi - ppi; /* invalid operands to binary - */
	pi - pi2;
    pd - c;

    /* Multiplicative Expressions */
	c * i;
	c * pi; /* invalid operands to binary * */
	i * d;
	i * ai; /* invalid operands to binary * */
	d * d;
	pd * i; /* invalid operands to binary * */
	c * c;
	foo * i; /* invalid operands to binary * */
	c / i;
	c / pi; /* invalid operands to binary / */
	i / d;
	i / ai; /* invalid operands to binary / */
	d / d;
	pd / i; /* invalid operands to binary / */
	c / c;
	foo / i; /* invalid operands to binary / */
	c % i;
	d % pi; /* invalid operands to binary % */
	i % c;
	ai % d; /* invalid operands to binary % */
	c % c;
	d % foo; /* invalid operands to binary % */
	i % i;
	c % d; /* invalid operands to binary % */

    /* Prefix Expressions (*, &, !, -, sizeof, cast) */
	*pi;
	*i; /* invalid operand to unary * */
	*pi;
	*ai;
	*api;
	**pi; /* invalid operand to unary * */
	*ppi;
	**ppi;
	*c; /* invalid operand to unary * */
	*pc;
	*ac;
	*apc;
	*ppc;
	**ppc;
	*d; /* invalid operand to unary * */
	*pd;
	*ad;
	*apd;
	*ppd;
	**ppd;

	&pi;
	&i;
	&pi;
	&ai; /* lvalue required in expression */
	&ai[5];
	&api; /* lvalue required in expression */
	&api[1];
	&ppi;
	&c;
	&pc;
	&ac; /* lvalue required in expression */
	&ac[5];
	&apc; /* lvalue required in expression */
	&apc[21];
	&ppc;
	&d;
	&pd;
	&ad; /* lvalue required in expression */
	&ad[12];
	&apd; /* lvalue required in expression */
	&apd[2];
	&ppd;

	&*pi;
	&**pi; /* invalid operand to unary * */
	&**ppi;
	&i;

	!i;
	!pi;
	!ai;
	!api;
	!ppi;
	!c;
	!pc;
	!ac;
	!apc;
	!ppc;
	!d;
	!pd;
	!ad;
	!apd;
	!ppd;
	!foo; /* invalid operand to unary ! */
	!bar; /* invalid operand to unary ! */
	!main; /* invalid operand to unary ! */

	-i;
	-pi; /* invalid operand to unary - */
	-ai; /* invalid operand to unary - */
	-api; /* invalid operand to unary - */
	-ppi; /* invalid operand to unary - */
	-c;
	-pc; /* invalid operand to unary - */
	-ac; /* invalid operand to unary - */
	-apc; /* invalid operand to unary - */
	-ppc; /* invalid operand to unary - */
	-d;
	-pd; /* invalid operand to unary - */
	-ad; /* invalid operand to unary - */
	-apd; /* invalid operand to unary - */
	-ppd; /* invalid operand to unary - */
	-foo; /* invalid operand to unary - */
	-bar; /* invalid operand to unary - */
	-main; /* invalid operand to unary - */

	sizeof (int);
	sizeof (char);
	sizeof (double);

	/*
	char x int -> int
	int x int -> int
	double x int -> int
	ptr(alpha) x int -> int
	*/
	(int) i;
	(int) pi;
	(int) ai;
	(int) api;
	(int) ppi;
	(int) c;
	(int) pc;
	(int) ac;
	(int) apc;
	(int) ppd;
	(int) d;
	(int) pd;
	(int) ad;
	(int) apd;
	(int) ppd;

	/*
	char x char -> char
	int x char -> char
	double x char -> char
	*/
	(char) i;
	(char) pi; /* invalid operand in cast expression */
	(char) ai; /* invalid operand in cast expression */
	(char) api; /* invalid operand in cast expression */
	(char) ppi; /* invalid operand in cast expression */
	(char) c;
	(char) pc; /* invalid operand in cast expression */
	(char) ac; /* invalid operand in cast expression */
	(char) apc; /* invalid operand in cast expression */
	(char) ppd; /* invalid operand in cast expression */
	(char) d;
	(char) pd; /* invalid operand in cast expression */
	(char) ad; /* invalid operand in cast expression */
	(char) apd; /* invalid operand in cast expression */
	(char) ppd; /* invalid operand in cast expression */

	/*
	char x double -> double
	int x double -> double
	double x double -> double
	*/
	(double) i;
	(double) pi; /* invalid operand in cast expression */
	(double) ai; /* invalid operand in cast expression */
	(double) api; /* invalid operand in cast expression */
	(double) ppi; /* invalid operand in cast expression */
	(double) c;
	(double) pc; /* invalid operand in cast expression */
	(double) ac; /* invalid operand in cast expression */
	(double) apc; /* invalid operand in cast expression */
	(double) ppd; /* invalid operand in cast expression */
	(double) d;
	(double) pd; /* invalid operand in cast expression */
	(double) ad; /* invalid operand in cast expression */
	(double) apd; /* invalid operand in cast expression */
	(double) ppd; /* invalid operand in cast expression */

	/*
	int x ptr(alpha) -> ptr(alpha)
	ptr(alpha) x ptr(beta) -> ptr(beta)
	*/
	(int *) i;
	(int *) pi;
	(int *) ai;
	(int *) api;
	(int *) ppi;
	(int *) c; /* invalid operand in cast expression */
	(int *) pc;
	(int *) ac;
	(int *) apc;
	(int *) ppd;
	(int *) d; /* invalid operand in cast expression */
	(int *) pd;
	(int *) ad;
	(int *) apd;
	(int *) ppd;

	(char *) i;
	(char *) pi;
	(char *) ai;
	(char *) api;
	(char *) ppi;
	(char *) c; /* invalid operand in cast expression */
	(char *) pc;
	(char *) ac;
	(char *) apc;
	(char *) ppd;
	(char *) d; /* invalid operand in cast expression */
	(char *) pd;
	(char *) ad;
	(char *) apd;
	(char *) ppd;

	(double *) i;
	(double *) pi;
	(double *) ai;
	(double *) api;
	(double *) ppi;
	(double *) c; /* invalid operand in cast expression */
	(double *) pc;
	(double *) ac;
	(double *) apc;
	(double *) ppd;
	(double *) d; /* invalid operand in cast expression */
	(double *) pd;
	(double *) ad;
	(double *) apd;
	(double *) ppd;

	(int **) i;
	(int **) pi;
	(int **) ai;
	(int **) api;
	(int **) ppi;
	(int **) c; /* invalid operand in cast expression */
	(int **) pc;
	(int **) ac;
	(int **) apc;
	(int **) ppd;
	(int **) d; /* invalid operand in cast expression */
	(int **) pd;
	(int **) ad;
	(int **) apd;
	(int **) ppd;

	(char **) i;
	(char **) pi;
	(char **) ai;
	(char **) api;
	(char **) ppi;
	(char **) c; /* invalid operand in cast expression */
	(char **) pc;
	(char **) ac;
	(char **) apc;
	(char **) ppd;
	(char **) d; /* invalid operand in cast expression */
	(char **) pd;
	(char **) ad;
	(char **) apd;
	(char **) ppd;

	(double **) i;
	(double **) pi;
	(double **) ai;
	(double **) api;
	(double **) ppi;
	(double **) c; /* invalid operand in cast expression */
	(double **) pc;
	(double **) ac;
	(double **) apc;
	(double **) ppd;
	(double **) d; /* invalid operand in cast expression */
	(double **) pd;
	(double **) ad;
	(double **) apd;
	(double **) ppd;

    /* Postfix Expressions */
	i[256]; /* invalid operands to binary [] */
	pi[256];
	ai[256];
	api[256];
	ppi[256];
	c[256]; /* invalid operands to binary [] */
	pc[256];
	ac[256];
	apc[256];
	ppd[256];
	d[256]; /* invalid operands to binary [] */
	pd[256];
	ad[256];
	apd[256];
	ppd[256];

	pi[pi]; /* invalid operands to binary [] */
	pi[c];
	ai[ai]; /* invalid operands to binary [] */
	api[i];
	ppi[d]; /* invalid operands to binary [] */
	pc[pd]; /* invalid operands to binary [] */

    /* Primary Expressions */
    i = 123;
	i = 123.456;
	d = 123;
	d = 123.456;
	c = 123; /* */
	"string"[123];
	"string"[123.456]; /* invalid operands to binary [] */

	/*
	int main(void);
	int foo(char a);
	int *bar(int j, char k, double l, int *m, char *n, double *o);
	*/

	i(); /* called object is not a function */
	pi(); /* called object is not a function */
	c(); /* called object is not a function */
	d(); /* called object is not a function */
	main();
	main(i); /* invalid arguments to called function */
	main(c); /* invalid arguments to called function */
	main(123); /* invalid arguments to called function */
	foo(456);
	foo(c);
	foo(i);
	foo(d);
	foo(); /* invalid arguments to called function */
	foo(pc); /* invalid arguments to called function */
	foo(c, c); /* invalid arguments to called function */
	foo(d, d); /* invalid arguments to called function */
	bar(i, c, d, pi, pc, pd);
	bar(); /* invalid arguments to called function */
	bar(i); /* invalid arguments to called function */
	bar(i, c, d); /* invalid arguments to called function */
	bar(i, c, d, i, c, d); /* invalid arguments to called function */

	/* Statements */
	return i;
	return c;
	return d;
	return pi; /* invalid return type */
	return ai; /* invalid return type */
}

int *main2(void) {
	return i; /* invalid return type */
	return pi;
	return pc; /* invalid return type */
	return pd; /* invalid return type */
	return ai;
	return api; /* invalid return type */

	while(i)
		return i; /* invalid return type */
	while(foo(i))
		return pi;
	while(foo) /* invalid type for test expression */
		return pi;
	if(c)
		return c; /* invalid return type */
	if (foo(i))
		return i; /* invalid return type */
	if (foo) /* invalid type for test expression */
		return pi;
	else
	  return d; /* invalid return type */

	i = 314;
  	pi = 314; /* invalid operands to binary = */
  	ai = 314; /* lvalue required in expression */
  	api = 314; /* lvalue required in expression */
  	ppi = 314; /* invalid operands to binary = */
  	c = 314;
  	pc = 314; /* invalid operands to binary = */
  	ac = 314; /* lvalue required in expression */
  	apc = 314; /* lvalue required in expression */
  	ppd = 314; /* invalid operands to binary = */
  	d = 314;
  	pd = 314; /* invalid operands to binary = */
  	ad = 314; /* lvalue required in expression */
  	apd = 314; /* lvalue required in expression */
  	ppd = 314; /* invalid operands to binary = */

	i = 3.14;
	pi = 3.14; /* invalid operands to binary = */
	ai = 3.14; /* lvalue required in expression */
	api = 3.14; /* lvalue required in expression */
	ppi = 3.14; /* invalid operands to binary = */
	c = 3.14;
	pc = 3.14; /* invalid operands to binary = */
	ac = 3.14; /* lvalue required in expression */
	apc = 3.14; /* lvalue required in expression */
	ppd = 3.14; /* invalid operands to binary = */
	d = 3.14;
	pd = 3.14; /* invalid operands to binary = */
	ad = 3.14; /* lvalue required in expression */
	apd = 3.14; /* lvalue required in expression */
	ppd = 3.14; /* invalid operands to binary = */

	pi = i + c - d * i / c; /* invalid operands to binary = */
	ai = i + c - d * i / c; /* lvalue required in expression */
	api = i + c - d * i / c; /* lvalue required in expression */
	ppi = i + c - d * i / c; /* invalid operands to binary = */
	c = i + c - d * i / c;
	pc = i + c - d * i / c; /* invalid operands to binary = */
	ac = i + c - d * i / c; /* lvalue required in expression */
	apc = i + c - d * i / c; /* lvalue required in expression */
	ppd = i + c - d * i / c; /* invalid operands to binary = */
	d = i + c - d * i / c;
	pd = i + c - d * i / c; /* invalid operands to binary = */
	ad = i + c - d * i / c; /* lvalue required in expression */
	apd = i + c - d * i / c; /* lvalue required in expression */
	ppd = i + c - d * i / c; /* invalid operands to binary = */

	*pi = i + c - d * i / c;
	ai[5] = i + c - d * i / c;
	api[2] = &i;
	*ppi = pi;
	i = -c;
	i = sizeof (int);
}


	/*
	i;
	pi;
	ai;
	api;
	ppi;
	c;
	pc;
	ac;
	apc;
	ppd;
	d;
	pd;
	ad;
	apd;
	ppd;
	*/
