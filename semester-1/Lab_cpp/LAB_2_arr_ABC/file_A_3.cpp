#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    //cin >> n;
    
    vector<int> a = {1, 3, 2, 5, 4, 6, 1};
    n = a.size();
    // for (int i = 0; i < n; i++) {
    //     cin >> a[i];
    // }
    
    int minCount = 0;
    int maxCount = 0;
    
    for (int i = 1; i < n - 1; i++) {
        if (a[i] < a[i-1] && a[i] < a[i+1]) {
            minCount++;
        }
        if (a[i] > a[i-1] && a[i] > a[i+1]) {
            maxCount++;
        }
    }
    
    cout << "min: " << minCount << endl;
    cout << "max: " << maxCount << endl;
    
    return 0;
}