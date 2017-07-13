#include "../Queue/Queue.h"
#include "../Stack/Stack.h"

#define BinNodePosi(T) BinNode<T>* //方便使用
#define stature(p) ((p)? (p)->height:-1)//高度
//性质判断
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lChild))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rChild))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lChild)
#define HasRChild(x) ((x).rChild)
#define HasChild(x) (HasLChild(x) || HasRChild(x)) //至少拥有一个孩子
#define HasBothChild(x) (HasLChild(x) && HasRChild(x)) //同时拥有两个孩子
#define IsLeaf(x) (!HasChild(x))
//返回具有关系的指针和结点
#define sibling(p) ( \
IsLChild(*(p)) ? \
(p)->parent->rChild : \
(p)->parent->lChild \
) //兄弟
#define uncle(x) ( \
IsLChild(*((x)->parent)) ? \
(x)->parent->parent->rChild : \
(x)->parent->parent->lChild \
) //叔叔
#define FromParentTo(x) ( \
IsRoot(x) ? _root : ( \
IsLChild(x) ? (x).parent->lChild : (x).parent->rChild \
) \
) //来自父亲的指针

typedef enum { RB_RED, RB_BLACK} RBColor;

template <typename T> struct BinNode {
	T data;
	BinNodePosi(T) parent;
	BinNodePosi(T) lChild;
	BinNodePosi(T) rChild;
	int height;
	int npl; //Null Path Length, Used in Left_Style_tree;
	RBcolor color;
	//constructors
	BinNode():parent(nullptr), lChild(nullptr), rChild(nullptr), height(0), npl(1), color(RB_RED){}
	BinNode(T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lc = nullptr, BinNodePosi(T) rc = nullptr, int h = 0, int l = 1, RBcolor c = RB_RED):
	:date(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c){} 
	//operation
	int size(); //size of sub_tree
	BinNodePosi(T) insertAsLC(T const&);
	BinNodePosi(T) insertAsRC(T const&);
	BinNodePosi(T) succ(); //取中序的直接后继
	template <typename VST> void travLevel(VST&);
	template <typename VST> void travPre(VST&);
	template <typename VST> void travIn(VST&);
	template <typename VST> void travPost(VST&);
	//operators
	bool operator < (BinNode const &bn){ return data < bn.data; }
	bool operator <= (BinNode const &bn){ return data <= bn.data; }
	bool operator > (BinNode const &bn){ return data > bn.data; }
	bool operator >= (BinNode const &bn){ return data >= bn.data; }
	bool operator == (BinNode const &bn){ return data == bn.data; }
	bool operator != (BinNode const &bn){ return data != bn.data; }

};

template <typename T>
int BinNode<T>::size(){
	int s = 1;
	if(lChild) s += lChild->size();
	if(rChild) s += rChild->size();
	return s;
}

template <typename T>
int BinNode<T>::insertAsLC(T const& e){

	return lChild = new BinNode(e, this);
}

template <typename T>
int BinNode<T>::insertAsLC(T const& e){
	return rChild = new BinNode(e, this);
}

template <typename T>
BinNodePosi(T) BinNode<T>::succ(){
	BinNodePosi(T) s = this;
	if(rChild){
		s = rChild;
		while(HasLChild(*s)) { s = s->lChild; }
	} else {
		while(IsRChild(*s)) { s = s->parent; }
		s = s->parent;
	}
	return s;
}

template <typename T>
 template <typename VST> //VST is the function used to visit the node
	void BinNode<T>::travLevel(VST& visit) {
		Queue<BinNodePosi(T)> Q;
		Q.enqueue(this);
		while(!Q.empty()) {
			BinNodePosi(T) x = Q.dequeue();
			visit(x);
			if (HasLChild(*x)) Q.push(x->lChild);
			if (HasRChild(*x)) Q.push(x->rChild);
		}
	}

template <typename T>
 template <typename VST> //VST is the function used to visit the node
	void BinNode<T>::travPre(VST& visit)
	{
		Stack<BinNodePosi(T)> S;
		if(x) S.push(x);
		while(!S.empty()) {
			x = S.top();
			visit(x);
			if (HasRChild(*x)) S.push(x->rChild);
			if (HasLChild(*x)) S.push(x->lChild);
			S.pop();
		}
	}

template <typename T>
 template <typename VST> //VST is the function used to visit the node
	void BinNode<T>::travIn(VST& visit)
	{
		if(this){
			lChild->travIn(visit);
			visit(*this);
			rChild->travIn(visit);
		}
	}

template <typename T>
 template <typename VST> //VST is the function used to visit the node
	void BinNode<T>::travPost(VST& visit)
	{
		if(this){
			lChild->travPost(visit);
			rChild->travPost(visit);
			visit(*this);
		}
	}
