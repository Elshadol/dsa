#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static const int MIN_MERGE = 32;

static inline void insertionSort(int a[], int lo, int hi)
{
	for (int i = lo + 1; i < hi; ++i) {
		int j = i;
		int pivot = a[i];
		for (; (lo < j) && (pivot < a[j - 1]); --j)
			a[j] = a[j - 1];

		a[j] = pivot;
	}
}

/*
 * a[lo, mi) and a[mi, hi) are sorted
 * merge a[lo, mi) and a[mi, hi) to form a[lo, hi)
 */
static inline void _merge(int a[], int lo, int mi, int hi, int aux[])
{
	// copy a[lo, mi) to aux
	const int aux_len = mi - lo;
	for (int i = 0, j = lo; i < aux_len; ++i, ++j)
		aux[i] = a[j];

	for (int i = lo, j = 0, k = mi; j < aux_len; ) {
		if ((k < hi) && (a[k] < aux[j]))
			a[i++] = a[k++];
		if ((hi <= k) || (aux[j] <= a[k]))
			a[i++] = aux[j++];
	}
}

void _merge_sort(int a[], int lo, int hi, int aux[])
{
	if (hi - lo < MIN_MERGE) {
		insertionSort(a, lo, hi);
		return;
	}

	int mi = lo + (hi - lo)/2;
	_merge_sort(a, lo, mi, aux);
	_merge_sort(a, mi, hi, aux);
	_merge(a, lo, mi, hi, aux);
}

void mergeSort1(int a[], int lo, int hi)
{
	assert(lo < hi);

	if (hi - lo < 2)
		return;

	int *aux = (int *)malloc(sizeof(a[lo]) * (hi - lo) / 2);
	if (aux != NULL) {
		_merge_sort(a, lo, hi, aux);
		free(aux);
	}
#if 0
	else {
		_inplace_merge(a, lo, hi);
	}
#endif
}

void mergeSort2(int a[], int lo, int hi)
{
	assert(lo < hi);

	const int n = hi - lo;
	if (n < 2)
		return;

	if (n < MIN_MERGE) {
		insertionSort(a, lo, hi);
		return;
	}

	int *aux = (int *)malloc(sizeof(a[lo]) * n / 2);
	if (aux != NULL) {
		// sort a[lo, lo+MIN_MERGE), a[lo+MIN_MERGE, lo+2*MIN_MERGE), etc
		int i = lo;
		for (; i + MIN_MERGE <= hi; i += MIN_MERGE)
			insertionSort(a, i, i + MIN_MERGE);

		if (i < hi)
			insertionSort(a, i, hi);

		for (int length = MIN_MERGE; length < n; length *= 2) {
			// merge a[lo, lo+length) and a[lo+length, lo+2*length)
			for (i = lo; i + 2*length <= hi; i += 2*length)
				_merge(a, i, i + length, i + 2*length, aux);
			if (i + length < hi)
				_merge(a, i, i + length, hi, aux);
		}
		free(aux);
	}
#if 0
	else {
		_inplace_merge(a, lo, hi);
	}
#endif
}

static int a[2000];

int main(void) {
	for (int i = 0; i < 2000; ++i)
		a[i] = 2000- i;


	mergeSort1(a, 0, 2000);

	for (int i = 0; i < 2000; ++i) {
		printf("%d ", a[i]);
		if (i % 10 == 0)
			puts(" ");
	}
	return 0;
}
