#include <iostream>
#include <fstream>
using namespace std;

void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        swap(arr[i], arr[minIndex]);
    }
}

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    const string inputPath = (argc > 1) ? argv[1] : "input.txt";
    ifstream fin(inputPath);
    if (!fin.is_open()) {
        cerr << "Failed to open input file: " << inputPath << endl;
        return 1;
    }

    int n;
    if (!(fin >> n)) {
        cerr << "Failed to read array size." << endl;
        return 1;
    }

    int arr[1000];
    for (int i = 0; i < n; i++) {
        if (!(fin >> arr[i])) {
            cerr << "Not enough elements in input file." << endl;
            return 1;
        }
    }

    int type;
    if (!(fin >> type)) {
        cerr << "Failed to read sort type." << endl;
        return 1;
    }

    switch (type) {
        case 1:
            insertionSort(arr, n);
            break;
        case 2:
            selectionSort(arr, n);
            break;
        case 3:
            bubbleSort(arr, n);
            break;
        default:
            cout << "Invalid type!" << endl;
            return 0;
    }

    cout << "Sorted array: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}
