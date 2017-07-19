#ifndef __VECTOR__
#define __VECTOR__


#include "../util.h"
typedef int Rank; 
#define DEFAULT_CAPACITY 30

template <typename T>
class PQ_ComplHeap;

template <typename T> class Vector { 
protected:
	Rank _size; 
	int _capacity; 
	T* _elem; 
	void copyFrom(T const* A, Rank lo, Rank hi); //ok
	void expand(); //ok
	void shrink(); //ok
  bool bubble(Rank lo, Rank hi); //ok
	void bubbleSort(Rank lo, Rank hi); //ok
	void merge(Rank lo, Rank mi, Rank hi); //ok
	void mergeSort(Rank lo, Rank hi); //ok
	void heapSort(Rank lo, Rank hi);//ok
	Rank partition(Rank lo, Rank hi); //ok
	void quickSort(Rank lo, Rank hi); //ok
	

public:
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) 
	{ _elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v); } 
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } 
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); } 
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } 
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } 
 
	~Vector() { delete [] _elem; } 
 
	Rank size() const { return _size; } 
	bool empty() const { return !_size; } 
	int disordered() const; //ok
	//find 意味着寻找值与e相等的元素，如果有多个，返回最后一个。
	Rank find(T const& e) const { return find(e, 0, _size); } 
	Rank find(T const& e, Rank lo, Rank hi) const; //ok
	Rank search(T const& e) const 
	{ return (0 >= _size) ? -1 : search(e, 0, _size); }
	Rank search(T const& e, Rank lo, Rank hi) const; //ok

	T& operator[](Rank r) const; //ok
	Vector<T> & operator=(Vector<T> const&); //ok
	T remove(Rank r); //ok
	int remove(Rank lo, Rank hi); //ok 
	Rank insert(Rank r, T const& e); //ok
	Rank insert(T const& e) { return insert(_size, e); } 
	void sort(Rank lo, Rank hi); 
	void sort() { sort(0, _size); } 
	void unsort(Rank lo, Rank hi); //ok
	void unsort() { unsort(0, _size); }
	int deduplicate(); //ok
	int uniquify(); //ok

	void traverse(void (*)(T&)); 
	template <typename VST> void traverse(VST&); 

};

template <typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi)
{
	_elem = new T[_capacity = 2 * (hi - lo)];
	_size = 0;
	while(lo < hi) {
		_elem[_size++] = A[lo++];
	}
}

template <typename T>
Vector<T> & Vector<T>::operator=(Vector<T> const& V)
{
	if(_elem) delete [] _elem;
	copyFrom(V._elem, 0, V._size());
	return *this;
}

template <typename T> 
void Vector<T>::expand() 
{ 
	if (_size < _capacity) return; 
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY; 
	T* oldElem = _elem; _elem = new T[_capacity <<= 1];
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i]; 
	delete [] oldElem;
}

template <typename T> void Vector<T>::shrink() 
{
	if (_capacity < DEFAULT_CAPACITY << 1) return; 
	if (_size << 2 > _capacity) return; //装填因子小于25%
	T* oldElem = _elem; _elem = new T[_capacity >>= 1]; 
	for (int i = 0; i < _size; i++) 
		_elem[i] = oldElem[i]; 
	delete [] oldElem;
}

template <typename T> 
T& Vector<T>::operator[](Rank r) const
{ return _elem[r]; }

template <typename T> void Vector<T>::unsort(Rank lo, Rank hi) { 
	T* V = _elem + lo; 
	for (Rank i = hi - lo; i > 0; i--) 
		Swap(V[i - 1], V[rand() % i]); 
}

template <typename T> 
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const { 
	while ((lo < hi--) && (e != _elem[hi])); 
	return hi; 
}//return -1 if failed

template <typename T> 
Rank Vector<T>::insert(Rank r, T const& e) { 
	expand(); 
	for (int i = _size; i > r; i--) _elem[i] = _elem[i-1]; 
	_elem[r] = e; _size++; 
	return r; 
}

template <typename T>
int Vector<T>::remove(Rank lo, Rank hi) {
	if(lo == hi) return 0;
	while(hi < _size) {
		_elem[lo++] = _elem[hi++];
	}
	_size = lo;
	shrink();
	return hi - lo;
}

template <typename T> 
T Vector<T>::remove(Rank r) { 
	T e = _elem[r]; 
	remove(r, r + 1); 
	return e; 
}

template <typename T>
int Vector<T>::deduplicate() {
	int oldSize = _size;
	Rank i = 1;
	while(i < _size) {
		(find(_elem[i], 0, i) < 0)?
		i++ : remove(i);
	}
	return oldSize - _size;
}

