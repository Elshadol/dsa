#define PARENT_OF(i) (((i) - 1) >> 1)
#define LEFT_CHILD_OF(i) ((i) << 1 + 1)
#define RIGHT_CHILD_OF(i) ((i) << 1 + 2)

static inline int percolate_up(int a[])
{
}

static inline int percolate_down(int a[])
{
}

// make a[lo, hi) become a heap;
static heapificate(int a[], int lo, int hi)
{
}

void heap_sort(int a[], int lo, int hi)
{
	if (hi - lo < 2)
		return;

	if (hi - lo < INSERTION_SORT_THRESHOLD) {
		insertion_sort(a, lo, hi);
		return;
	}

	heapificate(a, lo, hi);
	while (hi - lo > 1) {

}
