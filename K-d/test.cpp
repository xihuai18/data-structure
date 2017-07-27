#include "Twod_tree.h"

int data[20][2] = {
	{1,2}, {3,4}, {1,3}, {4,5}, {123,34}, {523,325}, {3425,345}, {4325,23}, {42,234}, {243,234}, 
	{7,23}, {678,345}, {8,3}, {8,68}, {46,8}, {576,867}, {567,8}, {678,567}, {867,567}, {687,456}
};

int main()
{
	TwoDT<int> kd;
	for(int i = 0; i < 20; ++i)
	{
		kd.insert(data[i]);
	}
	int lo[2] = {4,9}, hi[2] = {900,888};
	kd.printRangeSearch(lo, hi);
	system("pause");
	return 0;
}