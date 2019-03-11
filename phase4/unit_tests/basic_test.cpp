#include <cassert>
#include <iostream>
# include <cstdlib>

#include "../code/tokens.h"
#include "../code/Type.h"

//g++ basic_test.cpp ../code/Type.cpp -o typetest

using namespace std;


int checkParametersEquality(Parameters &left, Parameters &right){

	if(left.size() != right.size()){
		return 0;
	}

	Type l;
	Type r;

	for(int i = 0; i < left.size(); i++){
		if(left[i] != right[i]){
			l = left[i];
			r = right[i];
			cout << "\t" << l << endl;
			cout << "\t" << r << endl;
			return 0;

		}
	}
	return 1;

}

int main(){

	Type t(INT, 0);
/*	cout << t << endl;
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
*/
	//fails ..
	//fails ..

	Parameters *p1, *p2;
	p1 = new Parameters();
	p2 = new Parameters();

	if(p1 == p2){
		cout << "WORK" << endl;
	}

	p1->push_back(t);
	p2->push_back(t);

	if(p1 == p2){
		cout << "EQUALITY WORKING" << endl;
	}

	p1->push_back(Type(INT, 0));
	p2->push_back(Type(CHAR, 0));

	if(p1 == p2){
		cout << "EQUALITY BROKEN" << endl;
	}

	cout << checkParametersEquality(*p1, *p2) << endl;


}



