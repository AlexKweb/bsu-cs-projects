#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <dlfcn.h>

int* arr;
int n;
int minVal, maxVal, minIdx, maxIdx;
double avg;

void* MinMax(void*) {
    void* h = dlopen("libmydll.dylib", RTLD_LAZY);
    if (!h) { std::cerr << dlerror() << "\n"; return nullptr; }

    typedef void (*F)(int*, int, int*, int*);
    F f = (F)dlsym(h, "FindMinMax");
    f(arr, n, &minVal, &maxVal);
    dlclose(h);

    for (int i = 0; i < n; i++) if (arr[i] == minVal) { minIdx = i; break; }
    for (int i = 0; i < n; i++) if (arr[i] == maxVal) { maxIdx = i; break; }

    std::cout << "Min: " << minVal << "\n";
    std::cout << "Max: " << maxVal << "\n";
    return nullptr;
}

void* Average(void*) {
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
        usleep(12 * 1000); // 12 мс
    }
    avg = (double)sum / n;
    std::cout << "Average: " << avg << "\n";
    return nullptr;
}

int main() {
    std::cout << "n = ";
    std::cin >> n;
    arr = new int[n];
    std::cout << "Enter elements: ";
    for (int i = 0; i < n; i++) std::cin >> arr[i];

    pthread_t t1, t2;
    pthread_create(&t1, nullptr, MinMax, nullptr);
    pthread_create(&t2, nullptr, Average, nullptr);

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);

    arr[minIdx] = (int)avg;
    arr[maxIdx] = (int)avg;

    std::cout << "Result: ";
    for (int i = 0; i < n; i++) std::cout << arr[i] << " ";
    std::cout << "\n";

    delete[] arr;
    return 0;
}