#ifndef __TWOD_TREE__
#define __TWOD_TREE__

#include "../util.h"
#include <iostream>

#define TwoDNodePosi(T) TwoDNode<T>*

template <typename T>
struct TwoDNode {
  T _data[2];
  TwoDNodePosi(T) LChild;
  TwoDNodePosi(T) RChild;
  TwoDNode(T *d):LChild(nullptr), RChild(nullptr) { _data[0] = d[0]; _data[1] = d[1]; }
};

template <typename T>
class TwoDT
{
public:
  void insert(T *d);
  void printRangeSearch(T* lo, T*hi);

private:
  static void insert(T* d, TwoDNodePosi(T) & root, int level);
  static void printRangeSearch(T* lo, T* hi, TwoDNodePosi(T) & root, int level);
  TwoDNodePosi(T) _root = nullptr;
};


template<typename T>
void TwoDT<T>::insert(T* d, TwoDNodePosi(T) & root, int level)
{
  if (root == nullptr)
	root = new TwoDNode<T>(d);
  else {
	if (d[level] < root->_data[level])
	  insert(d, root->LChild, 1 - level);
	else insert(d, root->RChild, 1 - level);
  }
}

template<typename T>
void TwoDT<T>::printRangeSearch(T * lo, T * hi, TwoDNodePosi(T)& root, int level)
{
  if (root != nullptr)
  {
	if (root->_data[0] >= lo[0] && root->_data[0] <= hi[0] &&
	  root->_data[1] >= lo[1] && root->_data[1] <= hi[1])
	  std::cout << "(" << root->_data[0] << ", " << root->_data[1] << ") ";
	if (root->_data[level] <= hi[level])
	  printRangeSearch(lo, hi, root->RChild, 1 - level);
	if (root->_data[level] >= lo[level])
	  printRangeSearch(lo, hi, root->LChild, 1 - level);
  }
}


template<typename T>
void TwoDT<T>::insert(T * d)
{
  insert(d, _root, 0);
}

template<typename T>
void TwoDT<T>::printRangeSearch(T * lo, T * hi)
{
  printRangeSearch(lo, hi, _root, 0);
}


#endif // !__TWOD_TREE__

