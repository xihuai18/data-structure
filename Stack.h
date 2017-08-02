#ifndef __STACK__
#define __STACK__


#include <stdlib.h>

typedef int Rank;
template <typename T>
class Stack
{
public:
  Stack(size_t cap) :_capacity(cap), _size(0), _top(-1) { _data = new T[_capacity]; }
  ~Stack() { delete[] _capacity; }
  T& top(){ return _data[_top]; }
  T pop() { --_size;; return _data[_top--]; }
  void push(T d) { _data[++_top] = d; ++_size; }
  bool empty() { return _size == 0; }
  bool full() { return _size >= _capacity; }

private:
  T *_data;
  size_t _size;
  size_t _capacity;
  Rank _top;

};




#endif // !__STACK__
