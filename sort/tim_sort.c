#include <stdlib.h>

#include "sort_helper.h"
#include "sort.h"

#define MIN_MERGE 64

#define MIN_GALLOP 7
static int _min_gallop = MIN_GALLOP;

static int *_aux_array;
static int *__a;

#define RUN_STACK_SIZE 49
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

static int
_gallop_left(int key, const int a[], int base, int len, int hint)
{
    int last_ofs = 0;
    int ofs = 1;

    if (key > a[base + hint]) {
        int max_ofs = len - hint;
        while ((ofs < max_ofs) && (a[base + hint + ofs] < key)) {
            last_ofs = ofs;
            ofs = (ofs << 1) + 1;
            if (ofs <= 0)
                ofs = max_ofs;
        }
        if (ofs > max_ofs)
            ofs = max_ofs;

        last_ofs += hint;
        ofs += hint;
    } else {
        int max_ofs = hint + 1;
        while ((ofs < max_ofs) && (key <= a[base + hint - ofs])) {
            last_ofs = ofs;
            ofs = (ofs << 1) + 1;
            if (ofs <= 0)
                ofs = max_ofs;
        }
        if (ofs > max_ofs)
            ofs = max_ofs;

        int tmp = last_ofs;
        last_ofs = hint - ofs;
        ofs = hint - tmp;
    }

    ++last_ofs;
    while (last_ofs < ofs) {
        int m = last_ofs + ((ofs - last_ofs) >> 1);
        if (key > a[base + m])
            last_ofs = m + 1;
        else
            ofs = m;
    }
    return ofs;
}

static int
_gallop_right(int key, const int a[], int base, int len, int hint)
{
    int ofs = 1;
    int last_ofs = 0;

    if (key < a[base + hint]) {
        int max_ofs = hint + 1;
        while ((ofs < max_ofs) && (key < a[base + hint - ofs])) {
            last_ofs = ofs;
            ofs = (ofs << 1) + 1;
            if (ofs <= 0)
                ofs = max_ofs;
        }
        if (ofs > max_ofs)
            ofs = max_ofs;

        int tmp = last_ofs;
        last_ofs = hint - ofs;
        ofs = hint - tmp;
    } else {
        int max_ofs = len - hint;
        while ((ofs < max_ofs) && (key >= a[base + hint + ofs])) {
            last_ofs = ofs;
            ofs = (ofs << 1) + 1;
            if (ofs <= 0)
                ofs = max_ofs;
        }
        if (ofs > max_ofs)
            ofs = max_ofs;

        last_ofs += hint;
        ofs += hint;
    }

    ++last_ofs;
    while (last_ofs < ofs) {
        int m = last_ofs + ((ofs - last_ofs) >> 1);
        if (key < a[base + m])
            ofs = m;
        else
            last_ofs = m + 1;
    }
    return ofs;
}

static void
_merge_lo(int a[], int base1, int len1, int base2, int len2, int tmp[])
{
    array_copy(a, base1, tmp, 0, len1);

    int cursor1 = 0;
    int cursor2 = base2;
    int dest = base1;

    a[dest++] = a[cursor2++];
    --len2;
    if (len2 == 0) {
        array_copy(tmp, cursor1, a, dest, len1);
        return;
    }
    if (len1 == 1) {
        array_copy(a, cursor2, a, dest, len2);
        dest += len2;
        a[dest] = tmp[cursor1];
        return;
    }

    int min_gallop = _min_gallop;
    for (;;) {
        int count1 = 0, count2 = 0;
        do {
            if (a[cursor2] < tmp[cursor1]) {
                a[dest++] = a[cursor2++];
                count1 = 0;
                ++count2;
                --len2;
                if (len2 == 0) {
                    array_copy(tmp, cursor1, a, dest, len1);
                    return;
                }
            } else {
                a[dest++] = tmp[cursor1++];
                ++count1;
                count2 = 0;
                --len1;
                if (len1 == 1) {
                    array_copy(a, cursor2, a, dest, len2);
                    dest += len2;
                    a[dest] = tmp[cursor1];
                    return;
                }
            }
        } while ((count1 | count2) < min_gallop);

        ++min_gallop;
        do {
            if (min_gallop > 1)
                --min_gallop;
            _min_gallop = min_gallop;

            count1 = _gallop_right(a[cursor2], tmp, cursor1, len1, 0);
            if (count1 != 0) {
                array_copy(tmp, cursor1, a, dest, count1);
                dest += count1;
                cursor1 += count1;
                len1 -= count1;
                if (len1 == 1) {
                    array_copy(a, cursor2, a, dest, len2);
                    dest += len2;
                    a[dest] = a[cursor1];
                    return;
                }
            }
            a[dest++] = a[cursor2++];
            --len2;
            if (len2 == 0) {
                array_copy(tmp, cursor1, a, dest, len1);
                return;
            }

            count2 = _gallop_left(tmp[cursor1], a, cursor2, len2, 0);
            if (count2 != 0) {
                array_copy(a, cursor2, a, dest, count2);
                dest += count2;
                cursor2 += count2;
                len2 -= count2;
                if (len2 == 0) {
                    array_copy(tmp, cursor1, a, dest, len1);
                    return;
                }
            }
            a[dest++] = tmp[cursor1++];
            --len1;
            if (len1 == 1) {
                array_copy(a, cursor2, a, dest, len2);
                dest += len2;
                a[dest] = a[cursor1];
                return;
            }
        } while ((count1 >= MIN_GALLOP) || (count2 >= MIN_GALLOP));
        ++min_gallop;
        _min_gallop = min_gallop;
    }
}

