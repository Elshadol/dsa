#include <assert.h>
#include <stdio.h>

#include "sort.h"
#include "sort_helper.h"

#define INSERTION_SORT_THRESHOLD 47

void dual_pivot_quick_sort(int x[], int lo, int hi)
{
    int length = hi - lo;

    if (length < 2)
        return;

    if (length < INSERTION_SORT_THRESHOLD) {
        insertion_sort(x, lo, hi);
        return;
    }

    int seventh = (length >> 3) + (length >> 6) + (length >> 9) + 1;

    int e3 = lo + (length >> 1);
    int e2 = e3 - seventh;
    int e1 = e2 - seventh;
    int e4 = e3 + seventh;
    int e5 = e4 + seventh;

    if (x[e2] < x[e1]) {
        int t = x[e2];
        x[e2] = x[e1];
        x[e1] = t;
    }
    if (x[e3] < x[e2]) {
        int t = x[e3];
        x[e3] = x[e2];
        x[e2] = t;
        if (t < x[e1]) {
            x[e2] = x[e1];
            x[e1] = t;
        }
    }
    if (x[e4] < x[e3]) {
        int t = x[e4];
        x[e4] = x[e3];
        x[e3] = t;
        if (t < x[e2]) {
            x[e3] = x[e2];
            x[e2] = t;
            if (t < x[e1]) {
                x[e2] = x[e1];
                x[e1] = t;
            }
        }
    }
    if (x[e5] < x[e4]) {
        int t = x[e5];
        x[e5] = x[e4];
        x[e4] = t;
        if (t < x[e3]) {
            x[e4] = x[e3];
            x[e3] = t;
            if (t < x[e2]) {
                x[e3] = x[e2];
                x[e2] = t;
                if (t < x[e1]) {
                    x[e2] = x[e1];
                    x[e1] = t;
                }
            }
        }
    }

    if ((x[e1] != x[e2]) && (x[e2] != x[e3]) &&
        (x[e3] != x[e4]) && (x[e4] != x[e5])) {
        swap(&x[lo], &x[e2]);
        int pivot1 = x[lo];

        swap(&x[hi - 1],  &x[e4]);
        int pivot2 = x[hi - 1];

        int lt = lo + 1;
        while (x[lt] < pivot1)
            ++lt;

        int gt = hi - 1;
        while (x[gt - 1] > pivot2)
            --gt;

        for (int i = lt; i < gt; ++i) {
            if (x[i] < pivot1) {
                swap(&x[i], &x[lt++]);
            } else if (x[i] > pivot2) {
                while ((i < --gt) && (x[gt] > pivot2));
                swap(&x[i], &x[gt]);
                if (x[i] < pivot1)
                    swap(&x[i], &x[lt++]);
            }
        }
        swap(&x[lo], &x[--lt]);
        swap(&x[hi - 1], &x[gt++]);
        dual_pivot_quick_sort(x, lo, lt);
        dual_pivot_quick_sort(x, gt, hi);

        if ((lt < e1) && (gt < e2)) {
            while (x[lt] == pivot1) ++lt;

            while (x[gt - 1] == pivot2) --gt;

            for (int i = lt; i < gt; ++i) {
                if (x[i] == pivot1) {
                    swap(&x[i], &x[lt++]);
                } else if (x[i] == pivot2) {
                    while ((i < --gt) && (x[gt] == pivot2));
                    swap(&x[i], &x[gt]);
                    if (x[i] == pivot1)
                        swap(&x[i], &x[lt++]);
                }
            }
        }
        dual_pivot_quick_sort(x, lt, gt);
    } else {
        swap(&x[lo],  &x[e3]);
        int pivot = x[lo];

        int a = lo + 1;
        int b = a;
        int d = hi;
        int c = d;

        for ( ; ; ) {
            while ((b < c) && (x[b] <= pivot)) {
                if (x[b] == pivot)
                    swap(&x[a++], &x[b]);
                ++b;
            }
            while ((b < c) && (pivot <= x[c - 1])) {
                if (x[c - 1] == pivot)
                    swap(&x[--d], &x[c - 1]);
                --c;
            }
            if (b >= c) break;
            swap(&x[b++], &x[--c]);
        }

        int lan1 = a - lo < b - a ? a  - lo : b - a;
        for (int cursor1 = lo, cursor2 = b - lan1; lan1 > 0; --lan1)
            swap(&x[cursor1++], &x[cursor2++]);

        int lan2 = d - c < hi - d ? d - c : hi - d;
        for (int cursor1 = c, cursor2 = hi - lan2; lan2 > 0; --lan2)
            swap(&x[cursor1++], &x[cursor2++]);

        dual_pivot_quick_sort(x, lo, lo + (b - a));
        dual_pivot_quick_sort(x, hi - (d - c), hi);
    }
}
