#include <stdlib.h>

#include "sort.h"
#include "sort_helper.h"

#define INSERTIONSORT_THRESHOLD  47

void quick_sort(int a[], int lo, int hi)
{
    int len = hi - lo;
    if (len < INSERTIONSORT_THRESHOLD) {
        insertion_sort(a, lo, hi);
        return;
    }

    if (len < 1000) {
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
        quick_sort(a, lo, j);
        quick_sort(a, j + 1, hi);
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

        if ((a[e1] != a[e2]) && (a[e2] != a[e3]) && 
            (a[e3] != a[e4]) && (a[e4] != a[e5])) {
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
            quick_sort(a, lo, j);
            quick_sort(a, j + 1, hi);
        } else {
            /*
             * assert there are many duplicated elems in array,
             * use Bentley-Mcilroy partition
             */
            int eq1 = lo + 1;
            int lt = eq1;
            int eq2 = hi;
            int gt = eq2;
            do {
                while ((lt < gt) && (a[lt] <= a[lo])) {
                    if (a[lt] == a[lo])
                        swap(&a[eq1++], &a[lt]);
                    ++lt;
                }
                while ((lt < gt) && (a[lo] <= a[gt - 1])) {
                    if (a[gt - 1] == a[lo])
                        swap(&a[--eq2], &a[gt - 1]);
                    --gt;
                }
                if (lt >= gt)
                    break;
                swap(&a[lt++], &a[--gt]);
            } while (1);

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
}
