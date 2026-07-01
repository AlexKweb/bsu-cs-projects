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
        if (line == "#2") {
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

    int type;
    if (!(fin >> type)) {
        std::cout << "Нет типа сортировки";
        return 0;
    }

    switch (type) {
        case 1:
            arr.insertionSort();
            break;
        case 2:
            arr.selectionSort();
            break;
        case 3:
            arr.bubbleSort();
            break;
        default:
            return 0;
    }

    std::cout << "Отсортированный массив: " << arr << std::endl;
    return 0;
}
