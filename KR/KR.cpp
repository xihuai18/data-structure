#include "../util.h"
#include <iostream>

#define M 727//a prime
#define R 256//进制
#define DIGIT(S, i) ((S)[i])//取一位数
typedef long long HashCode;
bool check(char *T, char *P, size_t i);//从下标为i开始对比
HashCode perpareDm(size_t m);//最高位权
void updateHash(HashCode & hashT, char * T, size_t m, size_t i, HashCode Dm);
//新的首位i
int match(char * T, char * P);

HashCode perpareDm(size_t m)//最高位权
{
	HashCode Dm = 1;
	for(size_t i = 1; i < m; ++i) Dm = (R * Dm) % M;
	return Dm;
}

bool check(char *T, char *P, size_t i)//从下标为i开始对比
{
	size_t m = strlen(P);
	for(size_t j = 0; j < m; ++j)
		if(T[i + j] != P[j]) return false;
	return true;
}

int match(char * T, char * P)
{
	size_t n = strlen(T), m = strlen(P);
	HashCode Dm = perpareDm(m), hashT = 0, hashP = 0;
	for(size_t t = 0; t < m; ++t)
	{
		hashT = (hashT * R + DIGIT(T, t)) % M;
		hashP = (hashP * R + DIGIT(P, t)) % M;
	}
	size_t i = 0;
	while(true) {
		if(hashT == hashP)
			if(check(T, P, i))
				return i;
		if(++i > n - m) return -1;
		else updateHash(hashT, T, m, i, Dm);
	}
}

void updateHash(HashCode & hashT, char * T, size_t m, size_t i, HashCode Dm)
{
	hashT = (hashT - Dm * DIGIT(T, i - 1)) % M;
	hashT = (hashT * R + DIGIT(T, i + m - 1)) % M;
	if(0 > hashT) hashT += M;//保证为正数
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