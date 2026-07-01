#include <iostream>
#include <vector>

using namespace std;

int main() {
    
    vector<int> arr1 = {0, 2, 3, 4, 6, 7, 8};
    vector<int> arr2 = {1, 2, 4, 5, 6, 6, 7, 10};
    vector<int> arr3 = {1, 3, 4, 5, 6, 6, 7, 10};

    int Element = 0;
    int minElement = -1;

    int A = arr1.size();
    int B = arr2.size();
    int C = arr3.size();
    int sum = A+B+C;

    for (size_t i = 0, j = 0, k = 0; i + j + k < sum;)
    {
        if (arr1[i] == arr2[j] && arr2[j] == arr3[k])
        {
            minElement = arr1[i];
            break;
        }
        Element = min({arr1[i], arr2[j], arr3[k]});
        if(arr1[i] == Element) i++;
        if(arr2[j] == Element) j++;
        if(arr3[k] == Element) k++;
    }
    cout << "==> " << minElement;
    return 0;
}
