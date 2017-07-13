#include "../BST/BST.h"

template <typename T> class RedBlack : public BST<T> { 
protected:
	void solveDoubleRed(BinNodePosi(T) x); 
	void solveDoubleBlack(BinNodePosi(T) x); 
	int updateHeight(BinNodePosi(T) x); 
public:
	BinNodePosi(T) insert(const T& e) override; 
	bool remove(const T& e) override; 
// BST::search()等其余接口可直接沿用
};