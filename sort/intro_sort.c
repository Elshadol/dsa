#include <stdlib.h>
#include <assert.h>

#include "sort.h"
#include "sort_helper.h"

#define INSERTIONSORT_THRESHOLD 47

static inline int _depth_limit(int n)
{
    int k = 0;
    for ( ; n > 1; n >>= 1)
        ++k;

    return k;
}

static inline int _median3(const int a[], int e1, int e2, int e3)
{
    if (a[e1] < a[e2]) {
        if (a[e2] < a[e3])
            return e2;
        else if (a[e1] < a[e3])
            return e3;
        else
            return e1;
    } else {
        if (a[e1] < a[e3])
            return e1;
        else if (a[e2] < a[e3])
            return e3;
        else
            return e2;
    }
}

static inline int _ninther(const int a[], int lo, int hi)
{
    int e1 = lo;
    int e2 = lo + ((hi - lo) >> 1);
    int e3 = hi - 1;
    int eighth = (hi - lo) / 8;
    e1 = _median3(a, e1, e1 + eighth, e1 + 2*eighth);
    e2 = _median3(a, e2 - eighth, e2, e2 + eighth);
    e3 = _median3(a, e3 - 2*eighth, e3 - eighth, e3);
    return _median3(a, e1, e2, e3);
}

static int _partition(int a[], int lo, int hi)
{
    int idx;
    if (hi - lo < 1000)
        idx = _median3(a, lo, (lo + hi) >> 1, hi - 1);
    else
        idx = _ninther(a, lo, hi);
    swap(&a[lo], &a[idx]);

    int i = lo;
    int j = hi;
    do {
        do {
            ++i;
        } while ((i < hi) && (a[i] < a[lo]));
        do {
            --j;
        } while (a[lo] < a[j]);
        if (j < i)
            break;
        swap(&a[i], &a[j]);
    } while (1);
    swap(&a[lo], &a[j]);
    return j;
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

static void _final_insertion_sort(int a[], int lo, int hi)
{
    if (hi - lo < INSERTIONSORT_THRESHOLD) {
        insertion_sort(a, lo, hi);
    } else {
        insertion_sort(a, lo, INSERTIONSORT_THRESHOLD);
        pair_insertion_sort(a, INSERTIONSORT_THRESHOLD, hi);
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
