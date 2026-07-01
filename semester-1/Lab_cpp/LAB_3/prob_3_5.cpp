// #include <iostream>
// #include <cstring>   // memcpy, strcmp
// using namespace std;

// // универсальная сортировка: массив a, num элементов, len - размер элемента в байтах
// // cmp - функция сравнения (как в qsort)
// void sort(void* a, int num, int len, int (*cmp)(const void*, const void*)) {
//     if (num <= 1 || a == nullptr || cmp == nullptr) return;

//     char* arr = (char*) a;
//     // временный буфер для обмена
//     char* tmp = new char[len];

//     // простой пузырёк (один из самых понятных алгоритмов)
//     for (int i = 0; i < num - 1; ++i) {
//         for (int j = 0; j < num - 1 - i; ++j) {
//             void* p1 = arr + j * len;
//             void* p2 = arr + (j + 1) * len;
//             if (cmp(p1, p2) > 0) {
//                 // поменять местами p1 и p2 (memcpy через tmp)
//                 memcpy(tmp, p1, len);
//                 memcpy(p1, p2, len);
//                 memcpy(p2, tmp, len);
//             }
//         }
//     }
//     delete[] tmp;
// }

// // Пример cmp для int
// int cmp_int(const void* p1, const void* p2) {
//     int a = *(const int*)p1;
//     int b = *(const int*)p2;
//     if (a < b) return -1;
//     if (a > b) return 1;
//     return 0;
// }

// int cmp_cstr(const void* p1, const void* p2) {
//     const char* const* s1 = (const char* const*) p1;
//     const char* const* s2 = (const char* const*) p2;
//     return strcmp(*s1, *s2);
// }

// int main() {
//     // Пример 1: sort int 
//     int arr[] = { 5, 2, 9, 1, 7 };
//     int n = sizeof(arr) / sizeof(arr[0]);

//     cout << "Before int sort: ";
//     for (int i = 0; i < n; ++i) cout << arr[i] << " ";
//     cout << endl;

//     sort(arr, n, sizeof(int), cmp_int);

//     cout << "After int sort:  ";
//     for (int i = 0; i < n; ++i) cout << arr[i] << " ";
//     cout << endl << endl;

//     // Пример 2: sort c-sring
//     const char* names[] = { "Vasya", "Anna", "Ivan", "Boris" };
//     int m = sizeof(names) / sizeof(names[0]);

//     cout << "Before strings sort: ";
//     for (int i = 0; i < m; ++i) cout << names[i] << " ";
//     cout << endl;

//     sort((void*)names, m, sizeof(char*), cmp_cstr);

//     cout << "After strings sort:  ";
//     for (int i = 0; i < m; ++i) cout << names[i] << " ";
//     cout << endl;

//     cout << "After int sort:  ";
//     for (int i = 0; i < m; ++i) cout << names[i] << " ";
//     cout << endl;

    

//     return 0;
// }


