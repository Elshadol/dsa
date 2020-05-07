#include <stdlib.h>

#include "sort.h"
#include "sort_helper.h"

#define INSERTIONSORT_THRESHOLD  47

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

void quick_sort(int a[], int lo, int hi)
{
    if (hi - lo < 2)
        return;

    if (hi - lo < INSERTIONSORT_THRESHOLD) {
        insertion_sort(a, lo, hi);
        return;
    }

    int mi = _partition(a, lo, hi);
    quick_sort(a, lo, mi);
    quick_sort(a, mi + 1, hi);
}
