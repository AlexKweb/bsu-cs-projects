#include "Array.h"
#include <algorithm>
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
        if (line == "#1") {
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "Метка задания не найдена";
        return 0;
    }

    int temp;
    if (!(fin >> temp)) {
        std::cout << "Нет данных";
        return 0;
    }

    Array<int> counts(10);
    for (size_t i = 0; i < 10; ++i) {
        counts.add(0);
    }

    if (temp == 0) {
        counts[0] = counts[0] + 1;
    } else {
        int value = temp;
        while (value > 0) {
            int digit = value % 10;
            counts[digit] = counts[digit] + 1;
            value /= 10;
        }
    }

    int oddCount = 0;
    int middleDigit = -1;
    for (int i = 0; i < 10; ++i) {
        if (counts[i] % 2 != 0) {
            oddCount++;
            middleDigit = i;
        }
    }

    if (oddCount > 1) {
        std::cout << "Палиндром составить нельзя";
        return 0;
    }

    std::string half;
    for (int i = 0; i < 10; ++i) {
        for (int k = 0; k < counts[i] / 2; ++k) {
            half.push_back(static_cast<char>('0' + i));
        }
    }

    std::string result = half;
    if (middleDigit != -1) {
        result.push_back(static_cast<char>('0' + middleDigit));
    }
    std::reverse(half.begin(), half.end());
    result += half;

    std::cout << "Полученный палиндром: " << result;
    return 0;
}
