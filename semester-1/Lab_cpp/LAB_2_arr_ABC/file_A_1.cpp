#include <iostream>
#include <vector>

using namespace std;

int main() {
    
    vector<int> arr = {2,33,4,6,22,1,1,33,4,6,4,72,43,6,99};
    int b = 10;

    vector<int> arrFirstPart = {};
    vector<int> arrSecondPard = {};

    int size = arr.size();
    for (size_t i = 0; i < size; i++)
    {
        if (arr[i] <= b){
            arrFirstPart.push_back(arr[i]);
        }else {
            arrSecondPard.push_back(arr[i]);
        }
    }
    for (size_t i = 0; i < arrSecondPard.size(); i++){
        arrFirstPart.push_back(arrSecondPard[i]);
    };
    cout << "==> ";
    for (size_t i = 0; i < size; i++){
        cout << " " << arrFirstPart[i];
    };
    

    return 0;
}
