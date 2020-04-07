#include <time.h>
#include <stdlib.h>

static const int QUICKSORT_MIN_RANGE = 16;

// beginning of heap_sort()
#define PARENT_OF(i) (((i) - 1) / 2)
#define LEFT_CHILD_OF(p) ((p) * 2 + 1)
#define RIGHT_CHILD_OF(p) ((p) * 2 + 2)

static void _percolate_down(int a[], int N, int p)
{
	int temp = a[p];
	int c = LEFT_CHILD_OF(p);

	while (c < N) {
		if ((c + 1 < N) && (a[c] < a[c + 1]))
			++c;

		if (a[c] < temp)
			break;

		a[p] = a[c];
		p = c;
		c = LEFT_CHILD_OF(p);
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

	// no right child, only have left one
	if (c == N) {
		a[p] = a[c - 1];
		p = c - 1;
	}

	return p;
}

/* regard a[0, last) as a max-heap, and insert val to it */
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

static inline int _depth_limit(int n)
{
	int k = 0;
	for (; n > 1; n >>= 1)
		++k;

	return k;
}

// beginning of __final_insertion_sort()
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

static void _binary_insertion_sort(int a[], int lo, int hi, int start)
{
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

static inline void _final_insertion_sort(int a[], int lo, int hi)
{
	if (hi - lo > QUICKSORT_MIN_RANGE) {
		_insertion_sort(a, lo, QUICKSORT_MIN_RANGE);
		_binary_insertion_sort(a, lo, hi, QUICKSORT_MIN_RANGE);
	} else
		_insertion_sort(a, lo, hi);
}
// end of __final_insertion_sort

static void _introsort_loop(int a[], int lo, int hi, int depth_limit)
{
	while (hi - lo > QUICKSORT_MIN_RANGE) {
		if (depth_limit == 0) {
			heap_sort(a, lo, hi);
			return;
		}

		--depth_limit;
		int mi = _partition(a, lo, hi);
		if (mi - lo > hi - mi) {
			_introsort_loop(a, lo, mi, depth_limit);
			lo = mi;
		} else {
			_introsort_loop(a, mi + 1, hi, depth_limit);
			hi = mi + 1;
		}
	}
}


// ported from SGI STL sort()
void intro_sort(int a[], int lo, int hi)
{
	if (hi - lo < 2)
		return;

	_introsort_loop(a, lo, hi, _depth_limit(hi - lo));
	_final_insertion_sort(a, lo, hi);
}
