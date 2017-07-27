#ifndef _COMPLHEAP__
#define _COMPLHEAP__

#include "../util.h"

#define  InHeap(n, i)      ( ( ( -1 ) < ( i ) ) && ( ( i ) < ( n ) ) ) //判断PQ[i]是否合法
#define  Parent(i)         ( ( i - 1 ) >> 1 ) //PQ[i]的父节点（floor((i-1)/2)，i无论正负）
#define  LastInternal(n)   Parent( n - 1 ) //最后一个内部节点（即末节点的父亲）
#define  LChild(i)         ( 1 + ( ( i ) << 1 ) ) //PQ[i]的左孩子
#define  RChild(i)         ( ( 1 + ( i ) ) << 1 ) //PQ[i]的右孩子
#define  ParentValid(i)    ( 0 < i ) //判断PQ[i]是否有父亲
#define  LChildValid(n, i) InHeap( n, LChild( i ) ) //判断PQ[i]是否有一个（左）孩子
#define  RChildValid(n, i) InHeap( n, RChild( i ) ) //判断PQ[i]是否有两个孩子
#define  Bigger(PQ, i, j)  ( lt( PQ[i], PQ[j] ) ? j : i ) //取大者（等时前者优先）
#define  ProperParent(PQ, n, i) /*父子（至多）三者中的大者*/ \
            ( RChildValid(n, i) ? Bigger( PQ, Bigger( PQ, i, LChild(i) ), RChild(i) ) : \
            ( LChildValid(n, i) ? Bigger( PQ, i, LChild(i) ) : i \
            ) \
            ) //相等时父节点优先，如此可避免不必要的交换

#include "Priority Queue.h"
#include "../Vector/Vector.h"

template<typename T> class PQ_ComplHeap: public PQ<T>, public Vector<T>
{
  using Vector<T>::_size;
  using Vector<T>::_elem;
public:
  PQ_ComplHeap();
  PQ_ComplHeap(T * A, Rank n);
  virtual size_t size() { return _size; }
  virtual void insert(T const & e) override;
  virtual T getMax() override;
  virtual T delMax() override;
protected:
  Rank percolateDown(Rank n, Rank i);
  Rank percolateUp(Rank i);
  void heapify(Rank n);

};


template<typename T>
inline PQ_ComplHeap<T>::PQ_ComplHeap()
{ }

template<typename T>
inline PQ_ComplHeap<T>::PQ_ComplHeap(T * A, Rank n)
{
  copyFrom(A, 0, n); heapify(n);
}

template<typename T>
inline void PQ_ComplHeap<T>::insert(T const & e)
{
  Vector<T>::insert(e);
  percolateUp(_size - 1);
}

template<typename T>
inline T PQ_ComplHeap<T>::getMax()
{
  return _elem[0];
}

template<typename T>
inline T PQ_ComplHeap<T>::delMax()
{
  T max = _elem[0];
  _elem[0] = _elem[--_size];
  percolateDown(_size, 0);
  return max;
}

template<typename T>
inline Rank PQ_ComplHeap<T>::percolateDown(Rank n, Rank i)
{
  Rank p;
  while (i != (p = ProperParent(_elem, n, i)))
  {
	swap(_elem[i], _elem[p]);
	i = p;
  }
  return i;
}

template<typename T>
inline Rank PQ_ComplHeap<T>::percolateUp(Rank i)//返回上滤最终抵达的位置
{
  while (ParentValid(i))
  {
	Rank p = Parent(i);
	if (lt(_elem[i], _elem[p])) break;
	swap(_elem[i], _elem[p]);
  i = p;
  }
  return i;
}

template<typename T>
inline void PQ_ComplHeap<T>::heapify(Rank n)
{
  for (Rank i = LastInternal(n); InHeap(n, i); --i)
  {
	percolateDown(n, i);
  }
}



#endif