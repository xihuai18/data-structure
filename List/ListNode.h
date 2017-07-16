typedef int Rank;
#define ListNodePosi(T) ListNode<T>* 

template <typename T> struct ListNode {

  T data;
  ListNodePosi(T) pred;
  ListNodePosi(T) succ;

  ListNode() {}
  ListNode(T e, ListNodePosi(T) p = nullptr, ListNodePosi(T) s = nullptr)
	: data(e), pred(p), succ(s) {}

  ListNodePosi(T) insertAsPred(T const& e);
  ListNodePosi(T) insertAsSucc(T const& e);
};

template<typename T>
inline ListNodePosi(T) ListNode<T>::insertAsPred(T const & e)
{
  auto x = ListNode(e, pred, this);
  pred->succ = x;
  this->pred = x;
  return x;
}

template<typename T>
inline ListNodePosi(T) ListNode<T>::insertAsSucc(T const & e)
{
  auto x = ListNode(e, this, succ);
  succ->pred = x;
  this->succ = x;
  return x;
}