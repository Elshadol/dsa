#include <stdlib.h>

#include "sort_helper.h"
#include "sort.h"

#define MIN_MERGE 64

static int *__aux_array;
static int *__a;

#define RUN_STACK_SIZE 49
static int __run_base[RUN_STACK_SIZE];
static int __run_len[RUN_STACK_SIZE];
static int __run_stack_size;

static inline void _init_run_stack(void)
{
    __run_stack_size = 0;
}

static inline void _push_run_stack(int rb, int rl)
{
    __run_base[__run_stack_size] = rb;
    __run_len[__run_stack_size] = rl;
    ++__run_stack_size;
}

static inline int _min_run_length(int n)
{
    int r = 0;
    while (n >= MIN_MERGE) {
        r |= (n & 1);
        n >>= 1;
    }
    return (n + r);
}

static int _count_run_and_make_ascending(int a[], int lo, int hi)
{
    int i = lo + 1;
    if (i == hi)
        return 1;

    if (a[i++] < a[lo]) {
        while ((i < hi) && (a[i] < a[i - 1]))
            ++i;
        reverse_range(a, lo, i);
    } else {
        while ((i < hi) && (a[i] >= a[i - 1]))
            ++i;
    }

    return (i - lo);
}

static void _merge_lo(int a[], int lo, int mi, int hi, int aux[])
{
    int aux_len = mi - lo;
    array_copy(a, lo, aux, 0, aux_len);

    int cursor1 = 0;
    int cursor2 = mi;
    int dest = lo;

    while ((cursor1 < aux_len) && (cursor2 < hi)) {
        if (aux[cursor1] <= a[cursor2])
            a[dest++] = aux[cursor1++];
        else
            a[dest++] = a[cursor2++];
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

static void _merge_at(int i)
{
    int base1 = __run_base[i];
    int len1 = __run_len[i];
    int base2 = __run_base[i + 1];
    int len2 = __run_len[i + 1];
    __run_len[i] = len1 + len2;

    if (i == __run_stack_size - 3) {
        __run_base[i + 1] = __run_base[i + 2];
        __run_len[i + 1] = __run_len[i + 2];
    }
    --__run_stack_size;

    if (len1 < len2)
        _merge_lo(__a, base1, base2, base2 + len2, __aux_array);
    else
        _merge_hi(__a, base1, base2, base2 + len2, __aux_array);
}

static void _merge_collapse(void)
{
    while (__run_stack_size > 1) {
        int n = __run_stack_size - 2;
        if (((0 < n) && (__run_len[n - 1] <= __run_len[n] + __run_len[n + 1])) ||
            ((1 < n) && (__run_len[n - 2] <= __run_len[n - 1] + __run_len[n]))) {
            if (__run_len[n - 1] < __run_len[n + 1])
                --n;
        } else if ((n < 0) || (__run_len[n] > __run_len[n + 1])) {
            break;
        }
        _merge_at(n);
    }
}

static void _merge_force_collapse(void)
{
    while (__run_stack_size > 1) {
        int n = __run_stack_size - 2;
        if ((0 < n) && (__run_len[n - 1] < __run_len[n + 1]))
            --n;
        _merge_at(n);
    }
}

static void _insertion_sort(int a[], int begin, int end, int start)
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

/**
 * tiny_tim_sort is stolen from tim_sort and galloping mode is removed.
 * In fact , tiny_tim_sort is a stack-base merge sort.
 * Let X, Y, Z be the topmost 3 runs, run stack of tim_sort must
 * satisfy the following two principles:
 *      1. Y > Z
 *      2. X > Y + Z
 * Thus, run stack size of tim_sort grows slower than Fibonacci sequence.
 */
void tiny_tim_sort(int a[], int lo, int hi)
{
    int n = hi - lo;
    if (n < 2)
        return;

    if (n < MIN_MERGE) {
        insertion_sort(a, lo, hi);
        return;
    }

    _init_run_stack();
    __a = a;
    __aux_array = (int *)malloc(sizeof(a[0]) * n / 2);
    if (__aux_array == NULL)
        exit(1);

    int min_run_len = _min_run_length(n);
    do {
        int run_len = _count_run_and_make_ascending(a, lo, hi);
        if (run_len < min_run_len) {
            int force = n < min_run_len ? n : min_run_len;
            _insertion_sort(a, lo, lo + force, lo + run_len);
            run_len = force;
        }
        _push_run_stack(lo, run_len);
        _merge_collapse();

        lo += run_len;
        n -= run_len;
    } while (n != 0);

    _merge_force_collapse();
}
