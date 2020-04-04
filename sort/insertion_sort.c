static const int BINARY_INSERTION_SORT_THRESHOLD = 47;

inline void insertion_sort1(int a[], int lo, int hi)
{
	for (int i = lo + 1; i < hi; ++i) {
		int j = i;
		int pivot = a[i];
		for (; (j > lo) && (a[j - 1] > pivot); --j)
			a[j] = a[j - 1];
		a[j] = pivot;
	}
}

inline void insertion_sort2(int a[], int lo, int hi)
{
	/*
	 * loop invariant: a[lo, i) is always sorted
	 * at beginning, [lo, i) = [lo, lo + 1)
	 * then repeat inserting the first elem in [i, hi) to [lo, i),
	 * until [i, hi) == {}
	 */
	for (int i = lo + 1; i < hi; ++i) {
		int pivot = a[i];
		if (pivot < a[lo]) {
			for (; i > lo; --i)
				a[i] = a[i - 1];
			a[lo] = pivot;
		} else {
			int j = i;
			for (; a[j - 1] > pivot; --j)
				a[j] = a[j - 1];
			a[j] = pivot;
		}
	}
}

/*
 * insertion sort using binsearch
 * assert a[lo, start) is sorted
 */
void binary_insertion_sort(int a[], int lo, int hi, int start)
{
	if (start == lo) {
		start++;
	}

	for (; start < hi; ++start) {
		int pivot = a[start];

		int left = lo;
		int right = start;

		/*
		 * loop invariant:
		 * pivot >= a[lo, left)
		 * pivot < a[right, hi)
		 */
		while (left < right) {
			int mid = left + (right - left) / 2;
			if (pivot < a[mid])
				right = mid;
			else
				left = mid + 1;
		}

		int n = start - left;
		switch (n) {
		case 2:
			a[left + 2] = a[left + 1];
		case 1:
			a[left + 1] = a[left];
			break;
		default:
			// move a[left, start), to [left + 1, start + 1)
			while (n-- > 0) {
				a[start] = a[start - 1];
				--start;
			}
			break;
		}
		a[left] = pivot;
	}
}

void insertion_sort(int a[], int lo, int hi)
{
	if (hi - lo < 2)
		return;

	if (hi - lo < BINARY_INSERTION_SORT_THRESHOLD) {
		insertion_sort1(a, lo, hi);
	} else {
		insertion_sort1(a, lo, BINARY_INSERTION_SORT_THRESHOLD);
		binaryInsertionSort(a, lo, hi, BINARY_INSERTION_SORT_THRESHOLD);
	}
}
