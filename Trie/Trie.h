#ifndef __TRIE__
#define __TRIE__

#define Kind 26
#define NodePosi Node*

struct Node {
  int count;
  NodePosi Child[Kind];
  Node():count(0){
	for (int i = 0; i < Kind; ++i)
	  Child[i] = nullptr;
  }
};

class Trie {
private:
  NodePosi header;
  static void clear(NodePosi root);
  static void insert(char * key, NodePosi root);
  static int search(char * key, NodePosi root);
public:
  Trie() { header = new Node; }
  ~Trie() { clear(header); }
  void insert(char * key);
  int search(char * key);
};

void Trie::clear(NodePosi root)
{
  for (int i = 0; i < Kind; ++i)
  {
	if (root->Child[i] != nullptr)
	  clear(root->Child[i]);
  }
  delete root;
}

inline void Trie::insert(char * key, NodePosi root)
{
  root->count++;
  char c;
  if ((c = key[0]) != '\0')
  {
	int code = c - 97;
	if (root->Child[code] == nullptr)
	  root->Child[code] = new Node;
	insert(key + 1, root->Child[code]);
  }
}

inline int Trie::search(char * key, NodePosi root)
{
  char c;
  if ((c = key[0]) != '\0')
  {
	int code = c - 97;
	if (root->Child[code] == nullptr)
	  return 0;
	return search(key + 1, root->Child[code]);
  }
  else return root->count;
}

void Trie::insert(char * key)
{
  insert(key, header);
}

int Trie::search(char * key)
{
  return search(key, header);
}



#endif // !__TRIE__
