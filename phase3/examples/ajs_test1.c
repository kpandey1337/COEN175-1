
int testfun(int z, int s, double y);

int testfun(int z, double s, double c); /* conflicting types for 'testfun' */

int testfun(int z, int s, double s);  /* redeclaration of 's' and conflicting types for 'testfun' */

int testfun(int z, int s, double d);

int testfun(int a, int b, double cds){
    char a;         /* redeclaration of 'a' */
    int a[100];    /* redeclaration of 'a' */

    if(a == b){
        a = 3;
    }
    else{
        a = 5;
    }

    testfun(a,1,c);  /* 'c' undeclared */
    undecFun(a,3);
    undecFun2(0);
}

int undecFun(int x, int y);

double undecFun2(int y, int z); /* conflicting types for 'undecFun2' */
