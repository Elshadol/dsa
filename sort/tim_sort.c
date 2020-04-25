#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "sort_helper.h"
#include "sort.h"

#define MIN_MERGE 128

#define MIN_GALLOP 7
static int _min_gallop = MIN_GALLOP;

static int *_aux_array;
static int *__a;

static struct {
#define STACK_SIZE 49
	int run_base[STACK_SIZE];
	int run_len[STACK_SIZE];
	int stack_size;
} _run_stack;

static inline void _init_run_stack(void)
{
	_run_stack.stack_size = 0;
}

static inline void _push_run_stack(int rb, int rl)
{
	_run_stack.run_base[_run_stack.stack_size] = rb;
	_run_stack.run_len[_run_stack.stack_size] = rl;
	++_run_stack.stack_size;
}

static inline int _min_run_length(int n)
{
	assert(0 < n);

	int r = 0;
	while (n >= MIN_MERGE) {
		r |= (n & 1);
		n >>= 1;
	}
	return (n + r);
}

static int _count_run_and_make_ascending(int a[], int lo, int hi)
{
	assert(lo < hi);

	if (lo + 1 == hi)
		return 1;

	int i = lo + 1;
	if (a[i++] < a[lo]) {
		while ((i < hi) && (a[i] < a[i - 1]))
			++i;
		reverse_range(a, lo, i);
	} else {
		while ((i < hi) && (a[i] >= a[i - 1]))
			++i;
	}

	return i - lo;
}

