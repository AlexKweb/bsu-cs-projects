#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;


int main(int argc, char* argv[]) {
    const string inputPath = (argc > 1) ? argv[1] : "input.txt";
    ifstream fin(inputPath);
    if (!fin.is_open()) {
        cerr << "Failed to open input file: " << inputPath << endl;
        return 1;
    }

    int n, x;
    if (!(fin >> n)) {
        cerr << "Failed to read array size." << endl;
        return 1;
    }
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        if (!(fin >> arr[i])) {
            cerr << "Not enough elements in input file." << endl;
            return 1;
        }
    }
    if (!(fin >> x)) {
        cerr << "Failed to read search value." << endl;
        return 1;
    }
    
    
    //sort(arr.begin(), arr.end());  

    int left = 0;
    int right = n - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == x) {
            cout << "Found: " << arr[mid] << ", on pos: " << mid;
            return 0;
        }
        if (arr[mid] < x)
            left = mid + 1;
        else
            right = mid - 1;
    }
    cout << "Not Found";
    return 0;

    
}
