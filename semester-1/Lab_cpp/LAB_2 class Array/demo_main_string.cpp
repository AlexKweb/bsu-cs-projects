#include "Array.h"
#include <fstream>
#include <string>

int main() {
    std::ifstream fin("demo_input_string.txt");

    size_t cap, n;
    fin >> cap >> n;

    Array<std::string> arr(cap);
    for (size_t i = 0; i < n; ++i) {
        std::string v;
        fin >> v;
        arr.add(v);
    }

    int type;
    fin >> type;

    std::string x;
    fin >> x;

    std::cout << "\nИсходный массив (" << arr.getSize() << "): " << arr << "\n";

    if (type == 1) arr.insertionSort();
    if (type == 2) arr.selectionSort();
    if (type == 3) arr.bubbleSort();

    std::cout << "После сортировки: " << arr << "\n";

    long pos = arr.binarySearch(x);
    std::cout << "Бинарный поиск " << x << ": "
              << (pos >= 0 ? "позиция " + std::to_string(pos) : "не найден") << "\n\n";
    return 0;
}
