#include <stdlib.h>
#include <stdlib.h>
#include <assert.h>

#include "sort.h"
#include "sort_helper.h"

#define MEDIAN5_THRESHOLD 1000
#define INSERTIONSORT_THRESHOLD 47

static inline int _depth_limit(int n)
{
    int k = 0;
    for ( ; n > 1; n >>= 1)
        ++k;

    return k;
}

static int _partition(int a[], int lo, int hi)
{
    int len = hi - lo;

    if (len < MEDIAN5_THRESHOLD) {
        swap(&a[lo], &a[lo + (rand() % (hi - lo))]);
    } else {
        int seventh = (len >> 3) + (len >> 6) + (len >> 9) + 1;
        int e3 = lo + (len >> 1);
        int e2 = e3 - seventh;
        int e1 = e2 - seventh;
        int e4 = e3 + seventh;
        int e5 = e4 + seventh;

		if (a[e2] < a[e1]) {
			int t = a[e2]; a[e2] = a[e1]; a[e1] = t;
		}
        if (a[e3] < a[e2]) {
            int t = a[e3]; a[e3] = a[e2]; a[e2] = t;
            if (t < a[e1]) {
                a[e2] = a[e1]; a[e1] = t;
            }
        }
        if (a[e4] < a[e3]) {
            int t = a[e4]; a[e4] = a[e3]; a[e3] = t;
            if (t < a[e2]) {
                a[e3] = a[e2]; a[e2] = t;
                if (t < a[e1]) {
                    a[e2] = a[e1]; a[e1] = t;
                }
            }
        }
        if (a[e5] < a[e4]) {
            int t = a[e5]; a[e5] = a[e4]; a[e4] = t;
            if (t < a[e3]) {
                a[e4] = a[e3]; a[e3] = t;
                if (t < a[e2]) {
                    a[e3] = a[e2]; a[e2] = t;
                    if (t < a[e1]) {
                        a[e2] = a[e1]; a[e1] = t;
                    }
                }
            }
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

static void _introsort_loop(int a[], int lo, int hi, int depth_limit)
{
    while (hi - lo > INSERTIONSORT_THRESHOLD) {
        if (depth_limit == 0) {
            heap_sort(a, lo, hi);
            return;
        }

        --depth_limit;
        int mi = _partition(a, lo, hi);
        if (mi - lo > hi - mi) {
            _introsort_loop(a, lo, mi, depth_limit);
            lo =  mi + 1;
        } else {
            _introsort_loop(a, mi, hi, depth_limit);
            hi = mi;
        }
    }
}

static void _unguard_insertion_sort(int a[], int lo, int hi)
{
    for (int i = lo; i < hi; ++i) {
        int tmp = a[lo];

        int j = i;
        while (tmp < a[j - 1]) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = tmp;
    }
}

static void _final_insertion_sort(int a[], int lo, int hi)
{
    if (hi - lo < INSERTIONSORT_THRESHOLD) {
        insertion_sort(a, lo, hi);
    } else {
        insertion_sort(a, lo, INSERTIONSORT_THRESHOLD);
        _unguard_insertion_sort(a, INSERTIONSORT_THRESHOLD, hi);
    }
}

void intro_sort(int a[], int lo, int hi)
{
    assert((0 <= lo) && (lo < hi));

    if (hi - lo < 2)
        return;

    _introsort_loop(a, lo, hi, 2 * _depth_limit(hi - lo));
    _final_insertion_sort(a, lo, hi);
}
