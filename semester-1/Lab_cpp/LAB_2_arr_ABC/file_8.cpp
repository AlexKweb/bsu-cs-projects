#include <iostream>
#include <vector>

using namespace std;

int main() {
    
    vector<int> arr = {10, 2, 33, 4, 6, 77, 8};
    //vector<int> arr = {10, 2, 33, 4, 6, 77, 8};
    //vector<int> arr1 = {10, 2, 33, 4, 6, 77, 8};
    int A = arr.size();
    int b = 10;
    vector<int> result = {};

    for (size_t i = 0; i < A; i++){
        if(arr[i] < b)
        result.push_back(arr[i]);
    }
    for (size_t i = 0; i < A; i++){
        if(arr[i] == b)
        result.push_back(b);
    }
    for (size_t i = 0; i < A; i++){
        if(arr[i] > b)
        result.push_back(arr[i]);
    }
    for (size_t i = 0; i < A; i++)
    {
        cout << " " << result[i]; 
    }
    
    return 0;
}
