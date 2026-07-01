#include <iostream>
using namespace std;

bool canFormPalindrome(int n) {
    int arr[10] = {0};

    while (n > 0) {
        int d = n % 10;
        arr[d]++;
        n /= 10;
    }

    int odd = 0;
    for (int i = 0; i < 10; i++) {
        if (arr[i] % 2 != 0) odd++;
    }

    return odd <= 1;
}

int main() {
    int n;
    cout << "Enter: ";
    cin >> n;

    if (canFormPalindrome(n)) {
        cout << "may." << endl;
    } else {
        cout << "not may." << endl;
    }

    return 0;
}
