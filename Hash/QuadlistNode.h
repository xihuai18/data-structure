#ifndef __QLISTNODE__
#define __QLISTNODE__

#include "Entry.h"
#define QlistNodePosi(T)  QuadlistNode<T>* //跳转表节点位置

template <typename T> struct QuadlistNode { //QuadlistNode模板类
   T entry; //所存词条
   QlistNodePosi(T) pred;  QlistNodePosi(T) succ; //前驱、后继
   QlistNodePosi(T) above; QlistNodePosi(T) below; //上邻、下邻
   QuadlistNode //构造器
   ( T e = T(), QlistNodePosi(T) p = NULL, QlistNodePosi(T) s = NULL,
     QlistNodePosi(T) a = NULL, QlistNodePosi(T) b = NULL )
      : entry ( e ), pred ( p ), succ ( s ), above ( a ), below ( b ) {}
   QlistNodePosi(T) insertAsSuccAbove //插入新节点，以当前节点为前驱，以节点b为下邻
   (T const& e, QlistNodePosi(T) b = NULL)
   {
	 auto newNode = QuadlistNode<T>(e, this, b, nullptr, nullptr);
	 this->succ->pred = newNode;
	 this->succ = newNode;
	 if (b) {
	   if (b->above)
		 b->above->below = newNode;//doesn't need
	   b->above = newNode;
	 }
	 return newNode;
   }
};

#endif