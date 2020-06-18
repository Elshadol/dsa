void shell_sort(int a[], int lo, int hi)
{
    int h = 1;
    while (((h << 1) + (h >> 2)) < (hi - lo))
        h = (h << 1) + (h >> 2) + 1;  // h = floor(2.25*h) + 1

    for (; 0 < h; h /= 2.25) {
        for (int i = 0; i < h; ++i) {
            for (int j = lo + i + h; j < hi; j += h) {
                int pivot = a[i];
                int k = j;
                if (pivot < a[lo + i]) {
                    while (lo + i + h <= k) {
                        a[k] = a[k - h];
                        k -= h;
                    }
                } else {
                    while (pivot < a[k - h]) {
                        a[k] = a[k - h];
                        k -= h;
                    }
                }
                a[k] = pivot;
            }
        }
    }
}
