#include <math.h>

int jumpsearch(int a[], int N, int key)
{
	int step = (int)sqrt((double)N);
	int start = 0;
	int end = step;

	while ((end < N) && (a[end] <= key)) {
		start = end;
		end += step;
		if (end >= N)
			end = N;
	}

	// now a[start] <= key < a[end]
	while (end > start) {
		if (a[--end] == key)
			return end;
	}
	return -1;
}
