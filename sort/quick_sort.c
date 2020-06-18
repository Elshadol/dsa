#include "sort.h"
#include "sort_helper.h"

#define INSERTIONSORT_THRESHOLD  32
#define MEDIAN3_THRESHOLD 1024

static inline void _sort_condinate_pivots3(int a[], int e1,  int e2, int e3)
{
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
}

static void _sort_condinate_pivots5(int a[], int e1, int e2, int e3,
                                    int e4, int e5)
{
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
}

void quick_sort(int a[], int lo, int hi)
{
    int len = hi - lo;
    if (len < INSERTIONSORT_THRESHOLD) {
        insertion_sort(a, lo, hi);
        return;
    } else if (len < MEDIAN3_THRESHOLD) {
        int e1 = lo;
        int e2 = lo + (len >> 1);
        int e3 = hi - 1;
        _sort_condinate_pivots3(a, e1, e2, e3);
        swap(&a[lo], &a[e2]);
        goto HOARE_PARTITION;
    } else {
        int e1 = lo;
        int e2 = lo + (len >> 2);
        int e3 = lo + (len >> 1);
        int e4 = lo + (len >> 1) + (len >> 2);
        int e5 = hi - 1;
        _sort_condinate_pivots5(a, e1, e2, e3, e4, e5);
        swap(&a[lo], &a[e3]);
        if ((a[e1] != a[e2]) && (a[e2] != a[e3]) &&
            (a[e3] != a[e4]) && (a[e4] != a[e5]))
            goto HOARE_PARTITION;
        else
            goto BM_3WAY_PARTITION;
    }

HOARE_PARTITION: {
        int i = lo;
        int j = hi;
        int pivot = a[lo];
        for (;;) {
            do {
                ++i;
            } while (a[i] < pivot);
            do {
                --j;
            } while (pivot < a[j]);
            if (j < i)
                break;
            swap(&a[i], &a[j]);
        }
        a[lo] = a[j];
        a[j] = pivot;
        quick_sort(a, lo, j);
        quick_sort(a, j + 1, hi);
        return;
    }

BM_3WAY_PARTITION: {
        int eq1 = lo + 1;
        int lt = eq1;
        int eq2 = hi;
        int gt = eq2;
        int pivot = a[lo];

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

        quick_sort(a, lo, lo + (lt - eq1));
        quick_sort(a, hi - (eq2 - gt), hi);
    }
}
