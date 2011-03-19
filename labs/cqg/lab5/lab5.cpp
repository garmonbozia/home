#include <iostream>

#include "wcomplex.h"
#include "mqueue.h"

using std::cout;
using std::endl;

int main()
{
	wComplex A(1, 2);
	wComplex B(2, 4);
	wComplex C(0, 9);
	wComplex D(3, 1);
	wComplex E(2, 1); // E == A
	wComplex F(2, 7);
	wComplex G(7, 2); // F == G

	if(E == A) 
		cout << "E == A" << endl;

	mQueue<wComplex> dq;
	cout << "pushing elements..." << endl;
	dq.push(A);
	dq.push(B);
	dq.push(C);
	dq.push(D);
	dq.push(E);
	dq.push(F);
	dq.push(G);
	cout << "complete\n" << endl;
	
	wComplex tmp;
	cout << "poping all elements..." << endl;
	while(!dq.empty())
	{
		tmp = dq.pop();
		tmp.print();
	}
	cout << "complete\n" << endl;
	
	return 0;
}

