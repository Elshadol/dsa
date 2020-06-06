#include "sort.h"

static inline void _sift_down(int a[], int top, int last)
{
    int tmp = a[top];
    int max_idx = top;
    while (last >= (max_idx += max_idx)) {
        if ((max_idx != last) && (a[max_idx] < a[max_idx + 1]))
            ++max_idx;

        if (tmp >= a[max_idx])
            break;

        a[top] = a[max_idx];
        top = max_idx;
    }
}

static inline void _sift_down0(int a[], int last)
{
    int tmp = a[0];
    if (a[0] < a[1]) {
        tmp = a[1];
        a[1] = a[0];
        _sift_down(a, 1, last);
    }
    a[0] = a[last];
    a[last] = tmp;
}

static inline void _heap_sort(int a[], int n)
{
    int i = n >> 1;
    --n;
    for ( ; 0 < i; --i)
        _sift_down(a, i, n);

    for (i = n; 0 <= i; --i)
        _sift_down0(a, i);
}

void heap_sort(int a[], int lo, int hi)
{
    if (hi - lo < 2)
        return;

    _heap_sort(a + lo, hi - lo);
}
