#include "sort_helper.h"
#include "sort.h"

#include <stdlib.h>

#define MIN_MERGE 64

#define MIN_GALLOP 7
static int _min_gallop = MIN_GALLOP;

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

static void _galloping_merge_sort(int a[], int lo, int hi, int aux[])
{
    if (hi - lo < MIN_MERGE) {
        insertion_sort(a, lo, hi);
        return;
    }

    int mi = lo + ((hi - lo) >> 1);
    _galloping_merge_sort(a, lo, mi, aux);
    _galloping_merge_sort(a, mi, hi, aux);

    int base1 = lo;
    int len1 = mi - lo;
    int base2 = mi;
    int len2 = hi - mi;

    int k = _gallop_right(a[base2], a, base1, len1, 0);
    base1 += k;
    len1 -= k;
    if (len1 == 0)
        return;

    len2 = _gallop_left(a[base1 + len1 - 1], a, base2, len2, len2 - 1);
    if (len2 == 0)
        return;

    if (len1 < len2)
        _merge_lo(a, base1, len1, base2, len2, aux);
    else
        _merge_hi(a, base1, len1, base2, len2, aux);
}

void galloping_merge_sort(int a[], int lo, int hi)
{
    if (hi - lo < 2)
        return;

    int *aux = (int *)malloc(sizeof(a[0]) * (hi - lo) / 2);
    if (aux != NULL) {
        _galloping_merge_sort(a, lo, hi, aux);
        free(aux);
    } else
        exit(1);
}
