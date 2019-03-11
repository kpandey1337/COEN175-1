int foo(int x, int *y)
{
    int a[10], *p;
    /*, i, ;
    double d, *s;*/

   /* p = &d;			invalid operands to binary = */
    p = &a;			 /*lvalue required in expression */
    *a = 0;

}