#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Pos {
    int row;
    int col;
};

vector<Pos> collectPositions(int matrix[5][5], int target) {
    vector<Pos> result;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (matrix[i][j] == target) result.push_back({i, j});
    return result;
}

bool isRectangle(const Pos& p1, const Pos& p2, const Pos& p3, const Pos& p4) {
    int minRow = p1.row, maxRow = p1.row;
    int minCol = p1.col, maxCol = p1.col;

    auto update = [&](const Pos& p) {
        if (p.row < minRow) minRow = p.row;
        if (p.row > maxRow) maxRow = p.row;
        if (p.col < minCol) minCol = p.col;
        if (p.col > maxCol) maxCol = p.col;
    };

    update(p2);
    update(p3);
    update(p4);

    if (minRow == maxRow || minCol == maxCol) return false;

    auto fitsCorner = [&](const Pos& p) {
        bool rowOnEdge = (p.row == minRow || p.row == maxRow);
        bool colOnEdge = (p.col == minCol || p.col == maxCol);
        return rowOnEdge && colOnEdge;
    };

    auto samePos = [](const Pos& lhs, const Pos& rhs) {
        return lhs.row == rhs.row && lhs.col == rhs.col;
    };

    if (samePos(p1, p2) || samePos(p1, p3) || samePos(p1, p4) ||
        samePos(p2, p3) || samePos(p2, p4) || samePos(p3, p4)) {
        return false;
    }

    return fitsCorner(p1) && fitsCorner(p2) && fitsCorner(p3) && fitsCorner(p4);
}

bool existsRectangle(const vector<Pos>& a, const vector<Pos>& b, const vector<Pos>& c, const vector<Pos>& d) {
    for (const auto& p1 : a)
        for (const auto& p2 : b)
            for (const auto& p3 : c)
                for (const auto& p4 : d)
                    if (isRectangle(p1, p2, p3, p4)) return true;
    return false;
}

int main(int argc, char* argv[]) {
    const string inputPath = (argc > 1) ? argv[1] : "input.txt";
    ifstream fin(inputPath);
    if (!fin.is_open()) {
        cerr << "Failed to open input file: " << inputPath << endl;
        return 1;
    }

    int matrix[5][5];
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++) {
            if (!(fin >> matrix[i][j])) {
                cerr << "Not enough values for a 5x5 matrix in input file." << endl;
                return 1;
            }
        }

    int a, b, c, d;
    if (!(fin >> a >> b >> c >> d)) {
        cerr << "Failed to read four target values." << endl;
        return 1;
    }

    vector<Pos> posA = collectPositions(matrix, a);
    vector<Pos> posB = collectPositions(matrix, b);
    vector<Pos> posC = collectPositions(matrix, c);
    vector<Pos> posD = collectPositions(matrix, d);

    bool hasRectangle = existsRectangle(posA, posB, posC, posD);
    cout << (hasRectangle ? "Rectangle - true.\n" : "Rectangle - false.\n");
    return 0;
}
