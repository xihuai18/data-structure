#include "../BinTree/BinTree.h"
#include <algorithm>

template <typename T> class BST : public BinTree<T> { 
protected:
	BinNodePosi(T) _hot;//最后访问的结点
	BinNodePosi(T) connect34( 
		BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
		BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
	BinNodePosi(T) rotateAt(BinNodePosi(T) x); 
public: 
	virtual BinNodePosi(T) & search(const T& e); 
	virtual BinNodePosi(T) insert(const T& e);
	virtual bool remove(const T& e); 
};

template <typename T>
static BinNodePosi(T) & searchIn(BinNodePosi(T)& v, const T& e, BinNodePosi(T)& hot)
{
	if(!v || (e == v->data)) return v;
	hot = v;
	return searchIn((e > v->data? v->rChild:v->lChild), e, hot);
}

template <typename T>
BinNodePosi(T) & BST<T>::search(const T& e)
{ return searchIn(_root, e, _hot = nullptr); }

template <typename T>
BinNodePosi(T) & BST<T>::insert(const T& e)
{
	BinNodePosi(T) & x = search(e);
	if(x) return x;
	x = new BinNode<T>(e, _hot);
	_size++;
	updateHeightAbove(x);
	return x;
}

template <typename T>
bool BST<T>::remove(const T& e)
{
	BinNodePosi(T) & x = search(e);
	if(!x) return false;
	removeAt(x, _hot);
	_size--;
	updateHeightAbove(_hot);
	return true;
}

template <typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T)& x, BinNodePosi(T)& hot)
{
	BinNodePosi(T) w = x;//to delete
	BinNodePosi(T) succ = nullptr;//to replace
	if(!HasLChild(*x))
		succ = x = x->rChild;
	else if(!HasRChild(*x))
		succ = x = x->lChild;//succ != nullptr now;
	else {
		w = w->succ();
		swap(x->data, w->data);
		auto u = w->parent;
		((u == x) ? u->rChild:u->lChild) = succ = w->rChild; 
	}
	hot = w->parent;
	if(succ) succ->parent = hot;
	release(w->data);
	release(w);
	return succ;//return the node to replace
}

//3+4重构的反向联结必须有上层函数完成。
template <typename T>
BinNodePosi(T) connect34( 
		BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c,
		BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2,
		BinNodePosi(T) T3)
{
	a->lChild = T0; if(T0) T0->parent = a;
	a->rChild = T1; if(T1) T1->parent = a;
	updateHeight(a);
	c->lChild = T2; if(T2) T2->parent = c;
	c->rChild = T3; if(T3) T3->parent = c;
	updateHeight(c);
	b->lChild = a; a->parent = b;
	b->rChild = c; c->parent = b;
	updateHeight(b);
	return b;
}

template <typename T>
BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v)
{
	auto p = v->parent;
	auto g = p->parent;
	if(IsLChild(*p))//zig(clockwise)
	{
		if(IsLChild(*v)){//zig-zig
			p->parent = g->parent;
			return connect34(v, p, g, v->lChild, v->rChild, p->rChild, g->rChild);
		} else {//zig-zag
			v->parent = g->parent;
			return connect34(p, v, g, p->lChild, v->lChild, v->rChild, g->rChild);
		}
	} else {//zag
		if(IsRChild(*v)){//zag-zag
			p->parent = g->parent;
			return connect34(g, p, v, g->lChild, p->lChild, v->lChild, v->rChild);
		} else {//zag-zig
			v->parent = g->parent;
			return connect34(g, v, p, g->lChild, v->lChild, v->rChild, p->rChild);
		}
	}
}