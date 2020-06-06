#include "sort.h"
#include "sort_helper.h"

#define INSERTIONSORT_THRESHOLD  32

void quick_sort(int a[], int lo, int hi)
{
    int len = hi - lo;
    if (len < INSERTIONSORT_THRESHOLD) {
        insertion_sort(a, lo, hi);
        return;
    } else {
        int e1 = lo;
        int e2 = lo + (len >> 1);
        int e3 = hi - 1;
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
        swap(&a[lo], &a[e2]);
        int pivot = a[lo];
        int i = lo, j = hi;
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
    }
}
