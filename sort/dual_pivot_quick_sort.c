#include "sort.h"
#include "sort_helper.h"

#include <stdlib.h>

#define INSERTION_SORT_THRESHOLD 32
#define QUICK_SORT_THRESHOLD 1024

void dual_pivot_quick_sort(int a[], int lo, int hi)
{
    int length = hi - lo;
    if (length < INSERTION_SORT_THRESHOLD) {
        insertion_sort(a, lo, hi);
        return;
    } else if (length < QUICK_SORT_THRESHOLD) {
        int idx = lo + rand() % length;
        swap(&a[lo], &a[idx]);
        int pivot = a[lo];
        int i = lo, j = hi;
        for (;;) {
            do {
                ++i;
            } while ((i < j) && (a[i] < pivot));
            do {
                --j;
            } while (pivot < a[j]);
            if (j < i)
                break;
            swap(&a[i], &a[j]);
        }
        a[lo] = a[j];
        a[j] = pivot;
        dual_pivot_quick_sort(a, lo, j);
        dual_pivot_quick_sort(a, j + 1, hi);
    } else {
        int seventh = (length >> 3) + (length >> 6) + 1;
        int e3 = lo + (length >> 1);
        int e2 = e3 - seventh;
        int e1 = e2 - seventh;
        int e4 = e3 + seventh;
        int e5 = e4 + seventh;

        if (a[e2] < a[e1]) {
            int t = a[e2];
            a[e2] = a[e1];
            a[e1] = t;
        }
        if (a[e3] < a[e2]) {
            int t = a[e3];
            a[e3] = a[e2];
            a[e2] = t;
            if (t < a[e1]) {
                a[e2] = a[e1];
                a[e1] = t;
            }
        }
        if (a[e4] < a[e3]) {
            int t = a[e4];
            a[e4] = a[e3];
            a[e3] = t;
            if (t < a[e2]) {
                a[e3] = a[e2];
                a[e2] = t;
                if (t < a[e1]) {
                    a[e2] = a[e1];
                    a[e1] = t;
                }
            }
        }
        if (a[e5] < a[e4]) {
            int t = a[e5];
            a[e5] = a[e4];
            a[e4] = t;
            if (t < a[e3]) {
                a[e4] = a[e3];
                a[e3] = t;
                if (t < a[e2]) {
                    a[e3] = a[e2];
                    a[e2] = t;
                    if (t < a[e1]) {
                        a[e2] = a[e1];
                        a[e1] = t;
                    }
                }
            }
        }

        if ((a[e1] != a[e2]) && (a[e2] != a[e3]) &&
            (a[e3] != a[e4]) && (a[e4] != a[e5])) {
            swap(&a[lo], &a[e2]);
            int pivot1 = a[lo];

            swap(&a[hi - 1],  &a[e4]);
            int pivot2 = a[hi - 1];

            int lt = lo;
            while (a[++lt] < pivot1);

            int gt = hi;
            while (a[--gt] > pivot2);

            for (int i = lt; i < gt; ++i) {
                if (a[i] < pivot1) {
                    swap(&a[i], &a[lt++]);
                } else if (a[i] > pivot2) {
                    while ((i < --gt) && (a[gt] > pivot2));
                    swap(&a[i], &a[gt]);
                    if (a[i] < pivot1)
                        swap(&a[i], &a[lt++]);
                }
            }
            a[lo] = a[lt - 1];
            a[lt - 1] = pivot1;
            a[hi - 1] = a[gt];
            a[gt] = pivot2;
            dual_pivot_quick_sort(a, lo, lt - 1);
            dual_pivot_quick_sort(a, lt, gt);
            dual_pivot_quick_sort(a, gt + 1, hi);
        } else {
            swap(&a[lo],  &a[e3]);
            int pivot = a[lo];

            int eq1 = lo + 1;
            int lt = eq1;
            int eq2 = hi;
            int gt = eq2;

            for (;;) {
                while ((lt < gt) && (a[lt] <= pivot)) {
                    if (a[lt] == pivot)
                        swap(&a[eq1++], &a[lt]);
                    ++lt;
                }
                while ((lt < gt) && (pivot <= a[gt - 1])) {
                    if (a[gt - 1] == pivot)
                        swap(&a[--eq2], &a[gt - 1]);
                    --gt;
                }
                if (lt >= gt)
                    break;
                swap(&a[lt++], &a[--gt]);
            }

            int len1 = eq1 - lo < lt - eq1 ? eq1 - lo : lt - eq1;
            for (int c1 = lo, c2 = lt - len1; 0 < len1; --len1)
                swap(&a[c1++], &a[c2++]);

            int len2 = eq2 - gt < hi - eq2 ? eq2 - gt : hi - eq2;
            for (int c1 = gt, c2 = hi - len2; 0 < len2; --len2)
                swap(&a[c1++], &a[c2++]);

            dual_pivot_quick_sort(a, lo, lo + (lt - eq1));
            dual_pivot_quick_sort(a, hi - (eq2 - gt), hi);
        }
    }
}
