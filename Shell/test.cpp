#include "ShellSort.h"
#include "../util.h"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

void print(int *A, size_t n)
{
	for(int i = 0; i < n; ++i)
	{
		cout << A[i] << " ";
	}
	cout << endl;
}

int main()
{
	int n;
	cin >> n;
	int *A = new int [n];
	int dig;
	for(int i = 0; i < n; ++i)
	{
		cin >> dig;
		A[i] = dig;
	}
	print(A, n);
	ShellSort(A, n);
	print(A, n);
	return 0;
}