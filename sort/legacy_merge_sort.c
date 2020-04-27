#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "sort.h"
#include "sort_helper.h"

#define MIN_MERGE 128

static void _merge_lo(int a[], int base1, int len1,
					  int base2, int len2, int aux[])
{
	array_copy(a, base1, aux, 0, len1);

	int cursor1 = 0;
	int cursor2 = base2;
	int dest = base1;

	while (cursor1 < len1) {
		if ((cursor2 < base2 + len2) && (a[cursor2] < aux[cursor1]))
			a[dest++] = a[cursor2++];

		if ((base2 + len2 <= cursor2) || (aux[cursor1] <= a[cursor2]))
			a[dest++] = aux[cursor1++];
	}
}

static void _merge_hi(int a[], int base1, int len1,
							 int base2, int len2, int aux[])
{
	array_copy(a, base2, aux, 0, len2);

	int cursor1 = base1 + len1 - 1;
	int cursor2 = len2 - 1;
	int dest = base2 + len2 - 1;

	while (0 <= cursor2) {
		if ((base1 <= cursor1) && (a[cursor1] > aux[cursor2]))
			a[dest--] = a[cursor1--];

		if ((cursor1 < base1) || (aux[cursor2] >= a[cursor1]))
			a[dest--] = aux[cursor2--];
	}
}

static void _merge(int a[], int lo, int mi, int hi, int aux[])
{
	const int aux_len = mi - lo;
	array_copy(a, lo, aux, 0, aux_len);

	for (int i = lo, j = 0, k = mi; j < aux_len; ) {
		if ((k < hi) && (a[k] < aux[j]))
			a[i++] = a[k++];
		if ((hi <= k) || (aux[j] <= a[k]))
			a[i++] = aux[j++];
	}
}

static void _merge_sort(int a[], int lo, int hi, int aux[])
{
	if (hi - lo < MIN_MERGE) {
		insertion_sort(a, lo, hi);
		return;
	}

	int mi = lo + (hi - lo)/2;
	_merge_sort(a, lo, mi, aux);
	_merge_sort(a, mi, hi, aux);
	_merge_hi(a, lo, mi - lo, mi, hi - mi, aux);
}

static void _inplace_merge(int a[], int lo, int mi, int hi)
{
	if (a[mi - 1] < a[mi])
		return;

	int i = lo, j = mi;
	while ((i < j) && (j < hi)) {
		while ((i < j ) && (a[i] <= a[j]))
			++i;

		int idx = j;
		while ((j < hi) && (a[j] < a[i]))
			++j;

		rotate(a, i, idx, j);
		i += j - idx;
	}
}

static void _inplace_merge_sort(int a[], int lo, int hi)
{
	if (hi - lo < MIN_MERGE) {
		insertion_sort(a, lo, hi);
		return;
	}

	int mi = lo + (hi - lo)/2;
	_inplace_merge_sort(a, lo, mi);
	_inplace_merge_sort(a, mi, hi);
	_inplace_merge(a, lo, mi, hi);
}

void merge_sort(int a[], int lo, int hi)
{
	assert((0 <= lo) && (lo < hi));

	if (hi - lo < 2)
		return;

	if (hi - lo < MIN_MERGE) {
		insertion_sort(a, lo, hi);
		return;
	}

	int *aux = (int *)malloc(sizeof(a[0]) * (hi - lo) / 2);
	if (aux != NULL) {
		_merge_sort(a, lo, hi, aux);
		free(aux);
	}
	else {
		_inplace_merge_sort(a, lo, hi);
	}
}

static inline int _min_run_length(int n)
{
	int r = 0;
	while (n >= MIN_MERGE) {
		r |= (n & 1);
		n >>= 1;
	}
	return (n + r);
}

// non-recursive merge_sort
void merge_sort2(int a[], int lo, int hi)
{
	const int N = hi - lo;
	if (N < 2)
		return;

	int run_len = _min_run_length(N);

	int i = lo;
	for (; i + run_len <= hi; i += run_len)
		insertion_sort(a, i, i + run_len);

	if (i < hi)
		insertion_sort(a, i, hi);

	int *aux = (int *)malloc(sizeof(a[lo]) * N);
	if (aux != NULL) {
		for (int length = run_len; length < N; length *= 2) {
			for (i = lo; i + 2*length <= hi; i += 2*length)
				_merge(a, i, i + length, i + 2*length, aux);
			if (i + length < hi)
				_merge(a, i, i + length, hi, aux);
		}
		free(aux);
	} else {
		for (int length = run_len; length < N; length *= 2) {
			for (i = lo; i + 2*length <= hi; i += 2*length)
				_inplace_merge(a, i, i + length, i + 2*length);
			if (i + length < hi)
				_inplace_merge(a, i, i + length, hi);
		}
	}
}
