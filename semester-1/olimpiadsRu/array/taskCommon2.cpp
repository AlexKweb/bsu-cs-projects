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

    int K;
    cin >> K;

    //K = (K + N) % N;

    for (int i = 0; i < N; i++) {
        cout << arr[(i + N- K) % N] << " ";
    }
    cout << endl;

    return 0;
}