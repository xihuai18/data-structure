#include "Trie.h"
#include <iostream>

char text[13][10] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "one", "three", "ten" };

int main()
{
  Trie tree;
  for (int i = 0; i < 13; ++i)
	tree.insert(text[i]);
  for (int i = 0; i < 10; ++i)
	std::cout << tree.search(text[i]) << std::endl;
  return 0;
}