static int _gallop_left(int key, const int a[], int base, int len, int hint)
{
	assert((len > 0) && (hint >= 0) && (hint < len));
	int last_ofs = 0;
	int ofs = 1;
	if (key > a[base + hint]) {
		int max_ofs = len - hint;
		while ((ofs < max_ofs) && (key > a[base + hint + ofs])) {
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
	assert((-1 <= last_ofs) && (last_ofs < ofs) && (ofs <= len));

	++last_ofs;
	while (last_ofs < ofs) {
		int m = last_ofs + (ofs - last_ofs) / 2;

		if (key > a[base + m])
			last_ofs = m + 1;
		else
			ofs = m;
	}
	assert(last_ofs == ofs);
	return ofs;
}

static int _gallop_right(int key, const int a[], int base, int len, int hint) {
	assert((len > 0) && (hint >= 0) && (hint < len));

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
	assert((-1 <= last_ofs) && (last_ofs < ofs) && (ofs <= len));

	++last_ofs;
	while (last_ofs < ofs) {
		int m = last_ofs + (ofs - last_ofs) / 2;

		if (key < a[base + m])
			ofs = m;
		else
			last_ofs = m + 1;
	}
	assert(last_ofs == ofs);
	return ofs;
}

static void _merge_lo(int a[], int base1, int len1, int base2, int len2, int tmp[])
{
	assert((0 < len1) && (0 < len2) && (base1 + len1 == base2));

	array_copy(a, base1, tmp, 0, len1);

	int cursor1 = 0;
	int cursor2 = base2;
	int dest = base1;

	a[dest++] = a[cursor2++];
	if (--len2 == 0) {
		array_copy(tmp, cursor1, a, dest, len1);
		return;
	}

	if (len1 == 1) {
		array_copy(a, cursor2, a, dest, len2);
		a[dest + len2] = tmp[cursor1];
		return;
	}

	int min_gallop = _min_gallop;

	while (1) {
		int count1 = 0;
		int count2 = 0;

		do {
			assert((1 < len1) && (0 < len2));
			if (a[cursor2] < tmp[cursor1]) {
				a[dest++] = a[cursor2++];
				count1 = 0;
				++count2;

				if (--len2 == 0) {
					goto finnishing;
				}
			} else {
				a[dest++] = tmp[cursor1++];
				count1++;
				count2 = 0;

				if (--len1 == 1) {
					goto finnishing;
				}
			}
		} while ((count1 < min_gallop) && (count2 < min_gallop));

		do {
			assert((1 < len1) && (0 < len2));

			count1 = _gallop_right(a[cursor2], tmp, cursor1, len1, 0);
			if (count1 != 0) {
				array_copy(tmp, cursor1, a, dest, count1);
				dest += count1;
				cursor1 += count1;
				len1 -= count1;
				if (len1 <= 1) {
					goto finnishing;
				}
			}

			a[dest++] = a[cursor2++];
			if (--len2 == 0) {
				goto finnishing;
			}

			count2 = _gallop_left(tmp[cursor1], a, cursor2, len2, 0);
			if (count2 != 0) {
				array_copy(a, cursor2, a, dest, count2);
				dest += count2;
				cursor2 += count2;
				len2 -= count2;
				if (len2 == 0) {
					goto finnishing;
				}
			}

			a[dest++] = tmp[cursor1++];
			if (--len1 == 1) {
				goto finnishing;
			}
			--min_gallop;
		} while ((count1 >= MIN_GALLOP) | (count2 >= MIN_GALLOP));

		if (min_gallop < 0)
			min_gallop = 0;

		min_gallop += 2;
	}

finnishing:
	_min_gallop = (min_gallop < 1) ? 1 : min_gallop;

	if (len1 == 1) {
		assert(0 < len2);
		array_copy(a, cursor2, a, dest, len2);
		a[dest + len2] = tmp[cursor1];
	} else if (len1 == 0) {
		exit(1);
	} else {
		assert(1 < len1);
		assert(0 == len2);
		array_copy(tmp, cursor1, a, dest, len1);
	}
}

static void _merge_hi(int a[], int base1, int len1, int base2, int len2, int tmp[])
{
	assert((0 < len1) && (0 < len2) && (base1 + len1 == base2));

	array_copy(a, base2, tmp, 0, len2);

	int cursor1 = base1 + (len1 - 1);
	int cursor2 = len2 - 1;
	int dest = base2 + (len2 - 1);

	a[dest--] = a[cursor1--];
	if (--len1 == 0) {
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

	while (1) {
		int count1 = 0;
		int count2 = 0;

		do {
			assert((0 < len1) && (1 < len2));
			if (tmp[cursor2] < a[cursor1]) {
				a[dest--] = a[cursor1--];
				++count1;
				count2 = 0;

				if (--len1 == 0) {
					goto finnishing;
				}
			} else {
				a[dest--] = tmp[cursor2--];
				count1 = 0;
				++count2;

				if (--len2 == 1) {
					goto finnishing;
				}
			}
		} while ((count1 < min_gallop) && (count2 < min_gallop));

		do {
			assert((0 < len1) && (1 < len2));

			count1 = len1 - _gallop_right(tmp[cursor2], a, base1, len1, len1 - 1);
			if (count1 != 0) {
				dest -= count1;
				cursor1 -= count1;
				len1 -= count1;
				array_copy(a, cursor1 + 1, a, dest + 1, count1);

				if (len1 == 0) {
					goto finnishing;
				}
			}
			a[dest--] = tmp[cursor2--];
			if (--len2 == 1) {
				goto finnishing;
			}

			count2 = len2 - _gallop_left(a[cursor1], tmp, 0, len2, len2 - 1);
			if (count2 != 0) {
				dest -= count2;
				cursor2 -= count2;
				len2 -= count2;
				array_copy(tmp, cursor2 + 1, a, dest + 1, count2);
				if (len2 <= 1) {
					goto finnishing;
				}
			}
			a[dest--] = a[cursor1--];
			if (--len1 == 0) {
				goto finnishing;
			}
			--min_gallop;
		} while ((count1 >= MIN_GALLOP) | (count2 >= MIN_GALLOP));

		if (min_gallop < 0)
			min_gallop = 0;

		min_gallop += 2; // pernalize for leaving gallop mode
	}

finnishing:
	_min_gallop = (min_gallop < 1) ? 1 : min_gallop;

	if (len2 == 1) {
		assert(0 < len1);
		dest -= len1;
		cursor1 -= len1;
		array_copy(a, cursor1 + 1, a, dest + 1, len1);
		a[dest] = tmp[cursor2];
	} else if (len2 == 0) {
		exit(1);
	} else {
		assert(0 == len1);
		assert(0 < len2);
		array_copy(tmp, 0, a, dest - (len2 - 1), len2);
	}
}

static void _merge_at(int i)
{
	assert(_run_stack.stack_size > 1);
	assert(i >= 0);
	assert((i == _run_stack.stack_size - 2) || (i == _run_stack.stack_size - 3));

	int base1 = _run_stack.run_base[i];
	int len1 = _run_stack.run_len[i];
	int base2 = _run_stack.run_base[i + 1];
	int len2 = _run_stack.run_len[i + 1];
	assert((0 < len1) && (0 < len2));
	assert(base1 + len1 == base2);

	_run_stack.run_len[i] = len1 + len2;
	if (i == _run_stack.stack_size - 3) {
		_run_stack.run_base[i + 1] = _run_stack.run_base[i + 2];
		_run_stack.run_len[i + 1] = _run_stack.run_len[i + 2];
	}
	--_run_stack.stack_size;

	int k = _gallop_right(__a[base2], __a, base1, len1, 0);
	assert(0 <= k);

	base1 += k;
	len1 -= k;

	if (len1 == 0)
		return;

	len2 = _gallop_left(__a[base1 + len1 - 1], __a, base2, len2, len2 - 1);
	assert(0 <= len2);

	if (len2 == 0)
		return;

	if (len1 <= len2)
		_merge_lo(__a, base1, len1, base2, len2, _aux_array);
	else
		_merge_hi(__a, base1, len1, base2, len2, _aux_array);
}

static void _merge_collapse(void)
{
	while (_run_stack.stack_size > 1) {
		int n = _run_stack.stack_size - 2;

		if (((0 < n) && (_run_stack.run_len[n - 1] <= _run_stack.run_len[n] + _run_stack.run_len[n + 1])) ||
			((1 < n) && (_run_stack.run_len[n - 2] <= _run_stack.run_len[n - 1] + _run_stack.run_len[n]))) {
			if (_run_stack.run_len[n - 1] < _run_stack.run_len[n + 1])
				--n;
		} else if ((n < 0) || (_run_stack.run_len[n + 1] < _run_stack.run_len[n])) {
			break;  // invariant is established
		}
		_merge_at(n);
	}
}

static void _merge_force_collapse(void)
{
	while (_run_stack.stack_size > 1) {
		int n = _run_stack.stack_size - 2;

		if ((0 < n) && (_run_stack.run_len[n - 1] < _run_stack.run_len[n + 1]))
			--n;
		_merge_at(n);
	}
}

void tim_sort(int a[], int lo, int hi)
{
	assert((0 <= lo) && (lo < hi));

	int n = hi - lo;
	if (n < 2)
		return;

	if (n < MIN_MERGE) {
		insertion_sort(a, lo, hi);
		return;
	}

	_init_run_stack();
	__a = a;
	_aux_array = (int *)malloc(sizeof(a[0]) * n);

	int min_run_len = _min_run_length(n);
	do {
		int run_len = _count_run_and_make_ascending(a, lo, hi);
		if (run_len < min_run_len) {
			int force = n < min_run_len ? n : min_run_len;
			insertion_sort(a, lo, lo + force);
			run_len = force;
		}

		_push_run_stack(lo, run_len);
		_merge_collapse();

		lo += run_len;
		n -= run_len;
	} while (n != 0);

	assert(lo == hi);
	_merge_force_collapse();
	assert(_run_stack.stack_size == 1);
}
