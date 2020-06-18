#include <stdlib.h>

#include "sort.h"
#include "sort_helper.h"

#define MIN_MERGE 64

static inline int _min_run_length(int n)
{
    int r = 0;
    while (n >= MIN_MERGE) {
        r |= (n & 1);
        n >>= 1;
    }
    return (n + r);
}

static void _merge_lo(int a[], int lo, int mi, int hi, int aux[])
{
    int aux_len = mi - lo;
    array_copy(a, lo, aux, 0, aux_len);

    int cursor1 = 0;
    int cursor2 = mi;
    int dest = lo;

    while ((cursor1 < aux_len) && (cursor2 < hi)) {
        if (aux[cursor1] <= a[cursor2]) {
            a[dest] = aux[cursor1];
            ++cursor1;
        } else {
            a[dest] = a[cursor2];
            ++cursor2;
        }
        ++dest;
    }
    if (cursor1 < aux_len)
        array_copy(aux, cursor1, a, dest, aux_len - cursor1);
}

static void _merge_hi(int a[], int lo, int mi, int hi, int aux[])
{
    int aux_len = hi - mi;
    array_copy(a, mi, aux, 0, aux_len);

    int cursor1 = aux_len - 1;
    int cursor2 = mi - 1;
    int dest = hi - 1;

    while ((0 <= cursor1) && (lo <= cursor2)) {
        if (aux[cursor1] < a[cursor2]) {
            a[dest] = a[cursor2];
            --cursor2;
        } else {
            a[dest] = aux[cursor1];
            --cursor1;
        }
        --dest;
    }
    if (cursor1 >= 0)
        array_copy(aux, 0, a, dest - cursor1, cursor1 + 1);
}

void non_recur_merge_sort(int a[], int lo, int hi)
{
    int N = hi - lo;
    if (N < 2)
        return;

    int run_len = _min_run_length(N);

    int i = lo;
    for (; i + run_len <= hi; i += run_len)
        insertion_sort(a, i, i + run_len);
    if (i < hi)
        insertion_sort(a, i, hi);

    int *aux = (int *)malloc(sizeof(a[lo]) * N / 2);
    if (aux != NULL) {
        for (int length = run_len; length < N; length *= 2) {
            for (i = lo; i + 2*length <= hi; i += 2*length)
                _merge_lo(a, i, i + length, i + 2*length, aux);
            if (i + length < hi)
                _merge_hi(a, i, i + length, hi, aux);
        }
        free(aux);
    } else {
        exit(1);
    }
}
