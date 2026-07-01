#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N;
    cin >> N;

    vector<int> arr(N);
    for (int i = 0; i < N; i++) cin >> arr[i];

    int totalRemoved = 0;

    bool changed = true;
    while (changed) {
        changed = false;
        int i = 0;
        while (i + 2 < arr.size()) {
            if (arr[i] == arr[i+1] && arr[i+1] == arr[i+2]) {
                int val = arr[i];
                int j = i;
                while (j < arr.size() && arr[j] == val) {
                    arr.erase(arr.begin() + j);
                    totalRemoved++;
                }
                changed = true;
                i = 0;
            } else {
                i++;
            }
        }
    }

    cout << totalRemoved << endl;
    return 0;
}
    