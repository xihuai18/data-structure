#ifndef __FIB__
#define __FIB__

using dataType = long long;

class Fib
{
public:
	Fib(dataType n)//the first fibnacci number is not less than n;
	{ k_1 = 0; k = 1; while(k < n) { next(); } }
	dataType get(){ return k; }
	dataType next(){ k += k_1; k_1 = k - k_1; return k; }
	dataType prev(){ k_1 = k - k_1; k -= k_1; return k; }
private:
	dataType k, k_1;//k = fib(k), k_1 = fib(k-1);
};

#endif