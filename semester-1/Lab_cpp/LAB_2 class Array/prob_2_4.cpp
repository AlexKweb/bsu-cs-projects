#include "Array.h"
#include <fstream>

struct Position {
    int r;
    int c;
};

bool isRectangle(int minR, int maxR, int minC, int maxC, const Position& p1, const Position& p2, const Position& p3, const Position& p4) {
    if (minR == maxR || minC == maxC) {
        return false;
    }
    const Position points[4] = {p1, p2, p3, p4};
    for (const auto& p : points) {
        if (!((p.r == minR || p.r == maxR) && (p.c == minC || p.c == maxC))) {
            return false;
        }
    }
    return true;
}

int main() {
    std::ifstream fin("input.txt");
    if (!fin) {
        std::cout << "Не удалось открыть input.txt";
        return 0;
    }

    std::string line;
    bool found = false;
    while (std::getline(fin, line)) {
        if (line == "#4") {
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "Метка задания не найдена";
        return 0;
    }

    Array<int> matrix(25);
    for (int i = 0; i < 25; ++i) {
        int value;
        if (!(fin >> value)) {
            std::cout << "Недостаточно данных матрицы";
            return 0;
        }
        matrix.add(value);
    }

    int a, b, c, d;
    if (!(fin >> a >> b >> c >> d)) {
        std::cout << "Нет чисел вершин";
        return 0;
    }

    Array<Position> pa(25), pb(25), pc(25), pd(25);

    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            int val = matrix[i * 5 + j];
            if (val == a) pa.add({i, j});
            if (val == b) pb.add({i, j});
            if (val == c) pc.add({i, j});
            if (val == d) pd.add({i, j});
        }
    }

    for (size_t i1 = 0; i1 < pa.getSize(); ++i1)
        for (size_t i2 = 0; i2 < pb.getSize(); ++i2)
            for (size_t i3 = 0; i3 < pc.getSize(); ++i3)
                for (size_t i4 = 0; i4 < pd.getSize(); ++i4) {
                    int rows[4] = {pa[i1].r, pb[i2].r, pc[i3].r, pd[i4].r};
                    int cols[4] = {pa[i1].c, pb[i2].c, pc[i3].c, pd[i4].c};
                    int minR = rows[0], maxR = rows[0], minC = cols[0], maxC = cols[0];
                    for (int k = 1; k < 4; ++k) {
                        if (rows[k] < minR) minR = rows[k];
                        if (rows[k] > maxR) maxR = rows[k];
                        if (cols[k] < minC) minC = cols[k];
                        if (cols[k] > maxC) maxC = cols[k];
                    }
                    if (isRectangle(minR, maxR, minC, maxC, pa[i1], pb[i2], pc[i3], pd[i4])) {
                        std::cout << "Прямоугольник существует";
                        return 0;
                    }
                }

    std::cout << "Прямоугольник не найден";
    return 0;
}
