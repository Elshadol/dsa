#include <stdio.h>

void deselection_sort(int a[], int lo, int hi)
{
  if (hi - lo < 2)
    return;

  int left_pass = lo;
  int right_pass = hi;
  /* loop invariant:
   * a[lo, left_pass) is in order
   * a[right_pass, hi) is in order
   * when left_pass == right_pass, a[lo, hi) is sorted
   */
  while (left_pass != right_pass) {
    int min_elem_idx = left_pass;

    for (int i = min_elem_idx + 1; i < right_pass; ++i) {
		if (a[i] < a[min_elem_idx]) {
			min_elem_idx = i;
		}
    }

    int tmp = a[left_pass];
    a[left_pass] = a[min_elem_idx];
    a[min_elem_idx] = tmp;

	if (++left_pass == right_pass)
		break;

    int max_elem_idx = right_pass - 1;
    for (int i = max_elem_idx - 1; i >= left_pass; --i) {
      if (a[i] > a[max_elem_idx])
		max_elem_idx = i;
    }


	--right_pass;
      tmp = a[right_pass];
      a[right_pass] = a[max_elem_idx];
      a[max_elem_idx] = tmp;
  }
}

void selection_sort(int a[], int lo, int hi)
{
	if (hi - lo < 2)
		return;

	/*
	 * a[right_pass, hi) is sorted,
	 * find max elem in a[lo, right_pass),
	 * and swap it with a[right_pass - 1]
	 */
	for (int right_pass = hi; lo < right_pass; ) {
		int max_elem_idx = right_pass - 1;
		for (int i = max_elem_idx - 1; lo <= i; --i) {
			if (a[max_elem_idx] < a[i])
				max_elem_idx = i;
		}

		--right_pass;
		int t = a[right_pass];
		a[right_pass] = a[max_elem_idx];
		a[max_elem_idx] = t;
	}
}

static int a[100];

int main(void)
{
	for (int i = 0; i < 100; ++i)
		a[i] = 99 - i;

	selection_sort(a, 0, 100);

	for (int i = 0; i < 100; ++i)
		printf("%d\n", a[i]);

	return 0;
}
