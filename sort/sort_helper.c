#include <stdlib.h>

#include "sort_helper.h"

inline int disordered(const int a[], int lo, int hi)
{
	int cnt = 0;
	for (int i = lo + 1; i < hi; ++i)
		if (a[i - 1] > a[i])
			++cnt;

	return cnt;
}

inline void swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

inline void array_copy(int src[], int corsur1, int dest[],
							  int corsur2, int len)
{
	do {
		dest[corsur2++] = src[corsur1++];
	} while (0 < --len);
}

inline void reverse_range(int a[], int lo, int hi)
{
	while (lo < --hi) {
		int t = a[hi];
		a[hi] = a[lo];
		a[lo] = t;
		++lo;
	}
}

inline void rotate(int a[], int lo, int mi, int hi)
{
	reverse_range(a, lo, mi);
	reverse_range(a, mi, hi);
	reverse_range(a, lo, hi);
}
