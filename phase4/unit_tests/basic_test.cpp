#include <cassert>
#include <iostream>

#include "../code/tokens.h"
#include "../code/Type.h"

//g++ basic_test.cpp ../code/Type.cpp -o typetest

using namespace std;

int main(){

	Type t(INT, 0);
	cout << t << endl;
	cout << t.promote() << endl;

	Type u(CHAR, 0);
	cout << u << endl;
	cout << u.promote() << endl;
	assert(t == u.promote());

	Type v(CHAR, 3, 10);
	cout << v << endl;
	cout << v.promote() << endl;

	assert(t.isCompatibleWith(u));
	assert(u.isCompatibleWith(t));

	Type charptr(CHAR, 3);
	Type moreiptr(INT, 3);

	//fails ..
	//fails ..

}