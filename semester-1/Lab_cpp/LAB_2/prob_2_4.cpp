#include <iostream>
#include <vector>
using namespace std;

int funcB(int matrix[5][5],int row, int column, int&b, int&c, int&d){
    int toggle1 = false;
    int toggle2 = false;

    for (size_t i = column + 1; i < 5; i++)
    {
        if(matrix[row][i] == b){toggle1 = i;}
    }
    for (size_t i = row + 1; i < 5; i++)
    {
        if(matrix[i][column] == c){toggle2 = i;}
    }
    if (toggle1 && toggle2)
    {
        if (matrix[toggle2][toggle1] == d)
        {
            return 1;
        }
        
    }
    
    return 0;
}

bool outRez(){
    cout << "Rectangle - true.\n";
    return 0;
}

int main() {
    int matrix[5][5] = {
    {7, 2, 9, 1, 5},
    {4, 8, 3, 6, 2},
    {1, 5, 7, 4, 8},
    {9, 3, 2, 5, 6},
    {2, 6, 4, 8, 3}
};
    // cout << "Enter matrix 5x5:\n";
    // for(int i=0;i<5;i++)
    //     for(int j=0;j<5;j++)
    //         cin >> matrix[i][j];

    int a, b, c, d;
    cout << "Enter 4 top of rec: ";
    cin >> a >> b >> c >> d;

    for (size_t i = 0; i < 5; i++){
        for (size_t j = 0; j < 5; j++){
            if (matrix[i][j] == a) {
                if(funcB(matrix, i, j, b, c, d)){return outRez();}
                if(funcB(matrix, i, j, b, d, c)){return outRez();}
                if(funcB(matrix, i, j, c, b, d)){return outRez();}
                if(funcB(matrix, i, j, c, d, b)){return outRez();}
                if(funcB(matrix, i, j, c, b, d)){return outRez();}
                if(funcB(matrix, i, j, c, b, d)){return outRez();}
            } 
            if (matrix[i][j] == b) {
                swap(a,b);
                if(funcB(matrix, i, j, b, c, d)){return outRez();}
                if(funcB(matrix, i, j, b, d, c)){return outRez();}
                if(funcB(matrix, i, j, c, b, d)){return outRez();}
                if(funcB(matrix, i, j, c, d, b)){return outRez();}
                if(funcB(matrix, i, j, c, b, d)){return outRez();}
                if(funcB(matrix, i, j, c, b, d)){return outRez();}
                swap(a,b);
            } 
            if (matrix[i][j] == c) {
                swap(a,c);
                if(funcB(matrix, i, j, b, c, d)){return outRez();}
                if(funcB(matrix, i, j, b, d, c)){return outRez();}
                if(funcB(matrix, i, j, c, b, d)){return outRez();}
                if(funcB(matrix, i, j, c, d, b)){return outRez();}
                if(funcB(matrix, i, j, c, b, d)){return outRez();}
                if(funcB(matrix, i, j, c, b, d)){return outRez();}
                swap(a,c);
            } if (matrix[i][j] == d) {
                swap(a,d);
                if(funcB(matrix, i, j, b, c, d)){return outRez();}
                if(funcB(matrix, i, j, b, d, c)){return outRez();}
                if(funcB(matrix, i, j, c, b, d)){return outRez();}
                if(funcB(matrix, i, j, c, d, b)){return outRez();}
                if(funcB(matrix, i, j, c, b, d)){return outRez();}
                if(funcB(matrix, i, j, c, b, d)){return outRez();}
                swap(a,d);
            } 
        }
    }
    cout << "Rectangle - false.\n";
    return 0;
}


/*

   #include <iostream>
using namespace std;

int main() {
    int matrix[5][5] = {
        {7, 2, 9, 1, 5},
        {4, 8, 3, 6, 2},
        {1, 5, 7, 4, 8},
        {9, 3, 2, 5, 6},
        {2, 6, 4, 8, 3}
    };

    int a, b, c, d;
    cout << "Enter 4 numbers: ";
    cin >> a >> b >> c >> d;

    // координаты всех подходящих значений
    int ax[10], ay[10], ac = 0;
    int bx[10], by[10], bc = 0;
    int cx[10], cy[10], cc = 0;
    int dx[10], dy[10], dc = 0;

    // ищем позиции всех четырёх чисел
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++) {
            if (matrix[i][j] == a) { ax[ac] = i; ay[ac] = j; ac++; }
            if (matrix[i][j] == b) { bx[bc] = i; by[bc] = j; bc++; }
            if (matrix[i][j] == c) { cx[cc] = i; cy[cc] = j; cc++; }
            if (matrix[i][j] == d) { dx[dc] = i; dy[dc] = j; dc++; }
        }

    // перебираем все комбинации
    for (int i1 = 0; i1 < ac; i1++)
        for (int i2 = 0; i2 < bc; i2++)
            for (int i3 = 0; i3 < cc; i3++)
                for (int i4 = 0; i4 < dc; i4++) {

                    int r[4] = { ax[i1], bx[i2], cx[i3], dx[i4] };
                    int c0[4] = { ay[i1], by[i2], cy[i3], dy[i4] };

                    int minR = r[0], maxR = r[0];
                    int minC = c0[0], maxC = c0[0];

                    // ищем min/max строк и столбцов
                    for (int k = 1; k < 4; k++) {
                        if (r[k] < minR) minR = r[k];
                        if (r[k] > maxR) maxR = r[k];
                        if (c0[k] < minC) minC = c0[k];
                        if (c0[k] > maxC) maxC = c0[k];
                    }

                    // проверяем условие прямоугольника
                    bool ok = true;

                    if (minR == maxR || minC == maxC) ok = false;

                    for (int k = 0; k < 4; k++) {
                        if (!(r[k] == minR || r[k] == maxR)) ok = false;
                        if (!(c0[k] == minC || c0[k] == maxC)) ok = false;
                    }

                    if (ok) {
                        cout << "Rectangle - true.\n";
                        return 0;
                    }
                }

    cout << "Rectangle - false.\n";
    return 0;
}

*/

