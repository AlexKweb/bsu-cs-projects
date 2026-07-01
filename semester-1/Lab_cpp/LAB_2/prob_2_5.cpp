#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cout <<  "Enter size of array: ";
    cin >> n;
    vector<vector<int>> m(n, vector<int>(n));
    cout << endl << "Enter " << n*n << " elements: " << endl;
    for (int i=0; i<n; i++)
        for (int j=0; j<n; j++) 
        cin >> m[i][j];
    for (int i=0; i<n; i++)
        for (int j=0; j<n; j++) {
            int elem = m[i][j];
            bool minInRow = true, maxInCol = true;
            for (int k=0; k<n; k++)
                if (m[i][k] < elem) minInRow = false;
            for (int k=0; k<n; k++)
                if (m[k][j] > elem) maxInCol = false;
            if (minInRow && maxInCol)
                cout << "Element: " << elem << " in pos (" << i << "," << j << ") .\n";
        }
    return 0;
}