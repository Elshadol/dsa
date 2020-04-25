#include <time.h>
#include <stdlib.h>

#include "sort.h"
#include "sort_helper.h"

#define INSERTIONSORT_THRESHOLD  64

static int _partition(int a[], int lo, int hi)
{
	srand(time(NULL));
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

void quick_sort(int a[], int lo, int hi)
{
	if (hi - lo < 2)
		return;

	if (hi - lo < INSERTIONSORT_THRESHOLD) {
		insertion_sort(a, lo, hi);
		return;
	}

	int cut = _partition(a, lo, hi);

	quick_sort(a, lo, cut);
	quick_sort(a, cut + 1, hi);
}
