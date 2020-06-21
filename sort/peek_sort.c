#include <stdlib.h>

#include "sort_helper.h"
#include "sort.h"

#define MIN_MERGE 64

/* A naive implementation of peek_sort which is found from
 * an article named 'Nearly-Optimal Mergesorts:
 * Fast, Pratical Sorting Methods That Optimally Adapt to Existing Runs'
 * by J.Ian Munro and Sebstian Wild.
 */

static inline void _insertion_sort(int a[], int begin, int end, int start)
{
    if (start == begin)
        ++start;

    int i, j;
    for (i = start; i < end; ++i) {
        int tmp = a[i];
        if (tmp < a[begin]) {
            for (j = i; begin < j; --j)
                a[j] = a[j - 1];
        } else {
            for (j = i; tmp < a[j - 1]; --j)
                a[j] = a[j - 1];
        }
        a[j] = tmp;
    }
}

/* Functions begin with '_r' mean these function deal with their data
 * in reverse direction becuse of efficiency */
static inline void _rinsertion_sort(int a[], int rbegin, int rend, int rstart)
{
    if (rstart == rbegin)
        --rstart;

    int i, j;
    for (i = rstart; rend < i; --i) {
        int tmp = a[i];
        if (tmp > a[rbegin]) {
            for (j = i; j < rbegin; ++j)
                a[j] = a[j + 1];
        } else {
            for (j = i; tmp > a[j + 1]; ++j)
                a[j] = a[j + 1];
        }
        a[j] = tmp;
    }
}

static int _count_run_and_make_ascending(int a[], int begin, int end)
{
    int i = begin + 1;
    if (i == end)
        return 1;

    if (a[i++] < a[begin]) {
        while ((i < end) && (a[i] < a[i - 1]))
            ++i;
        reverse_range(a, begin, i);
    } else {
        while ((i < end) && (a[i] >= a[i - 1]))
            ++i;
    }
    return i - begin;
}

static int _rcount_run_and_make_ascending(int a[], int rbegin, int rend)
{
    int i = rbegin - 1;
    if (i == rend)
        return 1;

    if (a[i--] > a[rbegin]) {
        while ((rend < i) && (a[i] > a[i + 1]))
            --i;
        reverse_range(a, i + 1, rbegin + 1);
    } else {
        while ((rend < i) && (a[i] <= a[i + 1]))
            --i;
    }
    return rbegin - i;
}

static inline int _extent_lo(int a[], int rbegin, int rend)
{
    int len = rbegin - rend;
    int n = _rcount_run_and_make_ascending(a, rbegin, rend);
    if ((n < MIN_MERGE) && (n < len)) {
        int force = len < MIN_MERGE ? len : MIN_MERGE;
        _rinsertion_sort(a, rbegin, rbegin - force, rbegin - n);
        return rbegin - force + 1;
    }
    return rbegin - n + 1;
}

static inline int _extent_hi(int a[], int begin, int end)
{
    int len = begin - end;
    int n = _count_run_and_make_ascending(a, begin, end);
    if ((n < MIN_MERGE) && (n < len)) {
        int force = len < MIN_MERGE ? len : MIN_MERGE;
        _insertion_sort(a, begin, begin + force, begin + n);
        return begin + force;
    }
    return begin + n;
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

/* for space Optimal */
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

/*
 * array is divided into 3 parts: * [lsb, lse), [lse, hsb), [hsb, hse).
 * [lsb, lse) and [hsb, hse) are sorted
 */
static void _peek_sort(int a[], int lsb, int lse, int hsb, int hse, int aux[])
{
    if ((lse == hse) || (hsb == lsb))
        return;

    if (hse - lsb < MIN_MERGE) {
        _insertion_sort(a, lsb, hse, lse);
        return;
    }

    int mi = lsb + ((hse - lsb) >> 1);
    if (mi < lse) {
        _peek_sort(a, lse, lse, hsb, hse, aux);
        _merge_hi(a, lsb, lse, hse, aux);
    } else if (hsb <= mi) {
        _peek_sort(a, lsb, lse, hsb, hsb, aux);
        _merge_lo(a, lsb, hsb, hse, aux);
    } else {
        int i = _extent_lo(a, mi - 1, lse - 1);
        int j = _extent_hi(a, mi, hsb);
        if (mi - i < j - mi) {
            _merge_lo(a, i, mi, j, aux);
            _peek_sort(a, lsb, lse, i, i, aux);
            _peek_sort(a, i, j, hsb, hse, aux);
            _merge_lo(a, lsb, i, hse, aux);
        } else {
            _merge_hi(a, i, mi, j, aux);
            _peek_sort(a, j, j, hsb, hse, aux);
            _peek_sort(a, lsb, lse, i, j, aux);
            _merge_hi(a, lsb, j, hse, aux);
        }
    }
}

void peek_sort(int a[], int lo, int hi)
{
    int len = hi - lo;
    if (len < 2) {
        return;
    } else if (len < MIN_MERGE) {
        insertion_sort(a, lo, hi);
        return;
    } else {
        int *aux = (int *)malloc(sizeof(a[0]) * len / 2);
        _peek_sort(a, lo, lo, hi, hi, aux);
        free(aux);
    }
}
