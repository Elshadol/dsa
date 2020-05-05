#include <time.h>
#include <stdlib.h>
#include <assert.h>

#include "sort.h"
#include "sort_helper.h"

#define INSERTIONSORT_THRESHOLD  64

static int _partition(int a[], int lo, int hi)
{
    int len = hi - lo;

    if (len < 1000) {
        swap(&a[lo], &a[lo + (rand() % (hi - lo))]);
    } else {
        int seventh = (len >> 3) + (len >> 6) + (len >> 9) + 1;
        int e3 = lo + (len >> 1);
        int e2 = e3 - seventh;
        int e1 = e2 - seventh;
        int e4 = e3 + seventh;
        int e5 = e4 + seventh;

        int tmp = a[e2];
        if (tmp < a[e1]) {
            a[e2] = a[e1];
            a[e1] = tmp;
        }

        tmp = a[e3];
        if (tmp < a[e1]) {
            a[e3] = a[e2];
            a[e2] = a[e1];
            a[e1] = tmp;
        } else if (tmp < a[e2]) {
            a[e3] = a[e2];
            a[e2] = tmp;
        }

        tmp = a[e4];
        if (tmp < a[e1]) {
            a[e4] = a[e3];
            a[e3] = a[e2];
            a[e2] = a[e1];
            a[e1] = tmp;
        } else if (tmp < a[e2]) {
            a[e4] = a[e3];
            a[e3] = a[e2];
            a[e2] = tmp;
        } else if (tmp < a[e3]) {
            a[e4] = a[e3];
            a[e3] = tmp;
        }

        tmp = a[e5];
        if (tmp < a[e1]) {
            a[e5] = a[e4];
            a[e4] = a[e3];
            a[e3] = a[e2];
            a[e2] = a[e1];
            a[e1] = tmp;
        } else if (tmp < a[e2]) {
            a[e5] = a[e4];
            a[e4] = a[e3];
            a[e3] = a[e2];
            a[e2] = tmp;
        } else if (tmp < a[e3]) {
            a[e5] = a[e4];
            a[e4] = a[e3];
            a[e3] = tmp;
        } else if (tmp < a[e4]) {
            a[e5] = a[e4];
            a[e4] = tmp;
        }
        swap(&a[lo], &a[e3]);
    }

    int pivot = a[lo];
    int mi = lo;
    for (int i = lo + 1; i < hi; ++i) {
        if (a[i] < pivot)
            swap(&a[++mi], &a[i]);
    }
    swap(&a[lo], &a[mi]);
    return mi;
}

static void _quick_sort(int a[], int lo, int hi)
{
    if (hi - lo < INSERTIONSORT_THRESHOLD) {
        insertion_sort(a, lo, hi);
        return;
    }

    int mi = _partition(a, lo, hi);
    _quick_sort(a, lo, mi);
    _quick_sort(a, mi + 1, hi);
}

inline void quick_sort(int a[], int lo, int hi)
{
    assert((0 <= lo) && (lo < hi));

    if (hi - lo < 2)
        return;

    srand(time(NULL));
    _quick_sort(a, lo, hi);
}

int run_lo[100];
int run_hi[100];
int stack_size = 0;

void quick_sort1(int a[], int lo, int hi)
{
	if (hi - lo < INSERTIONSORT_THRESHOLD) {
		insertion_sort(a, lo, hi);
		return;
	}

	run_lo[stack_size] = lo;
	run_hi[stack_size] = hi;
	++stack_size;

	while (stack_size != 0) {
		int lo = run_lo[stack_size];
		int hi = run_hi[stack_size];
		--stack_size;

		if (hi - lo > INSERTIONSORT_THRESHOLD) {
			int mi = _partition(a, lo, hi);
			if (mi - lo < INSERTIONSORT_THRESHOLD) {
				insertion_sort(a, lo, mi);
			} else {
				run_lo[stack_size] = lo;
				run_hi[stack_size] = mi;
				++stack_size;
			}

			if (hi - mi < INSERTIONSORT_THRESHOLD) {
				insertion_sort(a, mi, hi);
			} else {
				run_lo[stack_size] = mi + 1;
				run_hi[stack_size] = hi;
				++stack_size;
			}
		} else {
			insertion_sort(a, lo, hi);
			continue;
		}
	}
}
