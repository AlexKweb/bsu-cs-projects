#include "Array.h"
#include <fstream>

int main() {
    std::ifstream fin("input.txt");
    if (!fin) {
        std::cout << "Не удалось открыть input.txt";
        return 0;
    }

    std::string line;
    bool found = false;
    while (std::getline(fin, line)) {
        if (line == "#3") {
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "Метка задания не найдена";
        return 0;
    }

    size_t n;
    if (!(fin >> n)) {
        std::cout << "Нет размера массива";
        return 0;
    }
    Array<int> arr(n);

    for (size_t i = 0; i < n; ++i) {
        int value;
        if (!(fin >> value)) {
            std::cout << "Недостаточно элементов массива";
            return 0;
        }
        arr.add(value);
    }

    arr.insertionSort();

    int target;
    if (!(fin >> target)) {
        std::cout << "Нет искомого значения";
        return 0;
    }

    long pos = arr.binarySearch(target);
    if (pos >= 0) {
        std::cout << "Найдено на позиции " << pos;
    } else {
        std::cout << "Элемент не найден";
    }
    return 0;
}
