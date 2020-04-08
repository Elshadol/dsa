#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

static const int MIN_MERGE = 32;

static struct {
#define STACK_SIZE 66
	int run_base[STACK_SIZE];
	int run_len[STACK_SIZE];
	int stack_size;
} run_stack;

static struct {
	int *b;
	int *aux;
} merge_stat;

static inline void __init_merge_stat(int *a, int N)
{
	merge_stat.b = a;
	merge_stat.aux = (int *)malloc(sizeof(a[0]) * N);
}

// functions of stack operation
static inline void __init_run_stack(void)
{
	run_stack.stack_size = 0;
}

static inline void __pop_stack(int *rb, int *rl)
{
	--run_stack.stack_size;
	*rb = run_stack.run_base[run_stack.stack_size];
	*rl = run_stack.run_len[run_stack.stack_size];
}

static inline void __push_run_stack(int run_base, int run_len)
{
	run_stack.run_base[run_stack.stack_size] = run_base;
	run_stack.run_len[run_stack.stack_size] = run_len;
	++run_stack.stack_size;
}
// end of stack operation

// beginning of insertion_sort()
static inline void insertion_sort(int a[], int lo, int hi)
{
	for (int i = lo + 1; i < hi; ++i) {
		int pivot = a[i];

		int j = i;
		for (; (j > lo) && (a[j - 1] > pivot); --j)
			a[j] = a[j - 1];

		a[j] = pivot;
	}
}

static void binary_insertion_sort(int a[], int lo, int hi, int start)
{
	if (start == lo) {
		start++;
	}

	for (; start < hi; ++start) {
		int pivot = a[start];

		int left = lo;
		int right = start;

		while (left < right) {
			int mid = left + (right - left) / 2;
			if (pivot < a[mid])
				right = mid;
			else
				left = mid + 1;
		}

		int n = start - left;
		switch (n) {
		case 2:
			a[left + 2] = a[left + 1];
		case 1:
			a[left + 1] = a[left];
			break;
		default:
			while (n-- > 0) {
				a[start] = a[start - 1];
				--start;
			}
			break;
		}
		a[left] = pivot;
	}
}
// end of insertion_sort()

// functions for makeing runs
static inline void _reverse_range(int a[], int lo, int hi)
{
	while (lo < --hi) {
		int t = a[hi];
		a[hi] = a[lo];
		a[lo] = t;
		++lo;
	}
}

static inline int _compute_min_run_length(int n)
{
	int r = 0;

	while (n >= MIN_MERGE) {
		r |= (n & 1);
		n >>= 1;
	}

	return (n + r);
}

static int _count_run_and_make_ascending(int a[], int lo, int hi)
{
	if (lo + 1 == hi)
		return 1;

	int i = lo + 1;
	if (a[i++] < a[lo]) {
		while ((i < hi) && (a[i] < a[i - 1]))
			++i;
		_reverse_range(a, lo, i);
	} else {
		while ((i < hi) && (a[i] >= a[i - 1]))
			++i;
	}

	return i - lo;
}
// end of functions of making runs

// functions for merge
static inline void _merge(int a[], int lo, int mi, int hi, int aux[])
{
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

static inline void _merge_at(int i)
{
	assert(run_stack.stack_size >= 2);
	assert(i >= 0);
	assert((i == run_stack.stack_size - 2) || (i == run_stack.stack_size - 3));

	int base1 = run_stack.run_base[i];
	int len1 = run_stack.run_len[i];
	int base2 = run_stack.run_base[i + 1];
	int len2 = run_stack.run_len[i + 1];

	assert((0 < len1) || (0 < len2));
	assert(base1 + len1 == base2);

	run_stack.run_len[i] = len1 + len2;
	if (i == run_stack.stack_size - 3) {
		run_stack.run_base[i + 1] = run_stack.run_base[i + 2];
		run_stack.run_len[i + 1] = run_stack.run_len[i + 2];
	}
	--run_stack.stack_size;

	_merge(merge_stat.b, base1, base2, base2 + len2, merge_stat.aux);
}

static void _merge_collapse()
{
	while (run_stack.stack_size > 1) {
		int n = run_stack.stack_size - 2;

		if (((0 < n) && (run_stack.run_len[n - 1] <= run_stack.run_len[n] + run_stack.run_len[n + 1])) ||
			((1 < n) && (run_stack.run_len[n - 2] <= run_stack.run_len[n - 1] + run_stack.run_len[n]))) {
			if (run_stack.run_len[n - 1] < run_stack.run_len[n + 1])
				--n;
		} else if ((n < 0) || (run_stack.run_len[n + 1] < run_stack.run_len[n])) {
			break;
		}

		_merge_at(n);
	}
}

static void _merge_force_collapse(void)
{
	while (run_stack.stack_size > 1) {
		int n = run_stack.stack_size - 2;

		if ((0 < n) && (run_stack.run_len[n - 1] < run_stack.run_len[n + 1]))
			--n;
		_merge_at(n);
	}
}
// end of functions for merge

void tim_sort(int a[], int lo, int hi)
{
	int n = hi - lo;
	if (n < 2)
		return;

	if (n < MIN_MERGE) {
		insertion_sort(a, lo, hi);
		return;
	}

	__init_merge_stat(a, n);
	__init_run_stack();

	int min_run_len = _compute_min_run_length(n);

	do {
		int run_len = _count_run_and_make_ascending(a, lo, hi);

		if (run_len < min_run_len) {
			int force = n < min_run_len ? n : min_run_len;
			binary_insertion_sort(a, lo, lo + force, lo + run_len);
			run_len = force;
		}

		__push_run_stack(lo, run_len);
		_merge_collapse();

		lo += run_len;
		n -= run_len;
	} while (n != 0);

	assert(lo == hi);
	_merge_force_collapse();
	assert(run_stack.stack_size == 1);
}
