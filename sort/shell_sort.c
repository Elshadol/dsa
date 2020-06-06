#include "sort.h"

void shell_sort(int a[], int lo, int hi)
{
    int h = 1;

    while (h < (hi - lo)/3)
        h = h*3 + 1;

    for (; h > 0; h /= 3) {
        for (int i = lo + h; i < hi; i++) {
            int pivot = a[i];
            int j = i;
            for (; (lo + h <= j) && (pivot < a[j - h]); j -= h)
                a[j] = a[j - h];

            a[j] = pivot;
        }
    }
}
void shell_sort(int a[], int lo, int hi)
{
    int h = 1;
    while (h < (hi - lo)/3)
        h = h*3 + 1;

    for (; h > 0; h /= 3) {
        for (int i = 0; i < h; ++i) {
            for (int j = lo + i + h; j < hi; j += h) {
                int pivot = a[i];
                int k = j;
                if (pivot < a[lo + i]) {
                    while (lo + i + h <= k) {
                        a[k] = a[k - h];
                        k -= h;
                    }
                } else {
                    while (pivot < a[k - h]) {
                        a[k] = a[k - h];
                        k -= h;
                    }
                }
                a[k] = pivot;
            }
        }
    }
}
