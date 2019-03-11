int f(void);
int *j(int k, int l, double m, int *n, char o);

int main(int a, double b, char c, int *d, int **e, int **g)
{
  double **h;
  int k, l;
  double m;
  int *n;
  char o;
  int i[5];

  /* Logical Expression */
  a || b; /* works */
  a || f; /* invalid operands to binary || */

  a && b; /* works */
  a && f; /* invalid operands to binary && */

  /* Equality Expression */
  a == b; /* works */
  a == c; /* works */
  e == g; /* works */
  d == e; /* invalid operands to binary == */
  e == h; /* invalid operands to binary == */
  f == a; /* invalid operands to binary == */

  a != b; /* works */
  a != c; /* works */
  e != g; /* works */
  d != e; /* invalid operands to binary != */
  e != h; /* invalid operands to binary != */
  f != a; /* invalid operands to binary != */

  /* Relational Expressions */
  a <= b; /* works */
  a <= c; /* works */
  e <= g; /* works */
  d <= e; /* invalid operands to binary <= */
  e <= h; /* invalid operands to binary <= */
  f <= a; /* invalid operands to binary <= */

  a >= b; /* works */
  a >= c; /* works */
  e >= g; /* works */
  d >= e; /* invalid operands to binary >= */
  e >= h; /* invalid operands to binary >= */
  f >= a; /* invalid operands to binary >= */

  a < b; /* works */
  a < c; /* works */
  e < g; /* works */
  d < e; /* invalid operands to binary < */
  e < h; /* invalid operands to binary < */
  f < a; /* invalid operands to binary < */

  a > b; /* works */
  a > c; /* works */
  e > g; /* works */
  d > e; /* invalid operands to binary > */
  e > h; /* invalid operands to binary > */
  f > a; /* invalid operands to binary > */

  /* Additive Expressions */
  c + a; /* works */
  c + b; /* works */
  a + a; /* works */
  a + b; /* works */
  b + b; /* works */
  d + a; /* works */
  a + d; /* works */
  d + d; /* invalid operands to binary + */
  a + f; /* invalid operands to binary + */

  c - a; /* works */
  c - b; /* works */
  a - a; /* works */
  a - b; /* works */
  b - b; /* works */
  d - a; /* works */
  d - d; /* works */
  a - d; /* invalid operands to binary - */
  a - f; /* invalid operands to binary - */
  e - d; /* invalid operands to binary - */
  h - e; /* invalid operands to binary - */

  /* Multiplicative Expression */
  a * a; /* works */
  a * b; /* works */
  b * a; /* works */
  b * b; /* works */
  a * c; /* works */
  a * d; /* invalid operands to binary * */
  d * d; /* invalid operands to binary * */
  a * f; /* invalid operands to binary * */

  a / a; /* works */
  a / b; /* works */
  b / a; /* works */
  b / b; /* works */
  a / c; /* works */
  a / d; /* invalid operands to binary / */
  d / d; /* invalid operands to binary / */
  a / f; /* invalid operands to binary / */

  a % a; /* works */
  a % c; /* works */
  a % b; /* invalid operands to binary % */
  b % a; /* invalid operands to binary % */
  a % d; /* invalid operands to binary % */
  a % f; /* invalid operands to binary % */

  /* Prefix Expression */
  *d; /* works */
  *e; /* works */
  **e; /* works */
  *i; /* works */
  *a; /* invalid operand to unary * */
  *b; /* invalid operand to unary * */
  *f; /* invalid operand to unary * */

  &a; /* works */
  &*d; /* works */
  &*e; /* works */
  &**e; /* works */
  &i[2]; /* works */
  &d; /* works */
  &i; /* lvalue required in expression */
  &(a + a); /* lvalue required in expression */
  &f; /* lvalue required in expression */
  &(&a); /* lvalue required in expression */
  &sizeof(int); /* lvalue required in expression */
  &13; /* lvalue required in expression */
  &"test"; /* lvalue required in expression */

  !a; /* works */
  !f; /* invalid operand to unary ! */

  -a; /* works */
  -b; /* works */
  -c; /* works */
  -d; /* invalid operand to unary - */
  -e; /* invalid operand to unary - */
  -f; /* invalid operand to unary - */

  sizeof(int); /* works */

  (int) a; /* works */
  (int) b; /* works */
  (int) c; /* works */
  (int *) d; /* works */
  (int *) e; /* works */
  (int **) d; /* works */
  (int **) e; /* works */
  (double *) d; /* works */
  (int) d; /* works */
  (int *) a; /* works */
  (char) c; /* works */
  (int *) c; /* invalid operand in cast expression */
  (char) d; /* invalid operand in cast expression */
  (double) d; /* invalid operand in cast expression */
  (int *) b; /* invalid operand in cast expression */
  (int *) f; /* invalid operand in cast expression */

  /* Postfix Expression */
  d[3]; /* works */
  e[3]; /* works */
  d[a]; /* works */
  d[c]; /* works */
  a[3]; /* invalid operands to binary [] */
  b[3]; /* invalid operands to binary [] */
  c[3]; /* invalid operands to binary [] */
  d[b]; /* invalid operands to binary [] */
  d[d]; /* invalid operands to binary [] */

  /* Primary Expression */
  "test"[1]; /* works */
  f(); /* works */
  q(); /* works */
  j(k, l, m, n, o); /* works */
  a(); /* called object is not a function */
  d(); /* called object is not a function */
  j(k, l, m, n); /* invalid arguments to called function */
  j(o, n, m , l, k); /* invalid arguments to called function */

  /* Statements */
  while(a){} /* works */
  if(a){} /* works */
  while (f){} /* invalid type for test expression */
  if(f){} /* invalid type for test expression */
  if(a+f)   /* invalid operands to binary + */
    a;
  while(a+f) /* invalid operands to binary + */
    a;

  a = 3; /* works */
  i[4] = 3; /* works */
  *d = 4; /* works */
  d = &a; /* works */
  e = &d; /* works */
  d = *e; /* works */
  d = i; /* works */
  d = e[2]; /* works */
  d = &e; /* invalid operands to binary = */
  e = *d; /* invalid operands to binary = */
  e = g[2]; /* invalid operands to binary = */
  i = 3; /* lvalue required in expression */
  d = 3; /* invalid operands to binary = */
  (a + a) = 3; /* lvalue required in expression */
  a = f; /* invalid operands to binary = */
  f = a; /* lvalue required in expression */
  e = &i; /* lvalue required in expression */
  d = e; /* invalid operands to binary = */
  h = e; /* invalid operands to binary = */
  f = (a+f); /* invalid operands to binary + */

  return a; /* work */

  return f(); /* works */

  while(a)    /* works */
    return a;

  if(a)
    return a;   /* works */
  else
    return a;   /* works */

  return (f+a); /* invalid operands to binary + */

  return j(k, l, m, n, o); /* invalid return type */

  return d; /* invalid return type */
}
