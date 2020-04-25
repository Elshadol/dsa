#include "sort.h"

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

void heap_sort(int a[], int lo, int hi)
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
