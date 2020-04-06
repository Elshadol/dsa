static const int QUICKSORT_MIN_RANGE = 32;

static inline int _median3(int a, int b, int c)
{
	if (a < b) {
		if (b < c)
			return b;
		else if (a < c)
			return c;
		else
			return a;
	} else {
		if (a < c)
			return a;
		else if (b < c)
			return c;
		else
			return b;
	}
}

static int _partition(int a[], int lo, int hi)
{
	int pivot = _median3(a[lo], a[lo + (hi - lo)/2], a[hi - 1]);

	for (; ;) {
		while (a[lo] < pivot)
			++lo;

		--hi;
		while (pivot < a[hi])
			-- hi;

		if (hi <= lo)
			return lo;

		int t = a[lo];
		a[lo] = a[hi];
		a[hi] = t;

		++lo;
	}
}

static inline void insertion_sort(int a[], int lo, int hi)
{
	for (int i = lo + 1; i < hi; ++i) {
		int pivot = a[i];

		int j = i;
		for (; (lo < j) && (pivot < a[j - 1]); --j)
			a[j] = a[j - 1];

		a[j] = pivot;
	}
}

void quicksort(int a[], int lo, int hi)
{
	if (hi - lo < 2)
		return;

	if (hi - lo < QUICKSORT_MIN_RANGE) {
		insertion_sort(a, lo, hi);
		return;
	}

	int cut = _partition(a, lo, hi);

	quicksort(a, lo, cut);
	quicksort(a, cut + 1, hi);
}
