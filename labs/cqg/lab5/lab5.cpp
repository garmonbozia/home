#include <iostream>
#include <map>
#include <string>
#include <cctype>
#include <queue>

#include "wcomplex.h"
#include "mqueue.h"

using namespace std;


int main()
{
	//creating some wComplex elements
	wComplex A(1, 2);
	wComplex B(2, 4);
	wComplex C(0, 9);
	wComplex D(3, 1);
	wComplex E(2, 1); // E == A
	wComplex F(2, 7);
	wComplex G(7, 2); // F == G

	mQueue<wComplex> dq;
	cout << "\n\nEmpty dq. size= " << dq.msize() << endl;
	dq.mpush(A);
	dq.mpush(B);
	dq.mpush(C);
	dq.mpush(D);
	cout << "dq.size= " << dq.msize() << endl;
	dq.mpush(E);
	cout << "Equal element pushed, dq.size= " << dq.msize() << endl;
	dq.mpush(F);
	dq.mpush(G);
	cout << "7 elements pushed, 2 of them are equal to some another, " <<
		"so final size= " << dq.msize() << endl;
		
	cout << "\n dq elements:\n";
	wComplex tmp;

	// all dq.elements are pushed
	while(!dq.mempty())
	{
		tmp = dq.mfront();
		tmp.print();
		dq.mpop();
	}
	cout << "dq is empty, size= " << dq.msize() << endl;
	
	return 0;
}

