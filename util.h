#ifndef __UTIL__
#define __UTIL__

#include "comp.h"
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <cstring>
#include <typeinfo.h>
#include <cstddef>

template <typename T>
void Swap(T& a, T& b)
{                                                                       
	T tmp = a;
	a = b;
	b = tmp;
}

template <typename T>
void Swap(T*& a, T*& b)
{
	swap(*a, *b);
}

template <typename T>
T Max(T a, T b)
{
	return lt(a, b) ? b : a;
}

template <typename T>
T Min(T a, T b)
{
	return lt(a, b) ? a : b;
}

#endif