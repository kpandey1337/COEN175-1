/* need to test pre, post, primary, and statement */

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
  int *k;
  k = bar(); /* invalid arguments to called function */

  /* pre functions */

  /* testing * */
  *i; /* invalid operand to unary * */
  *pi;
  *pi2;
  *ai;
  *api;
  **api;
  **pi; /* invalid operand to unary * */
  *c; /* invalid operand to unary * */
  *pc;
  *ac;
  *apc;
  **apc;
  ***apc; /* invalid operand to unary * */
  *ppc;
  **ppc;
  **i; /* invalid operand to unary * */
  *d; /* invalid operand to unary * */
  *pd;
  *ad;
  *apd;
  *ppd;
  **ppd;
  ***ppd; /* invalid operand to unary * */
  *foo; /* invalid operand to unary * */
  *bar; /* invalid operand to unary * */

  /* testing & */
  &i;
  &pi;
  &pi2;
  &ai;/* lvalue required in expression */
  &api; /* lvalue required in expression */
  &c;
  &pc;
  &ac; /* lvalue required in expression */
  &apc; /* lvalue required in expression */
  &ppc;
  &d;
  &ad; /* lvalue required in expression */
  &apd; /* lvalue required in expression */
  &ppd;
  &foo; /* lvalue required in expression */
  &bar; /* lvalue required in expression */
  &1; /* lvalue required in expression */
  &ai[4];
  &api[9];
  &apd[6];

  /* testing ! */
  /*must be a predicate type: after promotion,
    numeric or pointer */
  !i;
  !pi;
  !pi2;
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

  /* testing - */
  /* must be numeric after promotion */
  /* would have done the same as andy */

  /* postfix expressions */
  /* left op must be pointer after promotion
    expression must have type int after promotion */
  pi[c];
  pi[ad]; /* invalod operands to binary [] */
  pi[ppd]; /* invalid operand to unary & */

  /* primary expressions */


  /* statements */

  while(!pi) {}
  while(pi) {}
  while(i) {}
  while(c) {}
  while(api) {}
  while(ad) {}
  while (apd) {}
  while (ppd) {}
  while (foo) /* invalid type for test expression */
  while (bar) /* invalid type for test expression */
  while (main) /* invalid type for test expression */

  if(!pi) {}
  if(pi) {}
  if(i) {}
  if(c) {}
  if(api) {}
  if(ad) {}
  if (apd) {}
  if (ppd) {}
  if (foo) /* invalid type for test expression */
  if (bar) /* invalid type for test expression */
  if (main) /* invalid type for test expression */

  i = pi; /* invalid operands to binary operator = */
  i = d;
  i = ai; /* invalid operands to binary operator = */
  pi = pc; /* invalid operand to binary operator = */
  pi2 = ai;
  pi2 = pi;
  ai = api; /* lvalue required in expression */
  ai = i; /* lvalue required in expression */
  ai = c;/* lvalue required in expression */
  api = c; /* lvalue required in expression */
  c = c;
  c = ac; /* invalid operand to binary operator = */
  c = i;
  c = d;
  pc = pi; /* invalid operand to binary operator = */
  ac = ac; /* lvalue required in expression */
  apc = apc; /* lvalue required in expression */
  ppc = apc;
  ppc = ppd; /* invalid operand to binary operator = */
  ppc = i; /* invalid operand to binary operator = */
  ppc = c;/* invalid operand to binary operator = */
  ppc = d;/* invalid operand to binary operator = */
  d = d;
  d = i;
  d = c;
  d = pi; /* invalid operand to binary operator = */
  d = ac; /* invalid operand to binary operator = */
  ad = d; /* lvalue required in expression */
  apd = apd; /* lvalue required in expression */
  ppd = ppi; /* invalid operand to binary operator = */
  ppd = ppc; /* invalid operand to binary operator = */
  ppd = i; /* invalid operand to binary operator = */
  ppd = c; /* invalid operand to binary operator = */
  ppd = d; /* invalid operand to binary operator = */
  foo = i; /* lvalue required in expression */
  bar = i; /* lvalue required in expression */
  1 = i; /* lvalue required in expression */
  ai[4] = c;
  ai[4] = i;
  ai[4] = ppi; /* invalid operand to binary operator = */
  ai[4] = api; /* invalid operand to binary operator = */
  ai[4] = ai[6];
  ai[4] = api[6]; /* invalid operand to binary operator = */
  api[9] = 1; /* invalid operand to binary operator = */
  api[9] = pi;
  api[9] = 2.3; /* invalid operand to binary operator = */
  ai[9] = 1;
  ai[9] = 2.3;
  apd[6] = c; /* invalid operand to binary operator = */
  apd[6] = d; /* invalid operand to binary operator = */
  apd[6] = pi; /* invalid operand to binary operator = */
  *pc = *pd;

  /* rechecking cast */
  /* all should be invalid operand in cast epression */
  (int) foo; /* invalid operand in cast expression */
  (char) foo; /* invalid operand in cast expression */
  (double) foo; 
  (int *) foo;
  (char *) foo;
  (double *) foo;
  (int **) foo;
  (char **) foo;
  (double **) foo;
  (int) bar;
  (char) bar;
  (double) bar;
  (int *) bar;
  (char *) bar;
  (double *) bar;
  (int **) bar;
  (char **) bar;
  (double **) bar;

  return c;
  return i;
  return ai; /* invalid return type */
  return d;
}
