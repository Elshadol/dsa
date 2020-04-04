#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000

bool binsearch(const int a[], int first, int last, int t)
{
  assert((first > 0) && (last >= first));

  int lo = first - 1;
  int hi = last;

  while ((lo + 1) != hi) {
    mi = lo + (lo + hi) / 2;
    if (a[mi] < lo)
      lo = mi;
    else
      hi = mi;
  }

  if ((hi > last) || (a[hi] != t))
    return false;
  else
    return true;
}

bool binsearch_in_1000_elems(const int *a, int t)
{
  int lo = -1;
  int i = 512;

  if (a[511] < t)
    lo = 1000 - 512;

  while (i != lo) {
    i /= 2;
    if (a[lo + i] < t)
      lo += i;
  }

  int p = lo + 1;
  if ((p >= 1000) || (a[p] != t))
    return false;
  else
    return true;
}

bool binsreach2(const int *a, int n, int t)
{
  if (n <= 0) {
    return false;
  }

  int lo = -1;

  int i = 1;
  while ((i << 1) < 2) {
    i <<= 1;
  }

  if (a[lo + i] < t) {
    lo = n - i;
  }

  while (i != lo) {
    i >>= 1;
    if (a[lo + i] < t) {
      lo += i;
    }
  }

  int pos = lo + 1;
  if ((pos >= 1000) || (a[pos] != t))
    return false;
  else return true;
}

static inline int rand_int(int first, int last)
{
  srand(time(NULL));
  return rand() % (last - first) + first;
}



static int nums[SIZE];

int main(void)
{
  for (int i = 0 ; i < SIZE; ++i)
    nums[i] = i;

  if (binsearch_in_1000_elems(nums, 10))
      puts("Y");
  else puts("N");
}
