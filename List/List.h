#ifndef __LIST__
#define __LIST__


#include "ListNode.h"
#include <cstdlib>//rand()

template <typename T> class List {
private:
  int _size; 
  ListNodePosi(T) header; 
  ListNodePosi(T) trailer;
protected:
  void init();
  int clear();
  void copyNodes(ListNodePosi(T) l, int n);
  //排序中的n都是指向后n个(包括p)
  void merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m);
  void mergeSort(ListNodePosi(T)& p, int n);
  void selectionSort(ListNodePosi(T) p, int n);
  void insertionSort(ListNodePosi(T) p, int n);
public:

  List() { init(); }
  List(List<T> const& L);
  List(List<T> const& L, Rank r, int n);
  List(ListNodePosi(T) p, int n);
  ~List();

  Rank size() const { return _size; }
  bool empty() const { return _size <= 0; }
  T& operator[](Rank r) const;
  ListNodePosi(T) first() const { return header->succ; }
  ListNodePosi(T) last() const { return trailer->pred; }
  bool valid(ListNodePosi(T) p)
  {
	return p && (trailer != p) && (header != p);
  }
  int disordered() const;
  ListNodePosi(T) find(T const& e) const
  {
	return find(e, _size, trailer);
  }
  ListNodePosi(T) find(T const& e, int n, ListNodePosi(T) p) const;
  ListNodePosi(T) search(T const& e) const
  {
	return search(e, _size, trailer);
  }
  ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const;
  ListNodePosi(T) selectMax(ListNodePosi(T) p, int n);
  ListNodePosi(T) selectMax() { return selectMax(header->succ, _size); }

  ListNodePosi(T) insertAsFirst(T const& e);
  ListNodePosi(T) insertAsLast(T const& e);
  ListNodePosi(T) insertBefore(ListNodePosi(T) p, T const& e);
  ListNodePosi(T) insertAfter(ListNodePosi(T) p, T const& e);
  T remove(ListNodePosi(T) p);
  void merge(List<T>& L) { merge(first(), size, L, L.first(), L._size); }
  void sort(ListNodePosi(T) p, int n);
  void sort() { sort(first(), _size); }
  int deduplicate();
  int uniquify();
  void reverse();

  void traverse(void(*)(T&));
  template <typename VST>
  void traverse(VST&);
};

template<typename T>
inline void List<T>::init()
{
  header = new ListNode<T>; 
  trailer = new ListNode<T>; 
  header->succ = trailer; header->pred = NULL;
  trailer->pred = header; trailer->succ = NULL;
  _size = 0;
}

template<typename T>
inline int List<T>::clear()
{
  int OldSize = _size;
  while (0 < _size)
  {
	remove(header->succ);
  }
  return OldSize;
}

template<typename T>
inline void List<T>::copyNodes(ListNodePosi(T) l, int n)
{
  init();
  while (n--)
  {
	insertAsLast(l->data);
	l = l->succ;
  }
}

template<typename T>
inline void List<T>::merge(ListNodePosi(T)& p, int n, List<T>& L, ListNodePosi(T) q, int m)
{
  //注意this == L和rank(p) + n = rank(q)
  ListNodePosi(T) g = p->pred;
  while (m > 0)
  {
	if (n > 0 && (q->data >= p->data))
	{
	  if (q == (p = p->succ)) break;
	  --n;
	}
	else {
	  insertBefore(p, L.remove((q = q->succ)->pred));
	  --m;
	}
  }
  p = g->succ;
}

template<typename T>
inline void List<T>::mergeSort(ListNodePosi(T)& p, int n)
{
  if (n < 2) return ;
  int m = n >> 1;
  auto q = p;
  for (int i = 0; i < m; ++i )
  {
	q = q->succ;
  }
  mergeSort(p, m);
  mergeSort(q, n - m);
}

template<typename T>//比较范围包括p, 从p开始
inline void List<T>::selectionSort(ListNodePosi(T) p, int n)
{
  ListNodePosi(T) head = p->pred; ListNodePosi(T) tail = p;
  for (int i = 0; i < n; i++) tail = tail->succ; 
  while (1 < n) { 
	ListNodePosi(T) max = selectMax(head->succ, n); 
	insertBefore(tail, remove(max)); 
	tail = tail->pred; 
	n--;
  }
}

