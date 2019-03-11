int foo(int a, double b, char c){
	a = b;
}

int a;
double b;
char c;
int* pa;

int main(void){

	foo(a,b,c);
	foo((int) c,(double) a,(char) b);

}

int integers(int a, int b, double c, double d, double* e, double* f, int* g, char h, char* i){

	a || b;
	a && b;
	a || b && b || b || a;

	a == b;
	a != b;
	a == b && a != b;
	a != b || a == b;

	a >= b || a < b;
	a > b || a <= b;
	a > b && a <= b;
	a >= b && a < b;

/* additive */

	a + d + b;
	a - d + b;
	d + a - b;
	d - a - b;

	g + a;
	g - a;
	f + a;
	f - a;

	a + g;
	a + f;
	e - f;
	g - g;

/* mix */

	a + g && a + f || e - f && g - g;

/* multiplicative */

	a * b;
	a * c;
	a / b;
	a / c;
	a % b;
	h % a;
	a % h;

/* mix */

a + g && a + f || e - f && g - g || a * c && a / c || a % b && a % h;

/* prefix */

	*f;
	*g;
	*i;

	&a;

	-a;
	-c;
	-h;

	sizeof(int);
	sizeof(char);
	sizeof(double);
	sizeof(int*);
	sizeof(char*);
	sizeof(double*);

	(int) a;
	(int) c;
	(int) h;

	(double) a;
	(double) c;
	(double) h;

	(char) a;
	(char) c;
	(char) h;

	(char) (int) (double) a;
	(char)!*&c;
	(char) h;

/* Postfix */

	g[10 + 20];
	e[10 * 30];
	i[10 / 2];

	g[a];
	e[h];
	i[a];

/* Primary */


	foo(g, c, h);

	foo(h, c, h);

	foo((int) c, (double) a, (char) a);


	foo(10, 10.0, h);

	print(10,20);

	printf();

	(((10)));




}