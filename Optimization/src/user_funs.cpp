/*********************************************
Kod stanowi uzupe�nienie materia��w do �wicze�
w ramach przedmiotu metody optymalizacji.
Kod udost�pniony na licencji CC BY-SA 3.0
Autor: dr in�. �ukasz Sztangret
Katedra Informatyki Stosowanej i Modelowania
Akademia G�rniczo-Hutnicza
Data ostatniej modyfikacji: 19.09.2023
*********************************************/

#include "user_funs.h"

matrix ff0T(matrix x, matrix ud1, matrix ud2) {
    matrix y;
    y = pow(x(0) - ud1(0), 2) + pow(x(1) - ud1(1), 2);
    return y;
}

matrix ff0R(matrix x, matrix ud1, matrix ud2) {
    matrix y;
    matrix Y0 = matrix(2, 1), MT = matrix(2, new double[2]{m2d(x), 0.5});
    matrix *Y = solve_ode(df0, 0, 0.1, 10, Y0, ud1, MT);
    int n = get_len(Y[0]);
    double teta_max = Y[1](0, 0);
    for (int i = 1; i < n; ++i)
        if (teta_max < Y[1](i, 0))
            teta_max = Y[1](i, 0);
    y = abs(teta_max - m2d(ud1));
    Y[0].~matrix();
    Y[1].~matrix();
    return y;
}

matrix df0(double t, matrix Y, matrix ud1, matrix ud2) {
    matrix dY(2, 1);
    double m = 1, l = 0.5, b = 0.5, g = 9.81;
    double I = m * pow(l, 2);
    dY(0) = Y(1);
    dY(1) = ((t <= ud2(1)) * ud2(0) - m * g * l * sin(Y(0)) - b * Y(1)) / I;
    return dY;
}

matrix lab1_fun(matrix x, matrix ud1, matrix ud2) {
    matrix fx(1, 1);
    double y = -cos(0.1 * m2d(x)) * exp(-pow((0.1 * m2d(x) - 2 * M_PI), 2)) + 0.002 * pow(0.1 * m2d(x), 2);
    fx(0) = y;
    return fx;
}

matrix df1(double t, matrix Y, matrix ud1, matrix ud2)
{
	matrix dY(3, 1);

	double Va = Y(0);
	double Vb = Y(1);
	double Tb = Y(2);

	double Pa = ud1(0);
	double Ta = ud1(1);
	double Pb = ud1(2);
	double Db = ud1(3);
	double F_in = ud1(4);
	double T_in = ud1(5);
	double a = ud1(6);
	double b = ud1(7);

	double FaOut = 0;
	double FbOut = 0;

	if (Va > 0)
	{
		FaOut = a * b * m2d(ud2) * sqrt(2 * EARTH_ACCELERATION * (Va / Pa));
	}
		
	if (Vb > 0)
	{
		FbOut = a * b * Db * sqrt(2 * EARTH_ACCELERATION * (Vb / Pb));
	}

	double dTb_dt = FaOut / Vb * (Ta - Tb) + F_in / Vb * (T_in - Tb);

	if ((Va + FaOut) >= 0)
	{
		dY(0) = -FaOut;
	}
	else
	{
		dY(0) = -Va;
	}

	if ((Vb + FbOut) >= 0)
	{
		dY(1) = FaOut + F_in - FbOut;
	}
	else
	{
		dY(1) = -Vb;
	}
	
	if (Vb > 0)
	{
		dY(2) = dTb_dt;
	}
	else
	{
		dY(2) = 0;
	}

	return dY;
}

// Tylko Lagrange i Fib
// Do wykresu uzyc x tego ktory wyliczymy w tej funkcji
// Do 31 pazdziernika, jedna osoba oddaje sprawko. 
matrix f1R(matrix x, matrix ud1, matrix ud2)
{
	matrix y(1,1);
	matrix y0 = matrix(3, new double[3]{ 5, 1, 20 });

	matrix* y_ptr = solve_ode(df1, 0, 1, 2000, y0, ud1, x);

	int n = get_len(y_ptr[0]);

	double max = y_ptr[1](0, 2);

	for (int i = 0; i < n; i++)
		if (y_ptr[1](i, 2) > max)
			max = y_ptr[1](i, 2);

	//cout << std::setprecision(20) << "Max = " << max << endl;

	y(0) = abs(max - 50.0);

	y_ptr[0].~matrix();
	y_ptr[1].~matrix();

	return y;
}
