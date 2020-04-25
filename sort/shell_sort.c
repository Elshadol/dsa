void shell_sort(int a[], int lo, int hi)
{
	int h = 1;

	while (h < (hi - lo)/3)
		h = h*3 + 1;

	for (; h > 0; h /= 3) {
		for (int i = lo + h; i < hi; i++) {
			int pivot = a[i];
			int j = i;
			for (; (lo <= j - h) && (pivot < a[j - h]); j -= h)
				a[j] = a[j - h];

			a[j] = pivot;
		}
	}
}
