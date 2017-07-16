#ifndef __QUEUE__
#define __QUEUE__


#include "../List/List.h" 
template <typename T> class Queue: public List<T> { 
public: 
	void enqueue(T const& e) { List<T>::insertAsLast(e); } 
	T dequeue() { return remove(List<T>::first()); } 
	T& front() { return List<T>::first()->data; } 
};
//注意：dequeue返回左值，front返回右值。

#endif