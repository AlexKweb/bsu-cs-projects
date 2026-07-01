#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    int size = 2 * n + 1;

    vector<vector<int>> A(size, vector<int>(size));

    int x = n; // координаты центра
    int y = n;
    int value = 0;
    A[x][y] = value;

    // направления: вверх, влево, вниз, вправо
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, -1, 0, 1};
    int dir = 0;   // начинаем движение "вверх"
    int step = 1;  // длина шага
    int total = size * size - 1;
    value++;

    while (value <= total) {
        for (int repeat = 0; repeat < 2; ++repeat) {
            for (int i = 0; i < step && value <= total; ++i) {
                x += dx[dir];
                y += dy[dir];
                A[x][y] = value++;
            }
            dir = (dir + 1) % 4;  // поворот против часовой стрелки
        }
        ++step;  // после двух направлений увеличиваем длину шага
    }

    // вывод
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j)
            cout << setw(3) << A[i][j];
        cout << '\n';
    }

    return 0;
}
