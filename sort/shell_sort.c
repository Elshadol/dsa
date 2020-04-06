static const int SedgewickGap[] = {
	0, 1, 5, 19, 41, 109, 209, 505, 929, 2161, 3905, 8929, 16001, 36289, /* etc. */
};

static const int SedgewickGapSize =
	sizeof(SedgewickGap) / sizeof(SedgewickGap[0]);

void shell_sort0(int a[], int lo, int hi)
{
	int idx = SedgewickGapSize;
	while (SedgewickGap[--idx] >= hi- lo)
		continue;

	for (; 0 < idx; --idx) {
		int h =SedgewickGap[idx];
		for (int i = 0; i <  h; ++i) {
			for (int j = lo + i + h; j < hi; j += h) {
				int pivot = a[j];

				int k = j;
				for (; (lo + h <= k) && (pivot < a[k - h]); k -= h)
					a[k] = a[k - h];

				a[k] = pivot;
			}
		}
	}
}



void shell_sort1(int a[], int lo, int hi)
{
	int idx = SedgewickGapSize;
	while (SedgewickGap[--idx] >= hi - lo);

	for (; 0 < idx; --idx) {
		int  h = SedgewickGap[idx];
		for (int i = lo + h; i < hi; i++) {
			int pivot = a[i];

			int j = i;
			for (; (lo <= j - h) && (pivot < a[j - h]); j -= h)
				a[j] = a[j - h];

			a[j] = pivot;
		}
	}
}

void shell_sort2(int a[], int lo, int hi)
{
	int h = 1;
	while (h < (hi - lo)/3) h = h*3 + 1;

	for (; 0 < h; h /= 3) {
		for (int i = 0; i < h; ++i) {
			for (int j = lo + i + h; j < hi; j += h) {
				int pivot = a[j];

				int k = j;
				for (; (lo <= k - h) && (pivot < a[k - h]); k -= h)
					a[k] = a[k - h];
				a[k] = pivot;
			}
		}
	}
}

void shell_sort3(int a[], int lo, int hi)
{
	int h = 1;

	while (h < (hi - lo)/3) h = h*3 + 1;

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
