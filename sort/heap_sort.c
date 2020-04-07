#define PARENT_OF(i) (((i) - 1) / 2)
#define LEFT_CHILD_OF(p) ((p) * 2 + 1)
#define RIGHT_CHILD_OF(p) ((p) * 2 + 2)

static void _percolate_down(int a[], int N, int p)
{
	int temp = a[p];
	int c = LEFT_CHILD_OF(p);

	while (c < N) {
		if ((c + 1 < N) && (a[c] < a[c + 1]))
			++c;

		if (a[c] < temp)
			break;

		a[p] = a[c];
		p = c;
		c = LEFT_CHILD_OF(p);
	}

	a[p] = temp;
}

static inline void _heapificate(int a[], int N)
{
	// make max-heap start from the last internal elem in the bst
	for (int i = PARENT_OF(N - 1); 0 <= i; --i) {
		_percolate_down(a, N, i);
	}
}

// a[0, N) used to be a max-heap before popping a[0],
// after calling this function, a[0, p) is a max-heap
static inline int _adjust_after_pop_heap(int a[], int N)
{
	int p = 0;
	int c = RIGHT_CHILD_OF(p);

	while (c < N) {
		if (a[c] < a[c - 1])
			--c;

		a[p] = a[c];
		p = c;
		c = RIGHT_CHILD_OF(p);
	}

	// no right child, only have left one
	if (c == N) {
		a[p] = a[c - 1];
		p = c - 1;
	}

	return p;
}

/* regard a[0, last) as a max-heap, and insert val to it */
static inline void _push_heap(int a[], int last, int val)
{
	int i = last;

	while ((0 < i) && (a[PARENT_OF(i)] < val)) {
		a[i] = a[PARENT_OF(i)];
		i = PARENT_OF(i);
	}

	a[i]  = val;
}

void heap_sort(int a[], int lo, int hi)
{
	if (hi - lo < 2)
		return;

	int *b = a + lo;
	int N = hi - lo;
	_heapificate(b, N);


	/*
	 * b[0, i) is max-heap,  b[i, N) is sorted.
	 * swap b[0] with b[i - 1], and readjust b[0, i - 1) into max-heap
	 */
	for (int i = N; --i > 0; ) {
		int temp  = b[i];
		b[i] = b[0];
		b[0] = temp;

		_percolate_down(b, i, 0);
	}
}

void heap_sort1(int a[], int lo, int hi)
{
	if (hi - lo < 2)
		return;

	int *b = a + lo;
	const int N = hi - lo;

	/*
	 * 1. put items into a valid max-heap;
	 */
	_heapificate(b, N);

	for (int i = N; 0 < i; --i) {
		/*
		 * 2. remove the top elem of the heap, creating a vacancy V
		 */
		int temp = b[0];

		/*
		 * 3. Compare the two sub-heap leaders directly below V,
		 * and promote the biggest one into the vacancy.
		 * Recursively repeat step 3, redifining V to be the new vacancy,
		 * until reach the bottom of the heap
		 */
		int last = _adjust_after_pop_heap(b, i);
		int right = b[i - 1];
		_push_heap(b, last, right);

		b[i - 1] = temp;
	}
}
