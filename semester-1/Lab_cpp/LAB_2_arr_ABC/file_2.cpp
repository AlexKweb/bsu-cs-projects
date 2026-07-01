#include <iostream>
#include <vector>

using namespace std;

int main() {
    
    vector<int> arr1 = {1, 3, 5, 6, 7, 7, 7};
    vector<int> arr2 = {1, 2, 5, 7, 7, 8, 9};

    vector<int> amountElement = {};

    int k = 0;
    for (size_t i = 0, j = 0; i + j < arr1.size() + arr2.size();)
    {
        if (arr1[i] == arr2[j])
        {
            amountElement.push_back(arr1[i]);
            i++;
            j++;
        }else if (arr1[i] > arr2[j] && i < arr1.size()){
            j++;
        } else if(j < arr2.size()){
            i++;
        } else{
            j++;
        }
    }

    cout << "==>" ;

    for (size_t i = 0; i < amountElement.size(); i++)
    {
            cout <<  " " << amountElement[i];
    }
    
    

    return 0;
}
