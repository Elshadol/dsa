#include "sort.h"
#include "sort_helper.h"

#include <stdbool.h>
#include <stdlib.h>

#define INSERTIONSORT_THRESHOLD  32
#define NINTHER_THRESHOLD 512

#define NUMS_OF_RUN_STACK 85

static int _run_begin[NUMS_OF_RUN_STACK];
static int _run_end[NUMS_OF_RUN_STACK];
static int _run_stack_size = 0;

static inline void _init_run_stack(void)
{
    _run_stack_size = 0;
}

static inline bool _full_run_stack(void)
{
    return _run_stack_size == NUMS_OF_RUN_STACK;
}

static inline bool _empty_run_stack(void)
{
    return _run_stack_size == 0;
}

static inline void _push_run_stack(int run_begin, int run_end)
{
    if (_full_run_stack())
        exit(1);
    _run_begin[_run_stack_size] = run_begin;
    _run_end[_run_stack_size] = run_end;
    ++_run_stack_size;
}

static inline void _pop_run_stack(void)
{
    if (_empty_run_stack())
        exit(1);
    --_run_stack_size;
}

static inline void _top_run_stack(int *run_begin, int *run_end)
{
    *run_begin = _run_begin[_run_stack_size - 1];
    *run_end = _run_end[_run_stack_size - 1];
}

static inline int _median3(const int a[], int e1, int e2, int e3)
{
    if (a[e1] < a[e2]) {
        if (a[e2] < a[e3])
            return e2;
        else if (a[e1] < a[e3])
            return e3;
        else
            return e1;
    } else {
        if (a[e1] < a[e3])
            return e1;
        else if (a[e2] < a[e3])
            return e3;
        else
            return e2;
    }
}

static inline int _ninther(const int a[], int lo, int hi)
{
    int e1 = lo;
    int e2 = lo + ((hi - lo) >> 1);
    int e3 = hi - 1;
    int eighth = (hi - lo) >> 3;
    e1 = _median3(a, e1, e1 + eighth, e1 + 2*eighth);
    e2 = _median3(a, e2 - eighth, e2, e2 + eighth);
    e3 = _median3(a, e3 - 2*eighth, e3 - eighth, e3);
    return _median3(a, e1, e2, e3);
}

static int _partition(int a[], int lo, int hi)
{
    int idx;
    if (hi - lo < NINTHER_THRESHOLD)
        idx = _median3(a, lo, (lo + hi) >> 1, hi - 1);
    else
        idx = _ninther(a, lo, hi);
    swap(&a[lo], &a[idx]);

    int i = lo, j = hi;
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
    return j;
}

void non_recur_quick_sort(int a[], int lo, int hi)
{
    if (hi - lo < 2)
        return;

    _init_run_stack();
    _push_run_stack(lo, hi);
    while (!_empty_run_stack()) {
        int run_begin;
        int run_end;
        _top_run_stack(&run_begin, &run_end);
        _pop_run_stack();
        if (run_end - run_begin < INSERTIONSORT_THRESHOLD) {
            insertion_sort(a, run_begin, run_end);
        } else {
            int cut = _partition(a, run_begin, run_end);
            if (run_end - cut > cut - run_begin) {
                if (run_end - cut > 2)
                    _push_run_stack(cut + 1, run_end);
                if (cut - run_begin > 1)
                    _push_run_stack(run_begin, cut);
            } else {
                if (cut - run_begin > 1)
                    _push_run_stack(run_begin, cut);
                if (run_end - cut > 2)
                    _push_run_stack(cut + 1, run_end);
            }
        }
    }
}
