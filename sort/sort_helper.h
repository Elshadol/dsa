#ifndef SORT_HELPER_INCLUDED
#define SORT_HELPER_INCLUDED

/* some small and helpful functions used in sort algorithms */

int disordered(const int a[], int lo, int hi);

void swap(int *a, int *b);

void array_copy(int src[], int corsur1, int dest[], int corsur2, int len);

void reverse_range(int a[], int lo, int hi);

/* turn a[lo, ..., mi, ..., hi) to a[mi, ...hi, ..., lo) */
void rotate(int a[], int lo, int mi, int hi);

#endif
