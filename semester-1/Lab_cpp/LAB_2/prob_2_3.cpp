#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


int main() {
    int n, x;
    cout << "N: ";
    cin >> n;
    vector<int> arr(n);
    cout << "Enter array: ";
    for (int i = 0; i < n; i++) cin >> arr[i];
    cout << "What find: ";
    cin >> x;
    
    
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
