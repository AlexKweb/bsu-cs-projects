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
        if (line == "#5") {
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
        std::cout << "Нет размера матрицы";
        return 0;
    }

    Array<int> matrix(n * n);
    for (size_t i = 0; i < n * n; ++i) {
        int value;
        if (!(fin >> value)) {
            std::cout << "Недостаточно элементов матрицы";
            return 0;
        }
        matrix.add(value);
    }

    std::cout << "Элементы, минимальные в строке и максимальные в столбце:\n";
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            int elem = matrix[i * n + j];
            bool minInRow = true;
            bool maxInCol = true;

            for (size_t k = 0; k < n; ++k) {
                if (matrix[i * n + k] < elem) {
                    minInRow = false;
                }
                if (matrix[k * n + j] > elem) {
                    maxInCol = false;
                }
            }

            if (minInRow && maxInCol) {
                std::cout << elem << " в позиции (" << i << "," << j << ")\n";
            }
        }
    }

    return 0;
}
