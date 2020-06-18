#include "sort.h"

/* heap_sort is stolen from www.azillionmonkeys.com/qed/sorttest.c,
 * partial_sort is stolen from SGI STL
 */

#define PARENT_OF(c) ((c - 1) >> 1)
#define RIGHT_CHILD_OF(p) ((p + 1) << 1)

static inline void _push_heap(int a[], int lo, int hole_idx,
                              int top_idx, int value)
{
    int parent = PARENT_OF(hole_idx);
    while ((hole_idx > top_idx) && (a[lo + parent] < value)) {
        a[lo + hole_idx] = a[lo + parent];
        hole_idx = parent;
        parent = PARENT_OF(hole_idx);
    }
    a[lo + hole_idx] = value;
}

static inline void _adjust_heap(int a[], int lo, int hole_idx,
                                int len, int value)
{
    int top_idx = hole_idx;
    int right_child = RIGHT_CHILD_OF(hole_idx);

    while (right_child < len) {
        if (a[lo + right_child] < a[lo + right_child - 1])
            --right_child;

        a[lo + hole_idx] = a[lo + right_child];
        hole_idx = right_child;
        right_child = RIGHT_CHILD_OF(hole_idx);
    }
    if (right_child == len) {
        --right_child;
        a[lo + hole_idx] = a[lo + right_child];
        hole_idx = right_child;
    }
    _push_heap(a, lo, hole_idx, top_idx, value);
}

static inline void _pop_heap(int a[], int lo, int hi,
                             int result, int value)
{
    a[result] = a[lo];
    _adjust_heap(a, lo, 0, hi - lo, value);
}

static inline void _heapificate(int a[], int lo, int hi)
{
    int len = hi - lo;
    for (int p = PARENT_OF(len - 1); 0 <= p; --p)
        _adjust_heap(a, lo, p, len, a[lo + p]);
}

void heap_sort(int a[], int lo, int hi)
{
    if (hi - lo < 2)
        return;

    _heapificate(a, lo + 1, hi);
    while (hi - lo > 1) {
        int tmp = a[lo];
        if (tmp < a[lo + 1]) {
            tmp = a[lo + 1];
            _adjust_heap(a, lo + 1, 0, hi - lo - 1, a[0]);
        } 
        --hi;
        a[0] = a[hi];
        a[hi] = tmp;
    }
}

static inline void _sort_heap(int a[], int lo, int hi)
{
    while (hi - lo > 1) {
        _pop_heap(a, lo, hi - 1, hi - 1, a[hi - 1]);
        --hi;
    }
}

void partial_sort(int a[], int lo, int mi, int hi)
{
    _heapificate(a, lo, mi);
    for (int i = mi; i < hi; ++i) {
        if (a[i] < a[lo])
            _pop_heap(a, lo, mi, i, a[i]);
    }
    _sort_heap(a, lo, mi);
}

/*
 *  a fun heap_sort implementation:
    void heap_sort(int a[], int lo, int hi)
    {
      int mi = lo + ((hi - lo) >> 1);
      _heapificate(a, lo, mi);
      _heapificate(a, mi, hi);

      while (hi - mi > 0) {
        if (a[lo] > a[mi])
          _pop_heap(a, lo, mi, hi - 1, a[hi - 1]);
        else
          _pop_heap(a, mi, hi - 1, hi - 1, a[hi - 1]);
        --hi;
      }
      _sort_heap(a, lo, mi);
    }

    traditional heap sort implementation:
    void heap_sort1(int a[], int lo, int hi)
    {
      _heapificate(a, lo, hi);
      while (hi - lo > 1) {
        _pop_heap(a, lo, hi - 1, hi - 1, a[hi - 1]);
        --hi;
      }
    }
*/
