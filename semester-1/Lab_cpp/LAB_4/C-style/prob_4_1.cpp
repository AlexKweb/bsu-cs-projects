#include <iostream>
#include <cstring>
#include <cstdlib>  // для qsort
#include <fstream>  // для ifstream

using namespace std;

int compare_strings(const void* a, const void* b) {
    // a и b - это указатели на элементы массива (которые сами являются char*).
    // Мы приводим их к char** и разыменовываем, чтобы получить char* (строку),
    // которую затем передаем в strcmp.
    const char** str1_ptr = (const char**)a;
    const char** str2_ptr = (const char**)b;
    
    return strcmp(*str1_ptr, *str2_ptr); // strcmp возвращает <0, 0, >0
}

int main() {
    int n;

    ifstream in("for_task_1.txt");
    in >> n;
    in.ignore(); //

    char** arr = new char*[n];

    for (int i = 0; i < n; i++) {
        char* temp = new char[256]; 
        in.getline(temp, 256);  
        arr[i] = new char[strlen(temp) + 1];
        strcpy(arr[i], temp); 
    }
    in.close();
    
    qsort(arr, n, sizeof(char*), compare_strings);

    cout << endl << " after qsort \t ==>" << endl;
    for (int i = 0; i < n; i++) {
        cout << arr[i] << endl;
    }

    for (int i = 0; i < n; i++)
    {
        delete[] arr[i];
        arr[i] = nullptr;
    }
    delete[] arr;
    arr = nullptr;

    return 0;
}
