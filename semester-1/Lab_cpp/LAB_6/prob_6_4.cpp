#include <iostream>
#include "MyArray.h"

using namespace std;

// Определить, есть ли в целочисленной квадратной матрице 5-го порядка
// прямоугольник, вершинами которого являются заданные числа

const int MATRIX_SIZE = 5;

bool findRectangle(int matrix[MATRIX_SIZE][MATRIX_SIZE], int num1, int num2, int num3, int num4) {
    // Ищем все позиции заданных чисел
    int positions[4][2][100]; // [число][координаты][индекс позиции]
    int counts[4] = {0};
    
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (matrix[i][j] == num1) {
                positions[0][0][counts[0]] = i;
                positions[0][1][counts[0]] = j;
                counts[0]++;
            }
            if (matrix[i][j] == num2) {
                positions[1][0][counts[1]] = i;
                positions[1][1][counts[1]] = j;
                counts[1]++;
            }
            if (matrix[i][j] == num3) {
                positions[2][0][counts[2]] = i;
                positions[2][1][counts[2]] = j;
                counts[2]++;
            }
            if (matrix[i][j] == num4) {
                positions[3][0][counts[3]] = i;
                positions[3][1][counts[3]] = j;
                counts[3]++;
            }
        }
    }
    
    // Проверяем, можно ли из этих позиций составить прямоугольник
    // Прямоугольник: (x1,y1), (x1,y2), (x2,y1), (x2,y2)
    for (int i1 = 0; i1 < counts[0]; i1++) {
        for (int i2 = 0; i2 < counts[1]; i2++) {
            for (int i3 = 0; i3 < counts[2]; i3++) {
                for (int i4 = 0; i4 < counts[3]; i4++) {
                    int x1 = positions[0][0][i1], y1 = positions[0][1][i1];
                    int x2 = positions[1][0][i2], y2 = positions[1][1][i2];
                    int x3 = positions[2][0][i3], y3 = positions[2][1][i3];
                    int x4 = positions[3][0][i4], y4 = positions[3][1][i4];
                    
                    // Проверяем все возможные комбинации для прямоугольника
                    int points[4][2] = {{x1, y1}, {x2, y2}, {x3, y3}, {x4, y4}};
                    int values[4] = {num1, num2, num3, num4};
                    
                    // Проверяем, могут ли эти 4 точки образовать прямоугольник
                    for (int p1 = 0; p1 < 4; p1++) {
                        for (int p2 = 0; p2 < 4; p2++) {
                            if (p2 == p1) continue;
                            for (int p3 = 0; p3 < 4; p3++) {
                                if (p3 == p1 || p3 == p2) continue;
                                for (int p4 = 0; p4 < 4; p4++) {
                                    if (p4 == p1 || p4 == p2 || p4 == p3) continue;
                                    
                                    int px1 = points[p1][0], py1 = points[p1][1];
                                    int px2 = points[p2][0], py2 = points[p2][1];
                                    int px3 = points[p3][0], py3 = points[p3][1];
                                    int px4 = points[p4][0], py4 = points[p4][1];
                                    
                                    // Проверяем, являются ли эти 4 точки вершинами прямоугольника
                                    // Прямоугольник: противоположные углы имеют одинаковые координаты
                                    if (((px1 == px2 && py3 == py4 && py1 != py3 && px3 == px4) ||
                                         (px1 == px3 && py2 == py4 && py1 != py2 && px2 == px4) ||
                                         (px1 == px4 && py2 == py3 && py1 != py2 && px2 == px3)) &&
                                        ((px2 == px3 && py1 == py4) ||
                                         (px2 == px4 && py1 == py3) ||
                                         (px3 == px4 && py1 == py2))) {
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    return false;
}

int main() {
    int matrix[MATRIX_SIZE][MATRIX_SIZE];
    
    cout << "Введите элементы квадратной матрицы 5x5:" << endl;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            cin >> matrix[i][j];
        }
    }
    
    cout << "\nВведенная матрица:" << endl;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    
    int num1, num2, num3, num4;
    cout << "\nВведите четыре числа (вершины прямоугольника): ";
    cin >> num1 >> num2 >> num3 >> num4;
    
    if (findRectangle(matrix, num1, num2, num3, num4)) {
        cout << "В матрице существует прямоугольник с вершинами " 
             << num1 << ", " << num2 << ", " << num3 << ", " << num4 << endl;
    } else {
        cout << "В матрице нет прямоугольника с вершинами " 
             << num1 << ", " << num2 << ", " << num3 << ", " << num4 << endl;
    }
    
    return 0;
}
