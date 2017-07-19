#include "../Vector/Vector.h"
#include "LinearSelect.h"
#include "../util.h"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

int main()
{
	cout << "Enter the number of the digits:\t";
	size_t n;
	cin >> n;
	int digit;
	Vector<int> A;
	for(size_t i = 0; i < n; ++i)
	{
		cin >> digit;
		A.insert(digit);
	}
	A.insert(666);
	cout <<endl;
	for(int i = 0; i < A.size(); ++i)
	{
		cout << A[i] << " ";
	}
	cout << endl;

	cout << a << " " << b << endl;
	for(int i = 0; i < A.size(); ++i)
	{
		cout << A[i] << " ";
	}
	cout << endl;
	//insert is right
	cout << "Enter k:\t";
	int k;
	cin >> k;
	cout << "The " << k << "th number is " << linearSelect(A, k) << endl;
	return 0;
}