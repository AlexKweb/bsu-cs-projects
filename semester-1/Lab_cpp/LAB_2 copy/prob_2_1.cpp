#include <iostream>
#include <fstream>
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

int main(int argc, char* argv[]) {
    const string inputPath = (argc > 1) ? argv[1] : "input.txt";
    ifstream fin(inputPath);
    if (!fin.is_open()) {
        cerr << "Failed to open input file: " << inputPath << endl;
        return 1;
    }

    int n;
    if (!(fin >> n)) {
        cerr << "Failed to read number from input file." << endl;
        return 1;
    }

    if (canFormPalindrome(n)) {
        cout << "may." << endl;
    } else {
        cout << "not may." << endl;
    }

    return 0;
}
