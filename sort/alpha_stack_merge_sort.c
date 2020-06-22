#include <stdlib.h>

#include "sort_helper.h"
#include "sort.h"

#define MIN_MERGE 64

static int *_aux_array;
static int *__a;

/**
 * todo: find  more rigorous strategy to initialize _alpha
 * and dynamiclly change its value during running
 */
static const double _alpha = 1.5;

#define RUN_STACK_SIZE 85
static int _run_base[RUN_STACK_SIZE];
static int _run_len[RUN_STACK_SIZE];
static int _run_stack_size;

static inline void _init_run_stack(void)
{
    _run_stack_size = 0;
}

static inline void _push_run_stack(int rb, int rl)
{
    _run_base[_run_stack_size] = rb;
    _run_len[_run_stack_size] = rl;
    ++_run_stack_size;
}

static int _compute_min_run_length(int n)
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

static inline void _merge_at(int i)
{
    int base1 = _run_base[i];
    int len1 = _run_len[i];
    int base2 = _run_base[i + 1];
    int len2 = _run_len[i + 1];
    _run_len[i] = len1 + len2;
    --_run_stack_size;

    if (len1 < len2)
        _merge_lo(__a, base1, base2, base2 + len2, _aux_array);
    else
        _merge_hi(__a, base1, base2, base2 + len2, _aux_array);
}

static inline void _merge_collapse(void)
{
    while (_run_stack_size > 1) {
        int n = _run_stack_size - 2;
        if (_run_len[n] < (_alpha * _run_len[n + 1]))
            _merge_at(n);
        else
            break;
    }
}

static inline void _merge_force_collapse(void)
{
    while (_run_stack_size > 1) {
        int n = _run_stack_size - 2;
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
 * This kind of stack-base merge sort uses 'alpha-stack' strategy,
 * which consists in one rule: X > alpha * Y(X and Y are the topmost
 * 2 runs in run stack, alpha should bigger than 1)
 */
void alpha_stack_merge_sort(int a[], int lo, int hi)
{
    int len = hi - lo;
    if (len < 2)
        return;

    if (len < MIN_MERGE) {
        _insertion_sort(a, lo, hi, lo);
        return;
    }

    _init_run_stack();
    __a = a;
    _aux_array = (int *)malloc(sizeof(a[0]) * len / 2);
    if (_aux_array == NULL)
        exit(1);

    const int min_run_len = _compute_min_run_length(len);
    do {
        int run_len = _count_run_and_make_ascending(a, lo, hi);
        if (run_len < min_run_len) {
            int force = len < min_run_len ? len : min_run_len;
            _insertion_sort(a, lo, lo + force, lo + run_len);
            run_len = force;
        }

        _push_run_stack(lo, run_len);
        _merge_collapse();

        lo += run_len;
        len -= run_len;
    } while (len != 0);

    _merge_force_collapse();
}
