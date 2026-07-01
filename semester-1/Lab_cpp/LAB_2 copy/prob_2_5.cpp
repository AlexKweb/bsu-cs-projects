#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {
    const string inputPath = (argc > 1) ? argv[1] : "input.txt";
    ifstream fin(inputPath);
    if (!fin.is_open()) {
        cerr << "Failed to open input file: " << inputPath << endl;
        return 1;
    }

    int n;
    if (!(fin >> n)) {
        cerr << "Failed to read matrix size." << endl;
        return 1;
    }
    vector<vector<int>> m(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (!(fin >> m[i][j])) {
                cerr << "Not enough elements in input file." << endl;
                return 1;
            }
        }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            int elem = m[i][j];
            bool minInRow = true, maxInCol = true;
            for (int k = 0; k < n; k++)
                if (m[i][k] < elem) minInRow = false;
            for (int k = 0; k < n; k++)
                if (m[k][j] > elem) maxInCol = false;
            if (minInRow && maxInCol)
                cout << "Element: " << elem << " in pos (" << i << "," << j << ") .\n";
        }
    return 0;
}
