#ifndef __UNION_FIND__
#define __UNION_FIND__

/*
集合的名字是其根的名字
路径压缩指的是将Find过程中的所有结点的根设置为最终的根
合并有两种实现方式：
1. 按大小合并，_set[root]的值为该集合大小的相反数，初始化为-1，小集合并入大集合，，每次合并将其相加。
2. 按高度合并, _set[root]的值为集合深度的相反数，初始化为0，即单个结点深度为0，当且仅当合并的两个集合深度相等。
*/

#include "../util.h"
using SetType = int;
using ElementType = int;

class DisjSet {
private:
  SetType* _set;
  size_t _size;
protected:
  //按大小合并版本
  //void init()
  //{
  //  _set = new int[_size];
  //  for (int i = 0; i < _size; ++i)
  //    _set[i] = -1;
  //}
  //按高度合并版本
  void init()
  {
	_set = new int[_size];
	for (int i = 0; i < _size; ++i)
	  _set[i] = 0;
  }
public:
  //按高度合并
  void Union(SetType& fir, SetType& sec)
  {
	if (_set[fir] < _set[sec])
	  _set[sec] = fir;
	else {
	  if (_set[fir] == _set[sec])
		_set[sec]--;
	  _set[fir] = sec;
	}
  }

  //按大小合并
//  void Union(SetType& fir, SetType& sec)
//  {
//	if (_set[fir] < _set[sec]) {
//	  _set[fir] += _set[sec];
//	  _set[sec] = fir;
//	}
//	else {
//	  _set[sec] += _set[fir];
//	  _set[fir] = sec;
//	}
//  }

  SetType Find(ElementType& e) {
	if (_set[e] <= 0)
	  return e;
	else {
	  return _set[e] = Find(e);
	}
  }
};

#endif