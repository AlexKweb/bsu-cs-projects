#include <iostream>
#include <vector>

using namespace std;

int main() {
    
    vector<int> arr1 = {7, 7, 8, 20};
    vector<int> arr2 = {7, 8, 9, 10, 11};

    int q = 16;

    int min = 100;

    int tempArr[2];
    for (size_t i = 0, j = 0; i + j < arr1.size() + arr2.size();)
    {
        if (min > abs(q - (arr1[i] + arr2[j])))
        {
            min = abs(q - (arr1[i] + arr2[j]));
        }
        if (arr1[i] == arr2[j] )
        {
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

    cout  << " ==> " << min;

    return 0;
}
