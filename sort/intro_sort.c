#include <time.h>
#include <stdlib.h>

#define INSERTIONSORT_THRESHOLD 16

// beginning of heap_sort()
#define PARENT_OF(i) (((i) - 1) / 2)
#define RIGHT_CHILD_OF(p) ((p) * 2 + 2)

static void _percolate_down(int a[], int N, int p)
{
	int temp = a[p];
	int c = RIGHT_CHILD_OF(p);

	for (; c < N; p = c, c = RIGHT_CHILD_OF(p)) {
		if (a[c] < a[c - 1])
			--c;

		if (a[c] < temp)
			break;

		a[p] = a[c];
	}

	if (c == N) {
		--c;
		if (temp < a[c]) {
			a[p] = a[c];
			p = c;
		}
	}

	a[p] = temp;
}

static inline void _heapificate(int a[], int N)
{
	for (int i = PARENT_OF(N - 1); 0 <= i; --i) {
		_percolate_down(a, N, i);
	}
}

static inline int _adjust_after_pop_heap(int a[], int N)
{
	int p = 0;
	int c = RIGHT_CHILD_OF(p);

	while (c < N) {
		if (a[c] < a[c - 1])
			--c;

		a[p] = a[c];
		p = c;
		c = RIGHT_CHILD_OF(p);
	}

	if (c == N) {
		a[p] = a[c - 1];
		p = c - 1;
	}

	return p;
}

static inline void _push_heap(int a[], int last, int val)
{
	int i = last;

	while ((0 < i) && (a[PARENT_OF(i)] < val)) {
		a[i] = a[PARENT_OF(i)];
		i = PARENT_OF(i);
	}

	a[i]  = val;
}

static void heap_sort(int a[], int lo, int hi)
{
	if (hi - lo < 2)
		return;

	int *b = a + lo;
	const int N = hi - lo;

	_heapificate(b, N);

	for (int i = N; 0 < i; --i) {
		int temp = b[0];

		int last = _adjust_after_pop_heap(b, i);
		int right = b[i - 1];
		_push_heap(b, last, right);

		b[i - 1] = temp;
	}
}
// end of heap_sort()

static inline void swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

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
	for (; n > 1; n >>= 1)
		++k;

	return k;
}

static inline void _insertion_sort(int a[], int lo, int hi)
{
	for (int i = lo + 1; i < hi; ++i) {
		int pivot = a[i];

		int j = i;
		for (; (j > lo) && (a[j - 1] > pivot); --j)
			a[j] = a[j - 1];

		a[j] = pivot;
	}
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
	_insertion_sort(a, lo, hi);
}


void intro_sort(int a[], int lo, int hi)
{
	if (hi - lo < 2)
		return;

	_introsort_loop(a, lo, hi, _depth_limit(hi - lo));
}
