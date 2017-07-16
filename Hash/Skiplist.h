#ifndef __SKIPLIST__ 
#define __SKIPLIST__ 

#include "../List/List.h" //引入列表
#include "Entry.h" //引入词条
#include "Quadlist.h" //引入Quadlist
#include "Dictionary.h" //引入词典

template <typename K, typename V> //key、value
//符合Dictionary接口的Skiplist模板类（但隐含假设元素之间可比较大小）
class Skiplist : public Dictionary<K, V>, public List<Quadlist<Entry<K, V>>*> {
using List<Quadlist<Entry<K, V>>*>::remove;
using List<Quadlist<Entry<K, V>>*>::last;
using List<Quadlist<Entry<K, V>>*>::empty;
using List<Quadlist<Entry<K, V>>*>::first;
protected:
   bool skipSearch (
      ListNode<Quadlist<Entry<K, V>>*>* &qlist,
      QuadlistNode<Entry<K, V>>* &p,
      K& k );
public:
   int size() const { return List<Quadlist<Entry<K, V>>*>::empty() ? 0 : List<Quadlist<Entry<K, V>>*>::last()->data->size(); } //底层Quadlist的规模
   int level() { return List<Quadlist<Entry<K, V>>*>::size(); } //层高 == #Quadlist，不一定要开放
   bool put ( K, V ); //插入（注意与Map有别——Skiplist允许词条重复，故必然成功）
   V* get ( K k ); //读取
   bool remove ( K k ); //删除
};


#endif

template<typename K, typename V>
inline bool Skiplist<K, V>::skipSearch(ListNode<Quadlist<Entry<K, V>>*>*& qlist, QuadlistNode<Entry<K, V>>*& p, K & k)
{
  while (true) {
	while (p->succ && p->entry.key <= k) p = p->succ;
	p = p->pred;
	if (p->pred && p->entry.key == k) return true;
	qlist = qlist->succ;
	if (!qlist) return false;
	p = (p->pred) ? p->below : qlist->data->first;
   }
}

template<typename K, typename V>
inline bool Skiplist<K, V>::put(K k, V v)
{
  Entry<K, V> e = Entry<K, V>(k, v);
  if (empty()) insertAsFirst(new Quadlist<Entry<K, V> >);
  ListNode<Quadlist<Entry<K, V>* > >* qlist = first();
  QuadlistNode<Entry<K, V>>* p = qlist->data->first();
  if (skipSearch(qlist, p, k))
	while (p->below) p = p->below;
  qlist = last();
  QuadlistNode<Entry<K, V> >* b = qlist->data->insertAfterAbove(e, p);

}

template<typename K, typename V>
inline V * Skiplist<K, V>::get(K k)
{
  if (empty()) return nullptr;
  ListNode<Quadlist<Entry<K, V>* > >* qlist = first();
  QuadlistNode<Entry<K, V>>* p = qlist->data->first();
  return skipSearch(qlist, p, k) ? &(p->entry.value) : nullptr;
}

template <typename K, typename V> bool Skiplist<K, V>::remove(K k) { //跳转表词条删除算法
  if (empty()) return false; //空表情况
  ListNode<Quadlist<Entry<K, V>>*>* qlist = first(); //从顶层Quadlist的
  QuadlistNode<Entry<K, V>>* p = qlist->data->first(); //首节点开始
  if (!skipSearch(qlist, p, k)) return false; //目标词条不存在，直接返回
  do { //若目标词条存在，则逐层拆除与之对应的塔
	QuadlistNode<Entry<K, V>>* lower = p->below; //记住下一层节点，并
	qlist->data->remove(p); //删除当前层节点，再
	p = lower; qlist = qlist->succ; //转入下一层
  } while (qlist->succ); //如上不断重复，直到塔基
  while (!empty() && first()->data->empty()) //逐一地
	List<Quadlist<Entry<K, V>>*>::remove(first()); //清除已可能不含词条的顶层Quadlist
  return true; //删除操作成功完成
}