template <typename T> 
	template <typename VST> 
		void Vector<T>::traverse(VST& visit) 
		{ for (int i = 0; i < _size; i++) visit(_elem[i]); }

template <typename T> 
struct Increase 
{ virtual void operator()(T& e) { e++; } };

template <typename T> 
void increase(Vector<T> & V) 
{ V.traverse(Increase<T>()); }

template <typename T> 
int Vector<T>::disordered() const { 
	int n = 0; 
	for (int i = 1; i < _size; i++) 
		if (_elem[i - 1] > _elem[i]) n++; 
	return n;
}//ordered only if n == 0

template <typename T>//清除相邻 
int Vector<T>::uniquify() { 
	Rank i = 0, j = 0; 
	while (++j < _size) 
	if (_elem[i] != _elem[j]) 
		_elem[++i] = _elem[j]; 
	_size = ++i; shrink(); 
	return j - i; 
}

#include "../Fibonacci/Fib.h"
template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const
{
	Fib fib(hi - lo);
	while(lo < hi) {
		while(hi - lo < fib.get()) { fib.prev(); }
		Rank mi = lo + fib.get() - 1;
		if(e < _elem[mi]) hi = mi;
		else lo = mi + 1;
	}
	return --lo;//返回不大于e的元素的最大秩；
}

/*
普通二分
// 二分查找算法（版本C）：在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
template <typename T> static Rank binSearch(T* A, T const& e, Rank lo, Rank hi) {
while (lo < hi) { //每步迭代仅需做一次比较刞断，有两个分支
Rank mi = (lo + hi) >> 1; //以中点为轴点
(e < A[mi]) ? hi = mi : lo = mi + 1; //经比较后确定深入[lo, mi)或(mi, hi)
} //成功查找不能提前终止
return --lo; //循环结束时，lo为大亍e癿元素的最小秩，故lo - 1即丌大亍e癿元素的最大秩
} //有多个命中元素时，总能保证返回秩最大者；查找失败时，能够返回失败的位置
*/

template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi)
{
	while(!bubble(lo, hi--)) {}
}

template <typename T> 
bool Vector<T>::bubble(Rank lo, Rank hi)
{
	bool sorted = true;
	while(++lo < hi) {
		if(_elem[lo - 1] > _elem[lo])
		{
			sorted = false;
			Swap(_elem[lo - 1], _elem[lo]);
		}
	} 
	return sorted;
}

template <typename T> 
void Vector<T>::mergeSort(Rank lo, Rank hi) { 
  if (hi - lo < 2) return; 
  int mi = (lo + hi) >> 1; 
  mergeSort(lo, mi); mergeSort(mi, hi); merge(lo, mi, hi); 
  }

template<typename T>
inline void Vector<T>::heapSort(Rank lo, Rank hi)
{
  PQ_ComplHeap<T> H(_elem - lo, hi - lo);
  while (!H.empty())
  {
	_elem[--hi] = H.delMax();
  }
}

template <typename T> 
void Vector<T>::merge(Rank lo, Rank mi, Rank hi) { 
  T* A = _elem + lo; 
  int lb = mi - lo; T* B = new T[lb]; 
  for (Rank i = 0; i < lb; B[i] = A[i]) { i++; }
  int lc = hi - mi; T* C = _elem + mi; 
  for (Rank i = 0, j = 0, k = 0; (j < lb) || (k < lc); ) { 
  	if ( (j < lb) && ( !(k < lc) || (B[j] <= C[k]) ) ) A[i++] = B[j++];
  	if ( (k < lc) && ( !(j < lb) || (C[k] < B[j]) ) ) A[i++] = C[k++];
  }
  delete [] B;
} 

template <typename T>
void Vector<T>::sort(Rank lo, Rank hi)
{ heapSort(lo, hi); }

/*
unstable 
4 parts lo (lo, mi] (mi, k) [k, hi)
*/
template <typename T>
Rank Vector<T>::partition(Rank lo, Rank hi)
{
	Swap(_elem[lo], _elem[lo + rand() % (hi - lo - 1)]);
	T p = _elem[lo];
	Rank mi = lo;
	for(Rank k = lo + 1; k < hi; ++k)
	{
		if(_elem[k] < p)
			Swap(_elem[++mi], _elem[k]);
	}
	Swap(_elem[lo], _elem[mi]);
	return mi;
}

template <typename T>
void Vector<T>::quickSort(Rank lo, Rank hi)
{
	if(hi - lo < 2) return ;
	Rank mi = partition(lo, hi);
	quickSort(lo, mi + 1);
	quickSort(mi + 1, hi);
} 
#endif