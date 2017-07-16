#ifndef __SPLAY__
#define __SPLAY__

#include "../BST/BST.h"

template <typename T> class Splay: public BST<T>
{
protected:
	using BST<T>::_root;
	using BST<T>::_size;
	using BST<T>::_hot;
	BinNodePosi(T) splay(BinNodePosi(T) v);//将结点v伸展至根
public:
	BinNodePosi(T) & search(const T& e)override; 
	BinNodePosi(T) insert(const T& e)override;
	bool remove(const T& e)override;
};

template <typename NodePosi> inline
void attachAsLChild(NodePosi p, NodePosi lc)
{
	p->lChild = lc;
	if(lc) lc->parent = p;
}

template <typename NodePosi> inline
void attachAsRChild(NodePosi p, NodePosi rc)
{
	p->rChild = rc;
	if(rc) rc->parent = p;
}

template <typename T>
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v)
{
	if(!v) return nullptr;
	BinNodePosi(T) p, g;
	while((p = v->parent) && (g = p->parent)) {
		auto gg = g->parent;
		if(IsLChild(*v))
			if(IsLChild(*p)){//zig-zig
				attachAsLChild(g, p->rChild);
				attachAsLChild(p, v->rChild);
				attachAsRChild(p, g);
				attachAsRChild(v, p);
			} else { //zig-zag
				attachAsLChild(p, v->rChild);
				attachAsRChild(g, v->lChild);
				attachAsRChild(v, p);
				attachAsRChild(v, g);
			}
		else if(IsRChild(*p)){ //zag-zag
			attachAsRChild(g, p->lChild);
			attachAsRChild(p, v->lChild);
			attachAsLChild(p, g);
			attachAsLChild(v, p);
		} else {//zag-zig
			attachAsRChild(p, v->lChild);
			attachAsLChild(g, v->rChild);
			attachAsRChild(v, g);
			attachAsLChild(v, p);
		}
		if(!gg) v->parent = nullptr;
		else (g==gg->lChild)? attachAsLChild(gg, v) : attachAsRChild(gg, v);
		updateHeight(g);
		updateHeight(p);
		updateHeight(v);
	}//此时g == nullptr, 但是p 不一定（当v原高度为奇数时，p为原树根） 
	if(p = v->parent)//赋值是否多余？
	{//single rotate
		if(IsLChild(*v)){
			attachAsLChild(p, v->rChild);
			attachAsRChild(v, p);
		} else {
			attachAsRChild(p, v->lChild);
			attachAsLChild(v, p);
		}
		updateHeight(p);
		updateHeight(v);
	}
	v->parent = nullptr; 
	return v;
}

template <typename T>
BinNodePosi(T) & Splay<T>::search(const T& e)
{
	auto p = searchIn(_root, e, _hot = nullptr);
	_root = splay((p ? p : _hot));
	return _root;
}

template <typename T>
BinNodePosi(T) Splay<T>::insert(const T& e)//插入到根上
{
	if(!_root) { _size++; return _root = new BinNode<T>(e); }
	if(e = search(e)->data) return _root;
	_size++;
	auto t = _root;
	if(_root->data < e){
		t->parent = _root = new BinNode<T>(e, nullptr, t, t->rChild);
		if(HasRChild(*t)) { t->rChild->parent = _root; t->rChild = nullptr; }
	} else {
		t->parent = _root = new BinNode<T>(e, nullptr, t, t->lChild);
		if(HasLChild(*t)) { t->lChild->parent = _root; t->lChild = nullptr; }
	}
	updateHeightAbove(t);
	return _root;
}

template <typename T>
bool Splay<T>::remove(const T& e)
{
	if (!_root || (e != search(e)->data)) return false;
	auto w = _root;
	if (!HasLChild(*_root)) { 
  	_root = _root->rChild; if (_root) _root->parent = nullptr;
  } else if (!HasRChild(*_root)) { 
  	_root = _root->lChild; if (_root) _root->parent = nullptr;
	} else {
		auto lTree = _root->lChild;
		lTree->parent = nullptr;
		_root->lChild = nullptr;
		_root = _root->rChild;
		_root->parent = nullptr;
		search(w->data);
		//原伸展树中的右子树的最小结点在根处
		attachAsLChild(_root, lTree);
	}
	release(w->data);
	release(w);
	--_size;
	if(_root) updateHeight(_root);
	return true;
}

#endif