#ifndef __QUICKSELECT__
#define __QUICKSELECT__

#include "../Vector/Vector.h"

template <typename T>
T quickSelect(Vector<T> & A, Rank k)
{
	for(Rank lo = 0, hi = A.size(); hi = A.size(); lo < hi; )
	{
		Rank i = lo, j = hi - 1;
		T pivot = A[lo];
		while(i < j) {
			while((i < j) && (pivot <= A[j])) { j--; }
			A[i] = A[j];
			while((i < j) && (pivot >= A[i])) { i++; }
			A[j] = A[i];
		}
		A[i] = pivot;
		if(k <= i) hi = i;
		if(i <= k) lo = i + 1;
	}
	return A[k];
}


















#endif