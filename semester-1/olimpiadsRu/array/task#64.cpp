
#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N;
    cin >> N;
    vector<int> arr(N);

    for (int i = 0; i < N; i++) {
        cin >> arr[i];
    }

    for (int i = 0; i < N; i ++) {
        if (arr[i] % 2 == 0) 
        {
            cout << arr[i] << " ";
        }
    }

    return 0;
}
