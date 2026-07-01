#include <iostream>
#include <vector>

using namespace std;

int main() {
    
    vector<int> arr1 = {0, 2, 3, 4, 6, 7, 8};
    vector<int> arr2 = {1, 2, 4, 5, 6, 6, 7, 10};

    int amountElement = 0;

    for (size_t i = 0, j = 0; i + j < arr1.size() + arr2.size();)
    {
        if (arr1[i] == arr2[j])
        {
            amountElement++;
            i++;
            j++;
        }else if (arr1[i] > arr2[j] && i < arr1.size()){
            if (j < arr2.size() - 1) j++;
            else i++;
        } else{
            i++;
        }
    }
    cout << "==>" << amountElement;
    

    return 0;
}
