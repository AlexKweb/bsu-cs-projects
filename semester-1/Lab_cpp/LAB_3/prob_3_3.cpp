#include <iostream>


double kramer( double matrix[2][2])
    {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

int main()
{
    double a, b, c, d, e, f;

    std::cout << "Enter coefficients a,b and free term e(ax + by = e) : "  << '\n';
    std::cin >> a >> b >> e;

    std :: cout << "Enter coefficients c, d and free term f(cx + dy = f) : "  << '\n';
    std::cin >> c >> d >> f;

    double D_matrix[2][2] = { {a,b}, {c,d} };
    double D = kramer(D_matrix);


    if (D == 0)
    {
        if (kramer({{e,b}, {f,d}}) == 0 && kramer({{a,e}, {c,f}}) == 0)
            std::cout << "There are infinitely many solutions!" << '\n';
        else
            std::cout << "There are no solutions!" << '\n';
    }
    else
    {
        double Dx_matrix[2][2] = { {e,b}, {f,d} };
        double Dy_matrix[2][2] = { {a,e}, {c,f} };

        double Dx = kramer(Dx_matrix);
        double Dy = kramer(Dy_matrix);

        double x = Dx / D;
        double y = Dy / D;

        std::cout << "Solution of the system: " << '\n';
        std::cout << "X = " << x << '\n';
        std :: cout << "Y = " << y << '\n';

        return 0;


    }
}