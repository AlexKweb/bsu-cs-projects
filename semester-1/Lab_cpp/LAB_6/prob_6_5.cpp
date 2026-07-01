#include <iostream>
#include "MyArray.h"

using namespace std;

// Найти в целочисленной квадратной матрице такие элементы,
// которые являются одновременно наибольшими в столбце и наименьшими в строке

void findSaddlePoints(int** matrix, int n) {
    bool found = false;
    
    for (int i = 0; i < n; i++) {
        // Находим минимальный элемент в строке i
        int minInRow = matrix[i][0];
        int minCol = 0;
        for (int j = 1; j < n; j++) {
            if (matrix[i][j] < minInRow) {
                minInRow = matrix[i][j];
                minCol = j;
            }
        }
        
        // Проверяем, является ли этот элемент максимальным в своем столбце
        bool isMaxInCol = true;
        for (int k = 0; k < n; k++) {
            if (matrix[k][minCol] > minInRow) {
                isMaxInCol = false;
                break;
            }
        }
        
        if (isMaxInCol) {
            cout << "Элемент " << minInRow << " на позиции [" << i << "][" << minCol 
                 << "] является наибольшим в столбце " << minCol 
                 << " и наименьшим в строке " << i << endl;
            found = true;
        }
    }
    
    if (!found) {
        cout << "Таких элементов не найдено" << endl;
    }
}

int main() {
    int n;
    cout << "Введите размерность квадратной матрицы: ";
    cin >> n;
    
    if (n <= 0) {
        cout << "Ошибка: размерность должна быть положительным числом!" << endl;
        return 1;
    }
    
    // Выделяем память для матрицы
    int** matrix = new int*[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[n];
    }
    
    cout << "Введите элементы матрицы:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> matrix[i][j];
        }
    }
    
    cout << "\nВведенная матрица:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    
    cout << "\nРезультат поиска:" << endl;
    findSaddlePoints(matrix, n);
    
    // Освобождаем память
    for (int i = 0; i < n; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    
    return 0;
}
