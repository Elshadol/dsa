#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static const int SedgewickGap[] = {
	0, 1, 5, 19, 41, 109, 209, 505, 929, 2161, 3905, 8929, 16001,
};

static const int SedgewickGapSize =
	sizeof(SedgewickGap) / sizeof(SedgewickGap[0]);

void shellSort0(int a[], int lo, int hi)
{
	const int n = hi - lo;
	int idx = SedgewickGapSize;
	while (SedgewickGap[--idx] >= n);

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

void shellSort1(int a[], int lo, int hi)
{
	const int n = hi - lo;
	int h = 1;
	while (h < n/3) h = h*3 + 1;

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

void shellSort2(int a[], int lo, int hi)
{
	int n = hi - lo;
	int h = 1;

	while (h < n/3) h = h*3 + 1;

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

static int n[3000];

int main(void) {
	for (int i = 0; i < 3000; ++i)
		n[i] = 3000 - i;



	shellSort2(n, 0, 3000);

	for (int i = 0; i < 3000; ++i) {
		printf("%d ", n[i]);
		if (i % 10 == 0)
			puts(" ");
	}
	return 0;
}
