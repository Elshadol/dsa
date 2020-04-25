#include <time.h>
#include <stdlib.h>
#include <assert.h>

#include "sort_helper.h"
#include "sort.h"

#define INSERTIONSORT_THRESHOLD 64

static int _partition(int a[], int lo, int hi)
{
	swap(&a[lo], &a[lo + (rand() % (hi - lo))]);

	int pivot = a[lo];
	int mi = lo;

	for (int k = lo + 1; k < hi; ++k) {
		if (a[k] < pivot)
			swap(&a[++mi], &a[k]);
	}

	swap(&a[mi], &a[lo]);
	return mi;
}

static inline int _depth_limit(int n)
{
	int k = 0;
	for ( ; n > 1; n >>= 1)
		++k;

	return k;
}

static void _introsort_loop(int a[], int lo, int hi, int depth_limit)
{
	while (hi - lo > INSERTIONSORT_THRESHOLD) {
		if (depth_limit == 0) {
			heap_sort(a, lo, hi);
			return;
		}

		--depth_limit;
		int cut = _partition(a, lo, hi);
		if (cut - lo > hi - cut) {
			_introsort_loop(a, lo, cut, depth_limit);
			lo =  cut + 1;
		} else {
			_introsort_loop(a, cut, hi, depth_limit);
			hi = cut;
		}
	}
	insertion_sort(a, lo, hi);
}


void intro_sort(int a[], int lo, int hi)
{
	assert((0 <= lo) && (lo < hi));

	if (hi - lo < 2)
		return;

	_introsort_loop(a, lo, hi, _depth_limit(hi - lo));
}
