#ifndef __AVL__
#define __AVL__
#include "..\BST\BST.h"
//状态
#define Balanced(x) (stature((x).lChild) == stature((x).rChild))
#define BalFac(x) (stature((x).lChild) - stature((x).rChild))
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2))
#define tallerChild(x) (\
	stature((x)->lChild) > stature((x)->rChild) ? (x)->lChild : (\
	stature((x)->rChild) > stature((x)->lChild) ? (x)->rChild :	(\
	IsLChild(*(x)) ? (x)->lChild : (x)->rChild \
	)\
	)\
)

template <typename T> class AVL: public BST<T>
{
public:
	using BST<T>::_hot;
	using BST<T>::_size;
	using BST<T>::_root;
	BinNodePosi(T) insert(const T& e) override;
	bool remove(const T& e) override;
	//继承search()和rotateAt()等接口
};

template <typename T>
BinNodePosi(T) AVL<T>::insert(const T& e)
{
	BinNodePosi(T) & x = search(e);
	if(x) return x;
	x = new BinNode<T>(e, _hot);
	_size++;
	for(BinNodePosi(T) g = _hot; g; g = g->parent)
	{
		if(!AvlBalanced(*g)){
			FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
			break;
		} else 
			updateHeight(g);
	}
	return x;
}

template <typename T>
bool AVL<T>::remove(const T& e)
{
	BinNodePosi(T) & x = search(e);
	if(!x) return false;
	removeAt(x, _hot);
	_size--;
	for(BinNodePosi(T) g = _hot; g; g = g->parent)
	{
		if(!AvlBalanced(*g))
			FromParentTo(*g) = rotateAt(tallerChild(tallerChild(g)));
		updateHeight(g);
	}
}
#endif		