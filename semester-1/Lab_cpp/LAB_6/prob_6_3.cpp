#include <iostream>
#include "MyArray.h"

using namespace std;

// Бинарный поиск в отсортированном массиве
int binarySearch(const MyArray<int>& arr, int target) {
    int left = 0;
    int right = arr.getSize() - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        }
        
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return -1; // Элемент не найден
}

int main() {
    int n;
    cout << "Введите размерность массива: ";
    cin >> n;
    
    if (n <= 0) {
        cout << "Ошибка: размерность должна быть положительным числом!" << endl;
        return 1;
    }
    
    MyArray<int> arr(n);
    
    cout << "Введите элементы отсортированного массива (в порядке возрастания): ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    cout << "\nМассив: ";
    arr.print();
    
    int target;
    cout << "\nВведите искомый элемент: ";
    cin >> target;
    
    int index = binarySearch(arr, target);
    
    if (index != -1) {
        cout << "Элемент " << target << " найден на позиции " << index << " (индекс начинается с 0)" << endl;
    } else {
        cout << "Элемент " << target << " не найден в массиве" << endl;
    }
    
    return 0;
}
