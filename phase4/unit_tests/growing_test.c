
int foo(VOID);
int bar();

int main(){

	int a,b;

	int c = a || b;

	int d = a || foo;
	int e = a || bar;

}