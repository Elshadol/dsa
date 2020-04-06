#include <stdio.h>
#include <stdbool.h>

static inline void _swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}


void bubble_sort0(int a[], int lo, int hi)
{
	for (int i = hi; lo < i; --i) {
		for (int j = lo + 1; j < hi; ++j) {
			if (a[j - 1] > a[j])
				_swap(&a[j - 1], &a[j]);
		}
	}
}

void bubble_sort1(int a[], int lo, int hi)
{
	bool sorted = false;
	while (!sorted) {
		sorted = true;
		for (int i = lo; ++i < hi; ) {
			if (a[i - 1] > a[i]) {
				_swap(&a[i - 1], &a[i]);
				sorted = false;
			}
		}
		--hi;
	}
}

void bubble_sort2(int a[], int lo, int hi)
{
	while (lo < hi) {
		int last = lo;
		for (int i = lo; ++i < hi; ) {
			if (a[i - 1] < a[i]) {
				_swap(&a[i - 1], &a[i]);
				last = i;
			}
		}
		hi = last;
	}
}
