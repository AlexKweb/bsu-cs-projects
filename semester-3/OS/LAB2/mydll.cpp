#include <unistd.h>

extern "C" void FindMinMax(int* a, int n, int* mn, int* mx) {
    *mn = *mx = a[0];
    for (int i = 1; i < n; i++) {
        if (a[i] < *mn) *mn = a[i];
        usleep(7 * 1000); // 7 мс
        if (a[i] > *mx) *mx = a[i];
        usleep(7 * 1000); // 7 мс
    }
}