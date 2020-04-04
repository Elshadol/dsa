static inline void swap_int(int *a, int *b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void gnome_sort(int *a, int n)
{
  if (n < 2)
    return;

  for (int i = 1; i < n; ) {
    if (a[i - 1] < a[i]) {
      ++i;
    } else {
      swap_int(&a[i - 1], &a[i]);

      --i;

      if (i == 0)
	i = 1;
    }
  }
}

void gnome_sort(int *a, int n)
{
  if (n < 2)
    return;

  // find min elem and swap it with a[0], so a[0] now became sentinal
  int min_elem_idx = 0;
  for (int i = 1; i < n; ++i) {
    if (a[i] < a[min_elem_idx])
      min_elem_idx = i;
  }

  swap_int(&a[min_elem_idx], &a[0]);

  for (int i = 2; i < n; ) {
    if (a[i - 1] > a[i]) {
      swap_int(&a[i - 1], &a[i]);

      --i;
    } else {
      ++i;
    }
  }
}
