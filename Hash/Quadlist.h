#ifndef __QUADLIST__
#define __QUADLIST__

#include "QuadlistNode.h" //引入Quadlist节点类
template <typename T> class Quadlist { //Quadlist模板类
private:
   int _size; QlistNodePosi(T) header; QlistNodePosi(T) trailer; //规模、头哨兵、尾哨兵
protected:
   void init(); //Quadlist创建时的初始化
   int clear(); //清除所有节点
public:
// 构造函数
   Quadlist() { init(); } //默认
// 析构函数
   ~Quadlist() { clear(); delete header; delete trailer; } //删除所有节点，释放哨兵
// 只读访问接口
   Rank size() const { return _size; } //规模
   bool empty() const { return _size <= 0; } //判空
   QlistNodePosi(T) first() const { return header->succ; } //首节点位置
   QlistNodePosi(T) last() const { return trailer->pred; } //末节点位置
   bool valid ( QlistNodePosi(T) p ) //判断位置p是否对外合法
   { return p && ( trailer != p ) && ( header != p ); }
// 可写访问接口
   T remove ( QlistNodePosi(T) p ); //删除（合法）位置p处的节点，返回被删除节点的数值
   QlistNodePosi(T) //将*e作为p的后继、b的上邻插入
   insertAfterAbove ( T const& e, QlistNodePosi(T) p, QlistNodePosi(T) b = NULL );
// 遍历
   void traverse ( void (* ) ( T& ) ); //遍历各节点，依次实施指定操作（函数指针，只读或局部修改）
   template <typename VST> //操作器
   void traverse ( VST& ); //遍历各节点，依次实施指定操作（函数对象，可全局性修改节点）
}; //Quadlist

#endif

template<typename T>
inline void Quadlist<T>::init()
{
  _size = 0;
  header = new QuadlistNode<T>();
  trailer = new QuadlistNode<T>();
  header->above = nullptr;
  trailer->above = nullptr;
  header->below = nullptr;
  trailer->below = nullptr;
  header->succ = trailer;
  trailer->pred = trailer;
}

template<typename T>
inline int Quadlist<T>::clear()
{
  int oldSize = _size;
  while (_size > 0)
  {
	remove(header->succ);
  }
  return oldSize;
}

template<typename T>
inline T Quadlist<T>::remove(QlistNodePosi(T) p)
{
  p->succ->pred = p->pred;
  p->pred->succ = p->succ;
  T e = p->Entry.value;
  delete p;
  return e;
}

template<typename T>
inline QlistNodePosi(T) Quadlist<T>::insertAfterAbove(T const & e, QlistNodePosi(T) p, QlistNodePosi(T) b)
{
  _size++;
  return p->insertAsSuccAbove(e, b);
}
