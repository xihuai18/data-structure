#ifndef __MEDIAN__
#define __MEDIAN__

#include "../Vector/Vector.h"

template <typename T>
T trivialMedian (Vector<T>& S1, int lo1, int n1, Vector<T>& S2, int lo2, int n2)
{
	int hi1 = lo1 + n1;
	int hi2 = lo2 + n2;
	Vector<T> S;
	while((lo1 < hi1) && (lo2 < hi2)) {
		while((lo1 < hi1) && S1[lo1] <= S2[lo2]) { S.insert(S1[lo1++]); }
		while((lo2 < hi2) && S1[lo1] >= S2[lo2]) { S.insert(S2[lo1++]); }
	}
	while(lo1 < hi1) { S.insert(S1[lo1++]); }
	while(lo2 < hi2) { S.insert(S2[lo2++]); }
	return S[(n1 + n2) / 2];
}

/*
template <typename T> //序列S1[lo1, lo1 + n)和S2[lo2, lo2 + n)分别有序，n > 0，数据项可能重复
T median ( Vector<T>& S1, int lo1, Vector<T>& S2, int lo2, int n ) { //中位数算法（高效版）
   if ( n < 3 ) return trivialMedian ( S1, lo1, n, S2, lo2, n ); //递归基
   int mi1 = lo1 + n / 2, mi2 = lo2 + ( n - 1 ) / 2; //长度（接近）减半 (n-1)/2 = (n+1)/2 - 1。
   if ( S1[mi1] < S2[mi2] )
      return median ( S1, mi1, S2, lo2, n + lo1 - mi1 ); //取S1右半、S2左半
   else if ( S1[mi1] > S2[mi2] )
      return median ( S1, lo1, S2, mi2, n + lo2 - mi2 ); //取S1左半、S2右半
   else
      return S1[mi1];
}*/

template <typename T> //向量S1[lo1, lo1 + n1)和S2[lo2, lo2 + n2)分别有序，数据项可能重复
T median ( Vector<T>& S1, int lo1, int n1, Vector<T>& S2, int lo2, int n2 ) { //中位数算法
   if ( n1 > n2 ) return median ( S2, lo2, n2, S1, lo1, n1 ); //确保n1 <= n2
   if ( n2 < 6 ) //递归基：1 <= n1 <= n2 <= 5
      return trivialMedian ( S1, lo1, n1, S2, lo2, n2 );
   ///////////////////////////////////////////////////////////////////////
   //                lo1            lo1 + n1/2      lo1 + n1 - 1
   //                 |                 |                 |
   //                 X >>>>>>>>>>>>>>> X >>>>>>>>>>>>>>> X
   // Y .. trimmed .. Y >>>>>>>>>>>>>>> Y >>>>>>>>>>>>>>> Y .. trimmed .. Y
   // |               |                 |                 |               |
   // lo2     lo2 + (n2-n1)/2       lo2 + n2/2     lo2 + (n2+n1)/2    lo2 + n2 -1
   ///////////////////////////////////////////////////////////////////////
   if ( 2 * n1 < n2 ) //若两个向量的长度相差悬殊，则长者（S2）的两翼可直接截除
      return median ( S1, lo1, n1, S2, lo2 + ( n2 - n1 - 1 ) / 2, n1 + 2 - ( n2 - n1 ) % 2 );
   ///////////////////////////////////////////////////////////////////////
   //    lo1                  lo1 + n1/2              lo1 + n1 - 1
   //     |                       |                       |
   //     X >>>>>>>>>>>>>>>>>>>>> X >>>>>>>>>>>>>>>>>>>>> X
   //                             |
   //                            m1
   ///////////////////////////////////////////////////////////////////////
   //                            mi2b
   //                             |
   // lo2 + n2 - 1         lo2 + n2 - 1 - n1/2
   //     |                       |
   //     Y <<<<<<<<<<<<<<<<<<<<< Y ...
   //                                .
   //                               .
   //                              .
   //                             .
   //                            .
   //                           .
   //                          .
   //                         ... Y <<<<<<<<<<<<<<<<<<<<< Y
   //                             |                       |
   //                       lo2 + (n1-1)/2               lo2
   //                             |
   //                            mi2a
   ///////////////////////////////////////////////////////////////////////
   int mi1  = lo1 + n1 / 2;
   int mi2a = lo2 + ( n1 - 1 ) / 2;
   int mi2b = lo2 + n2 - 1 - n1 / 2;
   if ( S1[mi1] > S2[mi2b] ) //取S1左半、S2右半
      return median ( S1, lo1, n1 / 2 + 1, S2, mi2a, n2 - ( n1 - 1 ) / 2 );
   else if ( S1[mi1] < S2[mi2a] ) //取S1右半、S2左半
      return median ( S1, mi1, ( n1 + 1 ) / 2, S2, lo2, n2 - n1 / 2 );
   else //S1保留，S2左右同时缩短
      return median ( S1, lo1, n1, S2, mi2a, n2 - ( n1 - 1 ) / 2 * 2 );
}

#endif