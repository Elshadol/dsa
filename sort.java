private static int countRunAndMakeAscending(Object[] a, int lo, int hi) {
	int runHi = lo + 1;
	if (runHi == hi) {
		return 1;
	}

	// find the end of run, and reverse range if discending
	if (((Comparable)a[runHi++]).compareTo(a[lo]) < 0) {
		while ((runHi < hi) && ((Comparable)a[runHi].compareTo

