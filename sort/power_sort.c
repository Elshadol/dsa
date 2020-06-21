#include "sort.h"
#include "sort_helper.h"

#include <stdlib.h>

#define MIN_MERGE 64

#define NUMS_OF_RUN_STACK 85
static int _run_begin[NUMS_OF_RUN_STACK];
static int _run_end[NUMS_OF_RUN_STACK];
static int _run_stack_size = 0;

static inline void _init_run_stack()
{
    for (int i = 0; i < NUMS_OF_RUN_STACK; ++i)
        _run_begin[i] = -1;
    _run_stack_size = 0;
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

// this method is the performance bootleneck of power_sort
static int _node_power_loop(int left, int right, int start_a,
                            int end_a, int end_b)
{
    int nt2 = (right - left) << 1;
    int len_a = end_a - start_a;
    int len_b = end_b - end_a;
    long a = (start_a << 1) + len_a - (left << 1);
    long b = (end_a << 1) + len_b - (left << 1);
    int k = 0;
    while ((a - b < nt2) && (a/nt2 == b/nt2)) {
        ++k;
        a <<= 1;
        b <<= 1;
    }
    return k;
}

/**
 * A naive implementation of power_sort which is found from
 * an article named 'Nearly-Optimal Mergesorts:
 * Fast, Pratical Sorting Methods That Optimally Adapt to Existing Runs'
 * by J.Ian Munro and Sebstian Wild.
 */
void power_sort(int a[], int lo, int hi)
{
    int len = hi - lo;
    if (len < 2)
        return;

    if (len < MIN_MERGE) {
        insertion_sort(a, lo, hi);
        return;
    }

    _init_run_stack();

    int *aux_array = (int *)malloc(sizeof(a[0]) *  len);

    int begin_a = lo;
    int end_a = _extent_hi(a, begin_a, hi);
    while (end_a < hi) {
        int begin_b = end_a;
        int end_b = _extent_hi(a, begin_b, hi);
        // need more efficient method to compute node power
        int k = _node_power_loop(lo, hi, begin_a, end_a, end_b);
        for (int i = _run_stack_size; i > k; --i) {
            if (_run_begin[i] == -1)
                continue;
            if (_run_end[i] - _run_begin[i] < end_a - _run_end[i])
                _merge_lo(a, _run_begin[i], _run_end[i], end_a, aux_array);
            else
                _merge_hi(a, _run_begin[i], _run_end[i], end_a, aux_array);
            begin_a = _run_begin[i];
            _run_begin[i] = -1;
        }
        _run_begin[k] = begin_a;
        _run_end[k] = end_a;
        _run_stack_size = k;
        begin_a = begin_b;
        end_a = end_b;
    }
    for (int i = _run_stack_size; i > 0; --i) {
        if (_run_begin[i] == -1)
            continue;
        if (_run_end[i] - _run_begin[i] < end_a - _run_end[i])
            _merge_lo(a, _run_begin[i], _run_end[i], hi, aux_array);
        else
            _merge_hi(a, _run_begin[i], _run_end[i], hi, aux_array);
    }
    free(aux_array);
}
