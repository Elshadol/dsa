#include "sort_helper.h"

void gnome_sort(int *a, int n)
{
    if (n < 2)
        return;

    for (int i = 1; i < n; ) {
        if (a[i - 1] < a[i]) {
            ++i;
        } else {
            swap(&a[i - 1], &a[i]);

            --i;
            if (i == 0)
                i = 1;
        }
    }
}
