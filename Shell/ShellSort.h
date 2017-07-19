#ifndef __SHELLSORT__
#define __SHELLSORT__
#define SedgewickNum int

#include "../Vector/Vector.h"
#include "../util.h"

//Sedgewick {min(9*4^i-9*2^i+1, 4^j-3*2^j+1)}从这两个序列大于0开始计i, j
//即i >= 0, j >= 2
class Sedgewick{

	Vector<SedgewickNum> SedgewickList;
	Rank r;

	void genSedgewickList(SedgewickNum n)//n is the size of the to sort list
	{
		int i = 1, j = 2;
		r = -1;
		SedgewickNum num = 1;
		while(num < n)
		{
			SedgewickList.insert(num);
			++r;
			int a = 9*pow(4, i)-9*pow(2, i)+1;
			int b = pow(4, j)-3*pow(2, j)+1;
			if(a <= b)
				{ num = a; ++i; }
			else { num = b; ++j; }
		}
	}
public:
	Sedgewick(SedgewickNum n)
	{
		genSedgewickList(n);
	}
	SedgewickNum operator()()//generate the next Sedgewick 
	{												 //number less than n 
		return SedgewickList[r--];
	}																   
};


template <typename T>
void insertSelect(T* A, int Sta, int end, int step)
{
	for(int i = Sta; i < end; i += step)
	{
		int toInsert = A[i];
		int j;
		for(j = i; (j > Sta) && (A[j-step] > toInsert); j -= step)
		{
			A[j] = A[j-step];
		}
		A[j] = toInsert;
	}
}


template <typename T>
void ShellSort(T* A, size_t n)
{
	Sedgewick Sedge = Sedgewick(n);
	for(SedgewickNum w = Sedge(); w > 1; w = Sedge())
	{
		int layer = (n % w) ? n / w : n / w - 1;
		for(int i = 0; i + layer * w < n; ++i){
			insertSelect(A, i, i + layer * w + 1, w);
		}
	}
	insertSelect(A, 0, n, 1);
}

#endif