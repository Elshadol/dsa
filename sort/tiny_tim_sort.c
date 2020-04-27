#include <stdlib.h>

#include "sort_helper.h"
#include "sort.h"

#define MIN_MERGE 128

static int *_aux_array;
static int *__a;

#define RUN_STACK_SIZE 49
int _run_base[RUN_STACK_SIZE];
int _run_len[RUN_STACK_SIZE];
int _run_stack_size;

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

static inline int _compute_min_run_length(int n)
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

static inline void _merge(int a[], int base1, int len1,
					  int base2, int len2, int aux[])
{
	array_copy(a, base1, aux, 0, len1);

	int cursor1 = 0;
	int cursor2 = base2;
	int dest = base1;

	while (cursor1 < len1) {
		if ((cursor2 < base2 + len2) && (a[cursor2] < aux[cursor1]))
			a[dest++] = a[cursor2++];

		if ((base2 + len2 <= cursor2) || (aux[cursor1] <= a[cursor2]))
			a[dest++] = aux[cursor1++];
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

	_merge(__a, base1, len1, base2, len2, _aux_array);
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
	_aux_array = (int *)malloc(sizeof(a[0]) * n);
	if (_aux_array == NULL)
		exit(1);

	int min_run_len = _compute_min_run_length(n);
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

	_merge_force_collapse();
}
