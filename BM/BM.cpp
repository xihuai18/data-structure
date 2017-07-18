#include "..\util.h"
#include <iostream>
/*
-------------------------------------------------------------------------------
BM算法有两个策略BC（Bad Char），GS（Good Suffix）；
BC策略吸取某个字符不能匹配的教训，GS策略得到成功匹配的经验。
-------------------------------------------------------------------------------
*/


int * BuildBc(char * P)
{
	int * bc = new int [256];
	for(int i = 0; i < 256; ++i) bc[i] = -1;
	for(int i = 0, m = strlen(P); i < m; ++i)
	{
		bc[P[i]] = i;
	}
	return bc;
}

/*
hi, lo是上一个成功匹配的界（lo，hi]
case 1：
j在(lo, hi]，对应的下标为m-hi+j-1, 当ss[m-hi+j-1] < j-lo时，ss[j] = ss[m-hi+j-1]；
case 2：
j在(lo, hi]，对应的下标为m-hi+j-1, 当ss[m-hi+j-1] > j-lo时, 对于(j-ss[m-hi+j-1], lo], 需要与(m-hi+j-1-ss[m-hi+j-1], m-hi+lo-1]对比；
case 3：
j不在(lo, hi], 直接暴力。
其中，case2和3可以合并，同时注意hi和lo的转移，还有(m-hi+j-1-ss[m-hi+j-1], m-hi+j-1] = (m-1-ss[m-hi+j-1], m-1]。
*/
int * BuildSs(char * P)
{
	int m = strlen ( P ); int* ss = new int[m];
  ss[m - 1]  =  m;
   for ( int lo = m - 1, hi = m - 1, j = lo - 1; j >= 0; j -- )
      if ( ( lo < j ) && ( ss[m - hi + j - 1] <= j - lo ) )//case 1 
         ss[j] =  ss[m - hi + j - 1]; 
      else { //case 2 & 3
         hi = j; lo = min ( lo, hi );//合并case2和3
         while ( ( 0 <= lo ) && ( P[lo] == P[m - hi + lo - 1] ) ) 
            lo--;
         ss[j] = hi - lo;
      }
   return ss;
}

int * BuildGs(char * P)
{
	int * ss = BuildSs(P);
	int m = strlen(P);
	int * gs = new int[m];
	for ( size_t j = 0; j < m; j ++ ) gs[j] = m;
	for(size_t j = m - 1, i = 0; j > 0; --j) {
		if(j + 1 == ss[j])
			while(i < m - j - 1) {
				gs[i++] = m - j - 1;
			}
	}
	for(size_t j = 0; j < m - 1; ++j)
	{
		if(P[j - ss[j] - 1] != P[m - ss[j] - 1])
			gs[m - ss[j] - 1] = m - j - 1;
	}
	delete [] ss;
	return gs;
}

int match(char * T, char * P)
{
	int * gs = BuildGs(P);
	int * bc = BuildBc(P);
	size_t i = 0;
	int n = strlen(T), m = strlen(P);
	while(n >= i + m) {
		int j = m - 1;
		while(P[j] == T[i + j]) {
			if(0 > --j) break;
		}
		if(0 > j) break;
		i += max(j - bc[T[i + j]], gs[j]);
	}
	delete []gs;
	delete []bc;
	return (n < i + m) ? -1 : i;
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