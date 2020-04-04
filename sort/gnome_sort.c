#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

static inline void swap_int(int *a, int *b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

static int a[100];

#if 0
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
#else
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
#endif

bool is_ordered(const int *a, int n)
{
  for (int i = 1; i < n; ++i) {
    if (a[i - 1] > a[i]) 
      return false;
  }

  return true;
}

int main(void)
{
  srand((unsigned)time(NULL));
  
  for (int i = 0; i < 100; ++i)
    a[i] = rand();

  gnome_sort(a, 100);

  if (is_ordered(a, 100))
    puts("Y");
  else
    puts("N");
  
  return 0;
}
  
