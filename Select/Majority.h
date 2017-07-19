#ifndef __MAJORITY__
#define __MAJORITY__

#include "../Vector/Vector.h"

template <typename T>
bool majority(Vector<T> A, T& maj)
{
	maj = majEleCandidate(A);
	return majEleCheck(A, maj);
}

template <typename T>
bool majEleCheck(Vector<T> A, T maj)
{
	int occurence = 0;
	for(int i = 0; i < A.size(); ++i)
	{
		if(A[i] == maj) occurence++;
	}
	return 2 * occurence > A.size();
}

/*
以下采用减治法，不断减去前缀
*/
template <typename T>
T majEleCandidate(Vector<T> A)
{
	T maj;
	for(int c = 0, i = 0; i < A.size(); ++i)
	{
		if(0 == c){
			maj = A[i];
			c = 1;
		} else {
			maj == A[i]? ++c : --c;
		}
	}
	return maj;//此maj为可能的众数（此处众数为出现次数不小于数字总数目的一半（上取整）的数，
	//所以该众数若存在，必为中位数。
}

#endif