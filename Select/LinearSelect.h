#ifndef __LINEARSELECT__
#define __LINEARSELECT__

#include "../Vector/Vector.h"

int const Q = 5;

template <typename T>
T trivialSelect(Vector<T> & A, Rank	k)
{
	A.sort(0, A.size());
	return A[k];
}

template <typename T>
T linearSelect(Vector<T> & A, Rank k)
{
	size_t n = A.size();
	if(n < Q) return trivialSelect(A, k);
	Vector<T> mis;
	for(int i = 0, interval = n / Q; i < Q; ++i)
	{
		if(i == Q - 1) 
		{
			mis.sort(i * interval, n);
			mis.insert(A[(i * interval + n) / 2]);
		} else {
			mis.sort(i * interval, (i + 1) * interval); 
			mis.insert(A[(2 * i + 1) / 2 * interval]);
		}
	}
	T M = linearSelect(mis, mis.size() / 2);
	Vector<T> L, E, G;
	for(Rank i = 0; i < A.size(); ++i)
	{
		if(A[i] > M) G.insert(A[i]);
		else if(A[i] < M) L.insert(A[i]);
		else E.insert(A[i]);
	}
	if(L.size() >= k) return linearSelect(L, k);
	else if(L.size() + E.size() >= k) return M;
	else return linearSelect(G, k - L.size() - E.size());
}

#endif