#include <iostream>
#include "MyArray.h"

using namespace std;

// Сортировка вставками
void insertionSort(MyArray<int>& arr) {
    for (size_t i = 1; i < arr.getSize(); i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Сортировка выбором
void selectionSort(MyArray<int>& arr) {
    for (size_t i = 0; i < arr.getSize() - 1; i++) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < arr.getSize(); j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(arr[i], arr[minIndex]);
        }
    }
}

// Сортировка обменом (пузырьковая)
void bubbleSort(MyArray<int>& arr) {
    for (size_t i = 0; i < arr.getSize() - 1; i++) {
        bool swapped = false;
        for (size_t j = 0; j < arr.getSize() - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
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
    
    cout << "Введите элементы массива: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    cout << "\nИсходный массив: ";
    arr.print();
    
    int choice;
    cout << "\nВыберите тип сортировки:" << endl;
    cout << "1 - Сортировка вставками" << endl;
    cout << "2 - Сортировка выбором" << endl;
    cout << "3 - Сортировка обменом (пузырьковая)" << endl;
    cout << "Ваш выбор: ";
    cin >> choice;
    
    switch (choice) {
        case 1:
            insertionSort(arr);
            cout << "\nОтсортированный массив (вставками): ";
            break;
        case 2:
            selectionSort(arr);
            cout << "\nОтсортированный массив (выбором): ";
            break;
        case 3:
            bubbleSort(arr);
            cout << "\nОтсортированный массив (обменом): ";
            break;
        default:
            cout << "Ошибка: неверный выбор!" << endl;
            return 1;
    }
    
    arr.print();
    
    return 0;
}
