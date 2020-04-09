static struct {
	// int maybe overflow soon
	int f;  // f = fib(n - 1)
	int g;  // g = fib(n)
} fib;

static inline void __init_fib(int n)
{
	fib.f = 1;
	fib.g = 0;

	while (fib.g < n) {
		fib.g += fib.f;
		fib.f = fib.g - fib.f;
	}
}

static inline int __get_fib(void)
{
	return fib.g;
}

static inline int __next_fib(void)
{
	fib.g += fib.f;
	fib.f = fib.g - fib.f;

	return fib.g;
}

static inline int __prev_fib(void)
{
	fib.f = fib.g - fib.f;
	fib.g -= fib.f;

	return fib.g;
}

int fibsearch(int a[], int lo, int hi, int key)
{
	__init_fib(hi - lo);

	while (lo < hi) {
		while (hi - lo < __get_fib())
			__prev_fib();

		int mi = lo + __get_fib() - 1;

		if (key < a[mi])
			hi = mi;
		else if (a[mi] < key)
			lo = mi + 1;
		else
			return mi;
	}

	return -1;
}
