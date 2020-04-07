#include <stdlib.h>

static const int MIN_MERGE = 32;

static inline int _min_run_length(int n)
{
	int r = 0;
	while (n >= MIN_MERGE) {
		r |= (n & 1);
		n >>= 1;
	}

	return (n + r);
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

/*
 * a[lo, mi) and a[mi, hi) are sorted,
 * merge a[lo, mi) and a[mi, hi) to a[lo, hi)
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

static inline void _reverse(int a[], int lo, int hi)
{
	while (lo < --hi) {
		int t = a[hi];
		a[hi] = a[lo];
		a[lo] = t;
		++lo;
	}
}

static inline void _rotate(int a[], int lo, int mi, int hi)
{
	_reverse(a, lo, mi);
	_reverse(a, mi, hi);
	_reverse(a, lo, hi);
}

static void _inplace_merge(int a[], int lo, int mi, int hi)
{
	while ((lo < mi) && (mi < hi)) {
		while ((lo < mi) && (a[lo] < a[mi]))
			++lo;

		int idx = mi;
		while ((mi < hi) && (a[mi] < a[lo]))
			++mi;

		if (lo != idx)
			_rotate(a, lo, idx, mi);
		lo += mi -idx;
	}
}

void _merge_sort(int a[], int lo, int hi, int aux[])
{
	if (hi - lo < MIN_MERGE) {
		insertion_sort(a, lo, hi);
		return;
	}

	int mi = lo + (hi - lo)/2;
	_merge_sort(a, lo, mi, aux);
	_merge_sort(a, mi, hi, aux);
	_merge(a, lo, mi, hi, aux);
}

void _inplace_merge_sort(int a[], int lo, int hi)
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

void merge_sort1(int a[], int lo, int hi)
{

	if (hi - lo < 2)
		return;

	int *aux = (int *)malloc(sizeof(a[lo]) * (hi - lo) / 2);
	if (aux != NULL) {
		_merge_sort(a, lo, hi, aux);
		free(aux);
	}
	else {
		_inplace_merge_sort(a, lo, hi);
	}
}

void merge_sort2(int a[], int lo, int hi)
{
	const int N = hi - lo;
	if (N < 2)
		return;

	int run_len = _min_run_length(N);

	int *aux = (int *)malloc(sizeof(a[lo]) * N);
		// sort a[lo, lo+run_len), a[lo+run_len, lo+2*run_len), etc
	int i = lo;
	for (; i + run_len <= hi; i += run_len)
		insertion_sort(a, i, i + run_len);

	if (i < hi)
		insertion_sort(a, i, hi);

	if (aux != NULL) {
		for (int length = run_len; length < N; length *= 2) {
			// merge a[lo, lo+length) and a[lo+length, lo+2*length)
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
