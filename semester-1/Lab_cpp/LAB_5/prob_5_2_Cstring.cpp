#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <vector>

using namespace std;

struct Student
{
    char name[32];
    int num;
    double grade;
};

// ---------- SORT BY NAME
void sortByName(Student* arr, int N) {
    sort(arr, arr + N, [](const Student& a, const Student& b) {
        return strcmp(a.name, b.name) < 0;
    });

    cout << "\n ==> Sort(name):\n";
    for (int i = 0; i < N; i++) {
        cout << "\t" << left << setw(8)
             << arr[i].name << " " << arr[i].num << "   " << arr[i].grade << endl;
    }
}

// ---------- SEARCH
void searchName(Student* arr, int N) {
    char nameFind[32];

    cout << "Enter name to search: ";
    cin >> setw(31) >> nameFind;

    Student key{};
    strcpy(key.name, nameFind);

    auto cmp = [](const Student& a, const Student& b) {
        return strcmp(a.name, b.name) < 0;
    };

    auto range = equal_range(arr, arr + N, key, cmp);

    cout << "\n ==> Search(" << nameFind << "):\n";

    if (range.first == range.second) {
        cout << "\tNot found.\n";
    } else {
        for (auto it = range.first; it != range.second; ++it) {
            cout << "\t" << left << setw(8)
                 << it->name << " " << it->num << "   " << it->grade << endl;
        }
    }
}

// ---------- SORT BY GROUP + NAME 
void sortByGroupName(Student* arr, int N) {
    sort(arr, arr + N, [](const Student& a, const Student& b) {
        if (a.num != b.num)
            return a.num < b.num;
        return strcmp(a.name, b.name) < 0;
    });

    cout << "\n ==> Sort(group + name):\n";
    for (int i = 0; i < N; i++) {
        cout << "\t" << left << setw(8)
             << arr[i].name << " " << arr[i].num << "   " << arr[i].grade << endl;
    }
}

// ---------- AVERAGE BY GROUP 
void averageByGroup(Student* arr, int N) {
    cout << "\n ==> Average by group:\n";

    double sum = 0;
    int counter = 0;

    for (int i = 0; i < N; ++i) {
        if (i > 0 && arr[i].num != arr[i - 1].num) {
            cout << "\t" << arr[i - 1].num << " group avg: "
                 << (sum / counter) << endl;

            sum = arr[i].grade;
            counter = 1;
        } else {
            sum += arr[i].grade;
            counter++;
        }
    }

    cout << "\t" << arr[N - 1].num << " group avg: "
         << (sum / counter) << endl;
}

int main() {
    int N = 5;
    Student* arr = new Student[N];

    ifstream fin("list_of_students.txt");
    if (!fin.is_open()) {
        cout << "Error opening file!\n";
        return 1;
    }

    for (int i = 0; i < N; i++) {
        fin >> setw(31) >> arr[i].name >> arr[i].num >> arr[i].grade;
    }

    int choice;

    do {
        cout << "\n========== MENU ==========\n";
        cout << "1 - Sort by NAME\n";
        cout << "2 - Search by NAME\n";
        cout << "3 - Sort by GROUP + NAME\n";
        cout << "4 - Average by GROUP\n";
        cout << "0 - EXIT\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
            case 1:
                sortByName(arr, N);
                break;

            case 2:
                sortByName(arr, N);
                searchName(arr, N);
                break;

            case 3:
                sortByGroupName(arr, N);
                break;

            case 4:
                sortByGroupName(arr, N);
                averageByGroup(arr, N);
                break;

            case 0:
                cout << "Exit.\n";
                break;

            default:
                cout << "Invalid option.\n";
        }

    } while (choice != 0);

    delete[] arr;
    return 0;
}
