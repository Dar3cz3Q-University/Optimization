// Univeristy File

#include "pch.h"

#include "user_funs.h"

matrix ff0T(matrix x, matrix ud1, matrix ud2)
{
	matrix y;
	y = pow(x(0) - ud1(0), 2) + pow(x(1) - ud1(1), 2);
	return y;
}

matrix ff0R(matrix x, matrix ud1, matrix ud2)
{
	matrix y;
	matrix Y0 = matrix(2, 1), MT = matrix(2, new double[2] { m2d(x), 0.5 });
	matrix* Y = solve_ode(df0, 0, 0.1, 10, Y0, ud1, MT);
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

matrix df0(double t, matrix Y, matrix ud1, matrix ud2)
{
	matrix dY(2, 1);
	double m = 1, l = 0.5, b = 0.5, g = 9.81;
	double I = m * pow(l, 2);
	dY(0) = Y(1);
	dY(1) = ((t <= ud2(1)) * ud2(0) - m * g * l * sin(Y(0)) - b * Y(1)) / I;
	return dY;
}

matrix lab1_fun(matrix x, matrix ud1, matrix ud2)
{
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

	double FaOut = a * b * m2d(ud2) * sqrt(2 * EARTH_ACCELERATION * (Va / Pa));
	double FbOut = a * b * Db * sqrt(2 * EARTH_ACCELERATION * (Vb / Pb));
	double dTb_dt = FaOut / Vb * (Ta - Tb) + F_in / Vb * (T_in - Tb);

	dY(0) = -FaOut;
	dY(1) = FaOut + F_in - FbOut;
	dY(2) = dTb_dt;

	return dY;
}

matrix f1R(matrix x, matrix ud1, matrix ud2)
{
	matrix y(1, 1);
	matrix y0 = matrix(3, new double[3] { 5, 1, 20 });

	matrix* y_ptr = solve_ode(df1, 0, 1, 2000, y0, ud1, x);

	int n = get_len(y_ptr[0]);

	double max = y_ptr[1](0, 2);

	for (int i = 0; i < n; i++)
		if (y_ptr[1](i, 2) > max)
			max = y_ptr[1](i, 2);

	cout << std::setprecision(20) << "Max = " << max << endl;

	y(0) = abs(max - 50.0);

	y_ptr[0].~matrix();
	y_ptr[1].~matrix();

	return y;
}

matrix lab2_fun(matrix x, matrix ud1, matrix ud2)
{
	matrix y;

	y = pow(x(0), 2) + pow(x(1), 2) - cos(2.5 * M_PI * x(0)) - cos(2.5 * M_PI * x(1)) + 2;

	return y;
}

matrix df2(double t, matrix Y, matrix ud1, matrix ud2)
{
	matrix dY(2, 1);

	double alpha_t = Y(0);
	double omega_t = Y(1);

	double b = 0.5;
	double mr = 1.0;
	double mc = 5.0;
	double l = 1.0;

	double alpha_ref = ud1(0);
	double omega_ref = ud1(1);
	double k1 = ud2(0);
	double k2 = ud2(1);

	double I = 1.0 / 3.0 * mr * pow(l, 2) + mc * pow(l, 2);
	double Mt = k1 * (alpha_ref - alpha_t) + k2 * (omega_ref - omega_t);

	dY(0) = omega_t;
	dY(1) = (Mt - b * omega_t) / I;

	return dY;
}

matrix f2R(matrix x, matrix ud1, matrix ud2)
{
	matrix Y0(2, 1);
	matrix Yref(2, new double[2]{ M_PI, 0 });
	matrix* Y = solve_ode(df2, 0, 0.1, 100, Y0, Yref, x);

	double y = 0.0;
	int n = get_len(Y[0]);
	for (int i = 0; i < n; i++)
	{
		y += 10 * pow(Yref(0) - Y[1](i, 0), 2) +
			pow(Yref(1) - Y[1](i, 1), 2) +
			pow(x(0) * (Yref(0) - Y[1](i, 0)) + x(1) * (Yref(1) - Y[1](i, 1)), 2);
	}

	y *= 0.1;

	matrix result(1, 1);
	result(0) = y;

	Y[0].~matrix();
	Y[1].~matrix();

	return result;
}

matrix lab3_fun_help(matrix x, matrix ud1, matrix ud2)
{
	return sin(M_PI * sqrt(pow(x(0) / M_PI, 2) + pow(x(1) / M_PI, 2))) / (M_PI * sqrt(pow(x(0) / M_PI, 2) + pow(x(1) / M_PI, 2)));
}

matrix lab3_fun_outer(matrix x, matrix ud1, matrix ud2)
{
	matrix y = lab3_fun_help(x, ud1, ud2);

	// Sprawdzenie

	if (-x(0) + 1 > 0)
		y = y + ud2 * pow(-x(0) + 1, 2);

	if (-x(1) + 1 > 0)
		y = y + ud2 * pow(-x(1) + 1, 2);

	if (norm(x) - ud1 > 0)
		y = y + ud2 * pow(norm(x) - ud1, 2);

	return y;
}

matrix lab3_fun_inner(matrix x, matrix ud1, matrix ud2)
{
	matrix y = lab3_fun_help(x, ud1, ud2);

	// Sprawdzenie

	if (-x(0) + 1 >= 0)
		y = 1e10;
	else
		y = y - m2d(ud2) / (-x(0) + 1);

	if (-x(1) + 1 >= 0)
		y = 1e10;
	else
		y = y - m2d(ud2) / (-x(1) + 1);

	if (norm(x) - m2d(ud1) > 0)
		y = 1e10;
	else
		y = y - m2d(ud2) / (norm(x) - ud1);

	return y;
}

matrix df3(double t, matrix Y, matrix ud1, matrix ud2)
{
	matrix dY(4, 1);

	double C = 0.47;
	double rho = 1.2;
	double r = 12 * 0.01;
	double m = 0.6;
	double omega = ud2(0);

	double S = M_PI * pow(r, 2);
	double Dx = 0.5 * C * rho * S * Y(1) * abs(Y(1));
	double Dy = 0.5 * C * rho * S * Y(3) * abs(Y(3));
	double Fmx = rho * Y(3) * omega * M_PI * pow(r, 3);
	double Fmy = rho * Y(1) * omega * M_PI * pow(r, 3);

	dY(0) = Y(1);
	dY(1) = (-Dx - Fmx) / m;
	dY(2) = Y(3);
	dY(3) = (-m * EARTH_ACCELERATION - Dy - Fmy) / m;

	return dY;
}

matrix f3R(matrix x, matrix ud1, matrix ud2)
{
	matrix Y0(4, new double[4] {0, x(0), 100, 0 });
	matrix* Y = solve_ode(df3, 0, 0.01, 7, Y0, ud1, x(1));

	int n = get_len(Y[0]);
	int i0 = 0;
	int i50 = 0;
	matrix result;

	for (int i = 0; i < n; i++)
	{
		if (abs(Y[1](i, 2) - 50) < abs(Y[1](i50, 2) - 50))
			i50 = i;

		if (abs(Y[1](i, 2)) < abs(Y[1](i0, 2)))
			i0 = i;
	}

	result = -Y[1](i0, 0);

	if (abs(x(0)) - 10 > 0)
		result = result + ud2 * pow(abs(x(0)) - 10, 2); // ud2 to c

	if (abs(x(1)) - 15 > 0)
		result = result + ud2 * pow(abs(x(1)) - 15, 2);

	if (abs(Y[1](i50, 0) - 5) - 0.5 > 0)
		result = result + ud2 * pow(abs(Y[1](i50, 0) - 5) - 0.5, 2);

	Y[0].~matrix();
	Y[1].~matrix();

	return result;
}

matrix lab4_fun(matrix x, matrix ud1, matrix ud2)
{
	return pow((x(0) + 2 * x(1) - 7), 2) + pow((2 * x(0) + x(1) - 5), 2);
}

matrix fT4(matrix x, matrix ud1, matrix ud2)
{
	matrix y;

	if (isnan(ud2(0, 0)))
	{
		y = lab4_fun(x, ud1, ud2);
	}	
	else
	{
		y = fT4(ud2[0] + x * ud2[1]);
	}
		

	return y;
}

matrix lab4_grad(matrix x, matrix ud1, matrix ud2)
{
	matrix y(2, 1);

	y(0) = -34.0 + 10.0 * x(0) + 8.0 * x(1);
	y(1) = -38.0 + 8.0 * x(0) + 10.0 * x(1);

	return y;
}

matrix lab4_hes(matrix x, matrix ud1, matrix ud2)
{
	matrix y(2, 2);
	
	y(0, 0) = 10;
	y(0, 1) = 8;
	y(1, 0) = 8;
	y(1, 1) = 10;

	return y;
}

double sigmoid(matrix tetha, matrix x)
{
	matrix coef = trans(tetha) * x;
	return 1.0 / (1.0 + exp(-m2d(coef)));
}

matrix cost_function(matrix tetha, matrix ud1, matrix ud2)
{
	constexpr int m = 100;

	double sum = 0.0;
	for (int i = 0; i < m; i++)
		sum += ud2(i) * log(sigmoid(tetha, trans(get_row(ud1, i)))) + (1 - ud2(i)) * log(1 - sigmoid(tetha, trans(get_row(ud1, i))));

	sum = sum * -1.0 / m;
	return sum;
}

matrix cost_function_grad(matrix tetha, matrix ud1, matrix ud2)
{
	constexpr int m = 100;

	constexpr int n = 3;

	matrix result(n, 1);

	for (int j = 0; j < n; j++)
	{
		double sum = 0.0;
		for (int i = 0; i < m; i++)
			sum += (sigmoid(tetha, trans(get_row(ud1, i))) - ud2(i)) * ud1(i, j);

		result(j) = sum * 1.0 / m;
	}
	return result;
}
