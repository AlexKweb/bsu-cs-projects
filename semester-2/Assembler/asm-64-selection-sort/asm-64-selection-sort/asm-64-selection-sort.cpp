#include <iostream>
#include <iomanip>

extern "C" void Sort(int* arr, int n);

void PrintArray(const int* arr, int n)
{
    for (int i = 0; i < n; ++i)
        std::cout << arr[i] << ' ';
    std::cout << '\n';
}

int main()
{
    int arr[] = { 64, 25, 12, 22, 11, 90, 3, 7 };
    int n = sizeof(arr) / sizeof(arr[0]);

    std::cout << "Array:\n";
    PrintArray(arr, n);

    Sort(arr, n);

    std::cout << "After SelectionSort:\n";
    PrintArray(arr, n);

    return 0;
}