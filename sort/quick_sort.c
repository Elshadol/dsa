#include <time.h>
#include <stdlib.h>


static const int QUICKSORT_MIN_RANGE = 32;

static inline void swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

#if 0
static inline int _median3(int a, int b, int c)
{
	if (a < b) {
		if (b < c)
			return b;
		else if (a < c)
			return c;
		else
			return a;
	} else {
		if (a < c)
			return a;
		else if (b < c)
			return c;
		else
			return b;
	}
}
#endif


// a[lo, hi)
static int _partition(int a[], int lo, int hi)
{
	swap(&a[lo], &a[lo + (rand() % (hi - lo))]);

	int pivot = a[lo];
	int mi = lo;

	// loop invariant: a[lo + 1, mi) < pivot
	for (int k = lo + 1; k < hi; ++k) {
		if (a[k] < pivot)
			swap(&a[++mi], &a[k]);
	}

	swap(&a[mi], &a[lo]);
	return mi;
}

static inline void insertion_sort(int a[], int lo, int hi)
{
	for (int i = lo + 1; i < hi; ++i) {
		int pivot = a[i];

		int j = i;
		for (; (lo < j) && (pivot < a[j - 1]); --j)
			a[j] = a[j - 1];

		a[j] = pivot;
	}
}

void quick_sort(int a[], int lo, int hi)
{
	if (hi - lo < 2)
		return;

	if (hi - lo < QUICKSORT_MIN_RANGE) {
		insertion_sort(a, lo, hi);
		return;
	}

	int cut = _partition(a, lo, hi);

	quick_sort(a, lo, cut);
	quick_sort(a, cut + 1, hi);
}
