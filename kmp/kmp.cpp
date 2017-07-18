#include <cstring>
#include <iostream>
/*
注意next[j]是长度为j的字符串相等的真前缀和真后缀的长度
此处的i是next[j]或递归的next[j]
*/
int * buildNext(char * p)
{
	int m = strlen(p);
	int i, j;
	int * next = new int [m];
	i = next[0] = -1;
	j = 0;
	while(j < m - 1) {
		if(0 > i || p[j] == p[i]){
			++j, ++i;
			next[j] = next[i] != next[j] ? i : next[i];//改进的kmp，即真前缀和真后缀要在下一位上不同。
		}
		else i = next[i];
	}
	return next;
}

int match(char * T, char * p)
{
	int * next = buildNext(p);
	int i, j;
	i = j = 0;
	int n = strlen(T), m = strlen(p);
	while(i < n && j < m) {
		if(j < 0 || T[i] == p[j])//若j < 0，则T[i]一定已经与p[0]比较过；
		{
			++i;
			++j;
		} else {
			j = next[j];
		}
	}
	delete [] next;
	return n - (i - j) >= m ? i - j : -1;
}

int main()
{
	char T[1000], p[1000];
	std::cout << "enter a Text :\n";
	std::cin >> T;
	std::cout << "enter a Pattern :\n";
	std::cin>>p;
	std::cout << "Result:\t";
	std::cout << match(T, p) << std::endl;
	return 0;
}