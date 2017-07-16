#ifndef __REDBLACK__
#define __REDBLACK__


#include "../BST/BST.h"
#include <cstdlib>

#define IsBlack(p) (!(p) || (RB_BLACK == (p)->color)) //外部节点也视作黑节点
#define IsRed(p) (!IsBlack(p)) //非黑即红
//以上传入指针，因为要判断外部结点，以下传入对象
#define BlackHeightUpdated(x) (\
(stature((x).lChild) == stature((x).rChild)) && \
((x).height == (IsRed(&x) ? stature((x).lChild) : stature((x).lChild) + 1)) \
)

template <typename T> class RedBlack : public BST<T> { 
protected:
	using BST<T>::_root;
	using BST<T>::_hot;
	using BST<T>::_size;
	void solveDoubleRed(BinNodePosi(T) x); 
	void solveDoubleBlack(BinNodePosi(T) x); 
	int updateHeight(BinNodePosi(T) x); 
public:
	BinNodePosi(T) insert(const T& e) override; 
	bool remove(const T& e) override; 
// BST::search()等其余接口可直接沿用
};

template<typename T>
inline void RedBlack<T>::solveDoubleRed(BinNodePosi(T) x)
{
  if (IsRoot(*x)) { _root->color = RB_BLACK; _root->height++; return ; }
  BinNodePosi(T) p = x->parent; if (IsBlack(p)) return;
  BinNodePosi(T) g = p->parent;//because p is red, g must be black
  BinNodePosi(T) u = uncle(x);
  if (IsBlack(u)) {
	if (IsLChild(*x) == IsLChild(*p)){ p->color = RB_BLACK; }
	else x->color = RB_BLACK;
	g->color = RB_RED;
	BinNodePosi(T) gg = g->parent;
	BinNodePosi(T) r = FromParentTo(*g) = rotateAt(x);
	r->parent = gg;
  }
  else {
	p->color = RB_BLACK; p->height++;
	x->color = RB_BLACK; u->height++;
	if (!IsRoot(*g)) g->color = RB_RED;
	solveDoubleRed(g);
  }
}

template<typename T>
inline void RedBlack<T>::solveDoubleBlack(BinNodePosi(T) r)
{
  BinNodePosi(T) p = r ? r->parent : _hot; if (!p) return;
  BinNodePosi(T) s = sibling(r);
  if (IsBlack(s)) {
	BinNodePosi(T) t = nullptr;//if t == nullptr, then BB-2-B
	if (HasLChild(*s) && IsRed(s->lChild)) t = s->lChild;
	else if (HasRChild(*s) && IsRed(s->rChild)) t = s->rChild;
	if (t)//BB-1
	{
	  auto oldColor = p->color;
	  BinNodePosi(T) b = FromParentTo(*p) = rotateAt(t);
	  b->color = oldColor;
	  updateHeight(b);
	  if (HasLChild(*b)) b->lChild->color = RB_BLACK; updateHeight(b->lChild);
	  if (HasRChild(*b)) b->rChild->color = RB_BLACK; updateHeight(b->rChild);
	}
	else {
	  s->color = RB_RED; s->height--;
	  if (IsRed(p))//BB-2-R 
	 {
		p->color = RB_BLACK;
	  }
	  else {//BB-2-B
		p->height--;//为了将情况合并到已处理过的情况，将s染红，则p的黑高度下降1，类似与删除p的父节点。
		solveDoubleBlack(p);
	  }
	}
  }
  else {
	s->color = RB_BLACK; 
	p->color = RB_RED;
	auto t = IsLChild(*s) ? s->lChild : s->rChild;
	_hot = p;
	FromParentTo(*p) = rotateAt(t);
	solveDoubleBlack(r);//changed to BB-1 or BB-2-R
  }
}

template<typename T>
inline int RedBlack<T>::updateHeight(BinNodePosi(T) x)
{
  x->height = max(stature(x->lChild), stature(x->rChild));
  return IsBlack(x) ? x->height++ : x->height;
}

template<typename T>
inline BinNodePosi(T) RedBlack<T>::insert(const T & e)
{
  BinNodePosi(T) & x = search(e);
  if (x) return x;
  x = new BinNode<T>(e, _hot, NULL, NULL, -1);
  _size++;
  solveDoubleRed(x);
}

template<typename T>
inline bool RedBlack<T>::remove(const T & e)
{
  BinNodePosi(T) & x = search(e); if (!x) return false;
  BinNodePosi(T) r = removeAt(x, _hot); if (0 >= --_size) return true;
  if (!_hot) { _root->color = RB_BLACK; updateHeight(_root); return true; }
  if (BlackHeightUpdated(*(_hot)))
	return true;
  if (IsRed(r))
  {
	r->color = RB_BLACK; r->height++; return true;
  }
  solveDoubleBlack(r);
  return true;
}


#endif