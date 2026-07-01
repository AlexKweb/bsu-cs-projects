#include <iostream>
#include <string>
#include <cstdlib>  // для qsort
#include <fstream>  // для ifstream

using namespace std;

int compareStrings(const void* a, const void* b) {
    const string* s1 = static_cast<const string*>(a);
    const string* s2 = static_cast<const string*>(b);
    return s1->compare(*s2);
}

int main() {
    int n;
    // cout << "Enter N of stroke: "<< endl;
    // cin >> n;


    // cout << "Enter strokes:\n";
    // for (int i = 0; i < n; i++) {
    //     getline(cin, arr[i]);
    // }

    ifstream in("for_task_1.txt");
    in >> n;
    in.ignore();   // убрать символ новой строки после числа
    string* arr = new string[n];

    for (int i = 0; i < n; i++) {
        getline(in, arr[i]);
    }
    in.close();

    qsort(arr, n, sizeof(string), compareStrings);

    cout << endl << " after qsort \t ==>" << endl;
    for (int i = 0; i < n; i++) {
        cout << arr[i] << endl;
    }

    delete[] arr;
    arr = nullptr;

    return 0;
}
