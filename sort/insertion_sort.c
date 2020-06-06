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
			while (n-- > 0) {
				a[start] = a[start - 1];
				--start;
			}
			break;
		}
		a[left] = pivot;
	}
}

/* assert elements before lo are sorted,
 * and these elements are smallest in array,
 * so these elements play a role of sentinal
 */
void pair_insertion_sort(int a[], int lo, int hi)
{
	int i = lo;
	for ( ; i + 1 < hi; i += 2) {
		int a1, a2;
		if (a[i] < a[i + 1]) {
			a1 = a[i + 1];
			a2 = a[i];
		} else {
			a1 = a[i];
			a2 = a[i + 1];
		}
		int j = i;
		while (a1 < a[j - 1]) {
			a[j + 1] = a[j - 1];
			--j;
		}
		a[j + 1] = a1;

		while (a2 < a[j - 1]) {
			a[j] = a[j - 1];
			--j;
		}
		a[j] = a2;
	}

	if (i < hi) {
		int tmp = a[i];
		int j = i;
		for ( ; tmp < a[j - 1]; --j)
			a[j] = a[j - 1];
		a[j] = tmp;
	}
}