static void
_merge_hi(int a[], int base1, int len1, int base2, int len2, int tmp[])
{
    array_copy(a, base2, tmp, 0, len2);

    int cursor1 = base1 + len1 - 1;
    int cursor2 = len2 - 1;
    int dest = base2 + len2 - 1;

    a[dest--] = a[cursor1--];
    --len1;
    if (len1 == 0) {
        array_copy(tmp, 0, a, dest - (len2 - 1), len2);
        return;
    }
    if (len2 == 1) {
        dest -= len1;
        cursor1 -= len1;
        array_copy(a, cursor1 + 1, a, dest + 1, len1);
        a[dest] = tmp[cursor2];
        return;
    }

    int min_gallop = _min_gallop;
    for (;;) {
        int count1 = 0, count2 = 0;
        do {
            if (tmp[cursor2] < a[cursor1]) {
                a[dest--] = a[cursor1--];
                --len1;
                ++count1;
                count2 = 0;
                if (len1 == 0) {
                    array_copy(tmp, 0, a, dest - (len2 - 1), len2);
                    return;
                }
            } else {
                a[dest--] = tmp[cursor2--];
                --len2;
                count1 = 0;
                ++count2;
                if (len2 == 1) {
                    dest -= len1;
                    cursor1 -= len1;
                    array_copy(a, cursor1 + 1, a, dest + 1, len1);
                    a[dest] = tmp[cursor2];
                    return;
                }
            }
        } while ((count1 | count2) < min_gallop);

        ++min_gallop;
        do {
            if (min_gallop > 1)
                --min_gallop;
            _min_gallop = min_gallop;

            count1 = len1 - _gallop_right(tmp[cursor2], a, base1, len1, len1 - 1);
            if (count1 != 0) {
                dest -= count1;
                cursor1 -= count1;
                len1 -= count1;
                array_copy(a, cursor1 + 1, a, dest + 1, count1);
                if (len1 == 0) {
                    array_copy(tmp, 0, a, dest - (len2 - 1), len2);
                    return;
                }
            }
            a[dest--] = tmp[cursor2--];
            --len2;
            if (len2 == 1) {
                dest -= len1;
                cursor1 -= len1;
                array_copy(a, cursor1 + 1, a, dest + 1, len1);
                a[dest] = tmp[cursor2];
                return;
            }

            count2 = len2 - _gallop_left(a[cursor1], tmp, 0, len2, len2 - 1);
            if (count2 != 0) {
                dest -= count2;
                cursor2 -= count2;
                len2 -= count2;
                array_copy(tmp, cursor2 + 1, a, dest + 1, count2);
                if (len2 == 1) {
                    dest -= len1;
                    cursor1 -= len1;
                    array_copy(a, cursor1 + 1, a, dest + 1, len1);
                    a[dest] = tmp[cursor2];
                    return;
                }
            }
            a[dest--] = a[cursor1--];
            --len1;
            if (len1 == 0) {
                array_copy(tmp, 0, a, dest - (len2 - 1), len2);
                return;
            }
        } while ((count1 >= MIN_GALLOP) || (count2 >= MIN_GALLOP));
        ++min_gallop;
        _min_gallop = min_gallop;
    }
}

static void _merge_at(int i)
{
    int base1 = _run_base[i];
    int len1 = _run_len[i];
    int base2 = _run_base[i + 1];
    int len2 = _run_len[i + 1];

    _run_len[i] = len1 + len2;
    if (i == _run_stack_size - 3) {
        _run_base[i + 1] = _run_base[i + 2];
        _run_len[i + 1] = _run_len[i + 2];
    }
    --_run_stack_size;

    int k = _gallop_right(__a[base2], __a, base1, len1, 0);
    base1 += k;
    len1 -= k;
    if (len1 == 0)
        return;

    len2 =  _gallop_left(__a[base1 + len1 - 1], __a, base2, len2, len2 - 1);
    if (len2 == 0)
        return;

    if (len1 <= len2)
        _merge_lo(__a, base1, len1, base2, len2, _aux_array);
    else
        _merge_hi(__a, base1, len1, base2, len2, _aux_array);
}

static void _merge_collapse(void)
{
    while (_run_stack_size > 1) {
        int n = _run_stack_size - 2;
        if (((0 < n) && (_run_len[n - 1] <= _run_len[n] + _run_len[n + 1])) ||
            ((1 < n) && (_run_len[n - 2] <= _run_len[n - 1] + _run_len[n]))) {
            if (_run_len[n - 1] < _run_len[n + 1])
                --n;
        } else if ((n < 0) || (_run_len[n] > _run_len[n + 1])) {
            break;
        }
        _merge_at(n);
    }
}

static void _merge_force_collapse(void)
{
    while (_run_stack_size > 1) {
        int n = _run_stack_size - 2;
        if ((0 < n) && (_run_len[n - 1] < _run_len[n + 1]))
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
void tim_sort(int a[], int lo, int hi)
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
    _aux_array = (int *)malloc(sizeof(a[0]) * n / 2);
    if (_aux_array == NULL)
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
