
#ifndef __PQ_LEFTHEAP__
#define __PQ_LEFTHEAP__ 

#include "Priority Queue.h"
#include "../util.h"
#include "../BinTree/BinTree.h"


template <typename T>
class PQ_LeftHeap : public PQ<T>, public BinTree<T>
{
  using BinTree<T>::_size;
  using BinTree<T>::_root;
public:
  size_t size() { return _size; }
  T delMax();
  T getMax() { return _root->data; }
  void insert(T e);
};

template <typename T>
static BinNodePosi(T) merge(BinNodePosi(T) a, BinNodePosi(T) b)
{
  if (!a) return a;
  if (!b) return b;
  if (lt(a->data, b->data)) swap(a, b);
  a->rChild = merge(a->rChild, b);
  a->rChild->parent = a;
  if (!a->lChild || lt(a->lChild->npl, a->rChild->npl)) swap(a->lChild, a->rChild);
  a->npl = (a->rChild) ? a->rChild->npl + 1 : 1;
  return a;
}

template<typename T>
inline T PQ_LeftHeap<T>::delMax()
{
  BinNodePosi(T) lHeap = _root->lChild;
  BinNodePosi(T) rHeap = _root->rChild;
  T ret = _root->data;
  _root = merge(lHeap, rHeap);
  if (_root) _root->parent = nullptr;
  return ret;
}

template<typename T>
inline void PQ_LeftHeap<T>::insert(T e)
{
  BinNodePosi(T) v = new BinNode<T>(e);
  _root = merge(_root, v);
  _root->parent = nullptr;
  ++_size;
}


#endif // !__PQ_LEFTHEAP__

