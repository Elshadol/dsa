void bubble_sort1(int a[], int n)
{
	for (int i = 0; i < n; ++i) {
		for (int j = 1; j < n - i; ++j) {
			if (a[j - 1] < a[j]) {
				swap(a[j - 1], a[j]);
			}
		}
	}
}
