#include "sort.h"
#include "sort_helper.h"

#include <stdlib.h>

#define INSERTION_SORT_THRESHOLD 32

static void _merge(int a[], int lo, int mi, int hi, int aux[])
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

static void _merge_sort(int a[], int lo, int hi, int aux[])
{
	if (hi - lo < INSERTION_SORT_THRESHOLD) {
		insertion_sort(a, lo, hi);
		return;
	}

	int mi = lo + ((hi - lo) >> 1);
	_merge_sort(a, lo, mi, aux);
	_merge_sort(a, mi, hi, aux);
	_merge(a, lo, mi, hi, aux);
}

void merge_sort(int a[], int lo, int hi)
{
	if (hi - lo < 2)
		return;

	int *aux = (int *)malloc(sizeof(a[0]) * (hi - lo) / 2);
	if (aux != NULL) {
		_merge_sort(a, lo, hi, aux);
		free(aux);
	} else 
		exit(1);
}
