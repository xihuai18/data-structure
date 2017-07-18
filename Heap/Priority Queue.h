#include "../util.h"

template <typename T>
class PQ
{
public:
	virtual size_t size() = 0; 
	virtual void insert(T const & e) = 0;
	virtual T getMax() = 0;
	virtual T delMax() = 0; 
};