#include <iostream>     
#include <string>     
#include <algorithm>

using namespace std;


int main() {
    struct Student
    {
        string name;
        int num;
        double grade;
    };
    int N = 5;
    // cin >> N;
    Student *arr = new Student[N];
    // for (size_t i = 0; i < N; i++)
    // {
    //     getline(cin, arr[i].name);
    //     cin >> arr[i].num;
    //     cin >> arr[i].grade;   
    // }
    arr[0] = {"Charlie", 105, 5.0};
    arr[1] = {"Alice", 101, 4.5};
    arr[2] = {"David", 102, 4.1};
    arr[3] = {"Bob", 103, 3.9};
    arr[4] = {"Eve", 104, 4.8};

    sort((arr->name).begin(), (arr->name).end());

    for (size_t i = 0; i < 5; i++) cout << arr[i].name << " " << arr[i].num << endl;
    
    return 0;
}


