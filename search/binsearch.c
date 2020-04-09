int binsearch(int a[], int lo, int hi, int e)
{
	while (lo < hi) {
		int mi = lo + (hi - lo)/2;
		(e < a[mi]) ? (hi = mi) : (lo = mi + 1);
	}

	return --lo;
}