template<typename T>
inline void List<T>::insertionSort(ListNodePosi(T) p, int n)
{
  for (int i = 0; i < n; i++)
  {
	insertAfter(search(p->data, i, p), p->data);
	p = p->succ;
	remove(p->pred);
  }
}

template<typename T>
inline List<T>::List(List<T> const & L)
{
  copyNodes(L.first(), L.size());
}

template<typename T>
inline List<T>::List(List<T> const & L, Rank r, int n)
{
  auto p = L.first();
  while (--r > 0)
  {
	p = p->succ;
  }
  copyNodes(p, n);
}

template<typename T>
inline List<T>::List(ListNodePosi(T) p, int n)
{
  copyNodes(p, n);
}

template<typename T>
inline List<T>::~List()
{
  clear();
  delete header;
  delete trailer;
}

template<typename T>
inline T & List<T>::operator[](Rank r) const
{
  ListNodePosi(T) p = first();
  while (r-- > 0)
  {
	p = p->pred;
  }
  return p->data;
}

template<typename T>
inline int List<T>::disordered() const
{
  return 0;
}

template<typename T>//从p往前找最多n个(不包括p);
inline ListNodePosi(T) List<T>::find(T const & e, int n, ListNodePosi(T) p) const
{
  while (0 < n--)
  {
	if (e == (p = p->pred)->data) return p;
  }
  return nullptr;
}

template<typename T>
inline ListNodePosi(T) List<T>::search(T const & e, int n, ListNodePosi(T) p) const
{
  while (0 < n--)
  {
	if (((p = p->pred)->data) <= e) break;
  }
  return p;
}

template<typename T>//比较范围包括起始点, 且返回最前面的结点
inline ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n)
{
  auto max = p;
  for (auto cur = p; 1 < n; n--)
  {
	if ((cur = cur->succ)->data >= max->data)
	  max = cur;
  }
  return max;
}

template<typename T>
inline ListNodePosi(T) List<T>::insertAsFirst(T const & e)
{
  ++_size;
  return header->insertAsSucc(e);
}

template<typename T>
inline ListNodePosi(T) List<T>::insertAsLast(T const & e)
{
  ++_size;
  return trailer->insertAsPred(e);
}

template<typename T>
inline ListNodePosi(T) List<T>::insertBefore(ListNodePosi(T) p, T const & e)
{
  ++_size;
  return p->insertAsPred(e);
}

template<typename T>
inline ListNodePosi(T) List<T>::insertAfter(ListNodePosi(T) p, T const & e)
{
  ++_size;
  return p->insertAsSucc(e);
}

template<typename T>
inline T List<T>::remove(ListNodePosi(T) p)
{
  T e = p->data;
  p->pred->succ = p->succ;
  p->succ->pred = p->pred;
  delete p;
  _size--;
  return e;
}

template<typename T>
inline void List<T>::sort(ListNodePosi(T) p, int n)
{
  switch (rand() % 3)
  {
  case 1:insertionSort(p, n);
	break;
  case 2:selectionSort(p, n);
	break;
  default:mergeSort(p, n);
	break;
  }
}

template<typename T>
inline int List<T>::deduplicate()
{
  if (_size < 2) return 0;
  int OldSize = _size;
  auto p = header;
  Rank r = 0;
  while (trailer != (p = p->succ))
  {
	auto p = find(p->data, r, p);
	(p) ? remove(p) : r++;
  }
  return OldSize - _size;;
}

template<typename T>
inline int List<T>::uniquify()
{
  if (_size < 2) return 0; 
  int oldSize = _size; 
  ListNodePosi(T) p; 
  ListNodePosi(T) q; 
  for (p = header, q = p->succ; trailer != q; p = q, q = q->succ) 
	if (p->data == q->data) { remove(q); q = p; } 
  return oldSize - _size; 
}

template<typename T>
inline void List<T>::reverse()
{
  ListNodePosi(T) p = header->succ;
  ListNodePosi(T) q = trailer->pred->pred;
  while (header != q)
  {
	q->succ->succ = q;
	q->pred = q->succ;
  }
  header->succ = trailer->pred;
  trailer->pred->pred = header;
  trailer->pred = p;
  p->succ = trailer;
}

template<typename T>
inline void List<T>::traverse(void(*)(T &))
{
  for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ) visit(p->data);
}

template<typename T>
template<typename VST>
inline void List<T>::traverse(VST &)
{
  for (ListNodePosi(T) p = header->succ; p != trailer; p = p->succ) visit(p->data);
}


#endif