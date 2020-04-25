void insertion_sort(int a[], int lo, int hi)
{
	for (int i = lo + 1; i < hi; ++i) {
		int pivot = a[i];

		int j = i;
		if (pivot < a[lo]) {
			while (lo < j) {
				a[j] = a[j - 1];
				--j;
			}
		} else {
			 while (pivot < a[j - 1]) {
				a[j] = a[j - 1];
				--j;
			 }
		}
		a[j] = pivot;
	}
}

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
