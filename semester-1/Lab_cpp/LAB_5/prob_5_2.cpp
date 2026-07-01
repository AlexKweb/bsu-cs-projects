#include <iostream>     
#include <string>     
#include <algorithm>
#include <iomanip> // Required for setw
#include <fstream>


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
    // arr[0] = {"Charlie", 7, 5.0};
    // arr[1] = {"Alice", 7, 4.5};
    // arr[2] = {"David", 5, 4.1};
    // arr[3] = {"Bob", 6, 3.9};
    // //arr[4] = {"Eve", 5, 4.8};
    // arr[4] = {"Bob", 6, 3.9};
    
    ifstream fin("list_of_students.txt");
    if (!fin.is_open())
    {
        cout << "Error of openning file list_...s.txt";
    }
    
    for (size_t i = 0; i < N; i++)
    {
        fin >> arr[i].name >> arr[i].num >> arr[i].grade;
    }


    std::sort(arr, arr + N, [](const Student& a, const Student& b) {
        return a.name < b.name;
    });

    cout << endl << " ==>  Sort(name):" << endl;
    for (size_t i = 0; i < N; i++) {
        cout << "\t" << std::left << std::setw(8) << arr[i].name << " " << arr[i].num << "   " << arr[i].grade << endl;
    }
//--------------
    string nameFind = "Bob";

    auto range = std::equal_range(
    arr, arr + N,
    Student{nameFind, 0, 0},
    [](const Student& a, const Student& b){
        return a.name < b.name;
    }
);

cout << endl << " ==>  Find("<< nameFind <<  "):" << endl;

if (range.first == range.second) {
    cout << "\n\t not Find:\n";
} else {
    for (auto it = range.first; it != range.second; ++it) {
        cout << "\t" << std::left << std::setw(8)
             << it->name << " " << it->num << "   " << it->grade << endl;
    }
}

    

    // for (size_t i = 0; i < N; i++)
    // {
    //     if (arr[i].name == nameFind)
    //     {
    //         cout << endl << " ==>  Find(name):" << endl;
    //         cout << "\t" << std::left << std::setw(8) << arr[i].name << " " << arr[i].num << "   " << arr[i].grade << endl;
    //         break;
    //     }  
    // }
//---------------
    std::sort(arr, arr + N, [](const Student& a, const Student& b) {
        if (a.num != b.num)
        {
        return a.num < b.num;
        }else
        return a.name < b.name;
    });

    cout << endl << " ==>  Sort(group + name):" << endl;
    for (size_t i = 0; i < N; i++) {
        cout << "\t" << std::left << std::setw(8) << arr[i].name << " " << arr[i].num << "   " << arr[i].grade << endl;
    }
//---------------
    vector<float> marks;

    float sum = 0.0f; 
    int counter = 0;
    cout << endl << " ==>  Average by group:" << endl;

      for (size_t i = 0; i < N; ++i) {
        
        if (i > 0 && arr[i].num != arr[i-1].num) {
            if (counter > 0) {
                float average = sum / counter;
                marks.push_back(average);
                cout << "\t" << std::left << std::setw(2) << arr[i-1].num << " group avg: " << average << endl;
            }
            
            sum = arr[i].grade;
            counter = 1;   
        } else {
            sum += arr[i].grade;
            counter++;
        }
    }
        cout << "\t" << std::left << std::setw(2) << arr[N-1].num << " group avg: " << sum/counter<< endl;
    
    for (size_t i = 0; i < N; i++) {
    }

    delete[] arr;
    arr = nullptr;

    return 0;
}


