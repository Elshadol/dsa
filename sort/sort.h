#ifndef SORT_INCLUDED
#define SORT_INCLUDED

/* sort elems in a[lo, hi) */

void intro_sort(int a[], int lo, int hi);

/* recursive merge_sort */
void merge_sort(int a[], int lo, int hi);

/* non-recursive merge_sort */
void merge_sort2(int a[], int lo, int hi);

void quick_sort(int a[], int lo, int hi);

void heap_sort(int a[], int lo, int hi);

/* still has bugs to fix */
void tim_sort(int a[], int lo, int hi);

void shell_sort(int a[], int lo, int hi);

void insertion_sort(int a[], int lo, int hi);

/* assert a[lo, start) is sorted */
void binary_insertion_sort(int a[], int lo, int hi, int start);

#endif
