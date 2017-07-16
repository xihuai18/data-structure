#ifndef __HASHTABLE__
#define __HASHTABLE__

#include "Dictionary.h" //引入词典ADT
#include "../Bitmap/Bitmap.h" //引入位图
#include "Entry.h"
#include <cstring>

template <typename K, typename V> //key、value
class Hashtable : public Dictionary<K, V> { //符合Dictionary接口的Hashtable模板类
   /*DSA*/friend class UniPrint;
private:
   Entry<K, V>** ht; //桶数组，存放词条指针
   int M; //桶数组容量
   int N; //词条数量
   Bitmap* lazyRemoval; //懒惰删除标记
#define lazilyRemoved(x)  (lazyRemoval->test(x))
#define markAsRemoved(x)  (lazyRemoval->set(x))
protected:
   int probe4Hit ( const K& k ); //沿关键码k对应的查找链，找到词条匹配的桶
   int probe4Free ( const K& k ); //沿关键码k对应的查找链，找到首个可用空桶
   void rehash(); //重散列算法：扩充桶数组，保证装填因子在警戒线以下
public:
   Hashtable ( int c = 5 ); //创建一个容量不小于c的散列表（为测试暂时选用较小的默认值）
   ~Hashtable(); //释放桶数组及其中各（非空）元素所指向的词条
   int size() const { return N; } // 当前的词条数目
   bool put ( K, V ); //插入（禁止雷同词条，故可能失败）
   V* get ( K k ); //读取
   bool remove ( K k ); //删除
};

#endif

int primeNLT(int c, int n, char* file) {
  Bitmap B(file, n);
  while (c < n)
  {
	if (B.test(c)) c++;
	else return c;
  }
  return c;
}

#include "hashCode.h"

template<typename K, typename V>
inline int Hashtable<K, V>::probe4Hit(const K & k)
{
  int r = hashCode(k) % M;
  int old_r = r;
  int i = 0;
  while ((ht[r] && (k != ht[r]->key)) || (!ht[r] && lazilyRemoved(r)))
  {
	++i;
	r = (old_r + i * i) % M;
  }
  return r;
}

template<typename K, typename V>
inline int Hashtable<K, V>::probe4Free(const K & k)
{
  int r = hashCode(k) % M;
  int old_r = r;
  int i = 0;
  while (ht[r]) { ++i; r = (old_r + i * i) % M; }
  return r;
}

template<typename K, typename V>
inline void Hashtable<K, V>::rehash()
{
  int old_capacity = M;
  Entry<K, V> ** old_ht = ht;
  M = primeNLT(2 * M, 1048576, "prime-1048576-bitmap.txt");
  N = 0; ht = new Entry<K, V>*[M];
  memset(ht, 0, sizeof(Entry<K, V> *) * M);
  release(lazyRemoval); lazyRemoval = new Bitmap(M);
  for (int i = 0; i < old_capacity; ++i)
	if (ht[i])
	  put(old_ht[i]->key, old_ht[i]->value);
  release(old_ht);
}

template<typename K, typename V>
inline Hashtable<K, V>::Hashtable(int c)
{
  M = primeNLT(c, 1048576, "prime-1048576-bitmap.txt");
  N = 0;
  ht = new Entry<K, V>*[M];
  memset(ht, 0, sizeof(Entry<K, V>*) * M);
  lazyRemoval = new Bitmap(M);//懒惰删除标记
}

template<typename K, typename V>
inline Hashtable<K, V>::~Hashtable()
{
  for (int i = 0; i < M; i++)
	if (ht[i]) release(ht[i]);
  release(ht);
  release(lazyRemoval);
}

template<typename K, typename V>
inline bool Hashtable<K, V>::put(K k, V v)
{
  if (ht[probe4Hit(k)]) return false;
  int r = probe4Free(k);
  ht[r] = new Entry<K, V>(k, v);
  ++N;
  if (N * 2 > M) rehash();
  return true;
}

template<typename K, typename V>
inline V * Hashtable<K, V>::get(K k)
{
  V ret = probe4Hit(k);
  return (ht[ret]) ? ht[ret]->value : nullptr;
}

template<typename K, typename V>
inline bool Hashtable<K, V>::remove(K k)
{
  int r = probeHit(k); if (!ht[r]) return false;
  release(ht[r]); ht[r] = nullptr;
  markAsRemoved(r);
  N--;
  return true;
}
