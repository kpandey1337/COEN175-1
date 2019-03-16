int a, b;
int *c;

/*
int compare(void){
	a < b;
	a > b;
	a <= b;
	a >= b;
	a == b;
	a != b;
}
*/

/*
int or(void){
	a || b;
}

int and(void){
	a && b;
}
*/

/*
int not(void){
	!a;
}

int negate(void){
	-a;
}
*/

/*
int pointers(void){
	&*a;
	*a;
	&a;
}
*/
/*
int string(void){
	"asofiasf!";
	"1234567890";
}
*/
/*
int assign(void){
	a = b + 2;
	a = a + 3;
}
*/

int deref_assign(void){
	a = 69;
	*c = a;
	printf("%d", *c);
}