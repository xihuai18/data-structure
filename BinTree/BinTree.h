#ifndef __BINTREE__
#define __BINTREE__
#include "BinNode.h"
#include "../release.h"

template <typename T> class BinTree
{
protected:
	int _size;
	BinNodePosi(T) _root;
	virtual int updateHeight(BinNodePosi(T) x);
	void updateHeightAbove(BinNodePosi(T) x);//更新x及其祖先
public:
	BinTree():_size(0), _root(nullptr){}
	~BinTree(){if(_size > 0) remove(_root); }
	int size() const { return _size; }
	bool empty() const { return !_root; } 
	//以下函数返回位置
	BinNodePosi(T) root() const { return _root; }
	BinNodePosi(T) insertAsRoot(T const& e);
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e);
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &Tr);
	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &Tr);
	int remove(BinNodePosi(T) x);
	BinTree<T>* secede(BinNodePosi(T) x);//摘除子树，并作为独立子树返回
	template <typename VST> //操作器
  void travLevel(VST& visit) { if (_root) _root->travLevel(visit); } //局次遍历
  template <typename VST> //操作器
  void travPre(VST& visit) { if (_root) _root->travPre(visit); } //先序遍历
  template <typename VST> //操作器
  void travIn(VST& visit) { if (_root) _root->travIn(visit); } //中序遍历
  template <typename VST> //操作器
  void travPost(VST& visit) { if (_root) _root->travPost(visit); } //后序遍历
  //operators
  bool operator<(BinTree<T> const& t) { return _root && t._root && (_root < t._root); }
	bool operator<=(BinTree<T> const& t) { return _root && t._root && (_root <= t._root); }
	bool operator>(BinTree<T> const& t) { return _root && t._root && (_root > t._root); }
	bool operator>=(BinTree<T> const& t) { return _root && t._root && (_root >= t._root); }
	bool operator==(BinTree<T> const& t) { return _root && t._root && (_root == t._root); }
	bool operator!=(BinTree<T> const& t) { return _root && t._root && (_root != t._root); }

};

template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x)
{
	return x->height = 1 + max(stature(x->lChild), stature(x->rChild));
}

template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x)
{
	while(x) {
		updateHeight(x);
		x = x->parent;
	}
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e)
{
	_size = 1;
	return _root = new BinNode<T>(e);
}

template <typename T> 
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e)
{ 
	_size++; 
	x->insertAsLC(e); 
	updateHeightAbove(x); 
	return x->lChild; 
}

template <typename T> 
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e)
{ 
	_size++; 
	x->insertAsRC(e); 
	updateHeightAbove(x); 
	return x->rChild; 
}

template <typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>* &Tr)
{
	if(x->lChild = Tr->_root)
		x->lChild->parent = x;
	_size += Tr->_size;
	updateHeightAbove(x);
	Tr->_root = nullptr;
	Tr->_size = 0;
	release(Tr);
	Tr = nullptr;
	return x;
}

template <typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &Tr)
{
	if(x->rChild = Tr->_root)
		x->rChild->parent = x;
	_size += Tr->_size;
	updateHeightAbove(x);
	Tr->_root = nullptr;
	Tr->_size = 0;
	release(Tr);
	Tr = nullptr;
	return x;
}

template <typename T>
static int removeAt(BinNodePosi(T) x)
{
	if(!x) return 0;
	int n = 1 + removeAt(x->lChild) + removeAt(x->rChild);
	release(x->data);
	release(x);
	return n;
}

template <typename T>
int BinTree<T>::remove(BinNodePosi(T) x)
{
	FromParentTo(*x) = nullptr;
	updateHeightAbove(x->parent);
	int n = removeAt(x);
	_size -= n;
	return n;
}

template <typename T>
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x)
{
	FromParentTo(*x) = NULL; 
	updateHeightAbove(x->parent); 
	BinTree<T>* S = new BinTree<T>; 
	S->_root = x; 
	x->parent = NULL; 
	S->_size = x->size(); 
	_size -= S->_size; 
	return S; 
}

#endif