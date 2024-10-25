/*********************************************
Kod stanowi uzupe�nienie materia��w do �wicze�
w ramach przedmiotu metody optymalizacji.
Kod udost�pniony na licencji CC BY-SA 3.0
Autor: dr in�. �ukasz Sztangret
Katedra Informatyki Stosowanej i Modelowania
Akademia G�rniczo-Hutnicza
Data ostatniej modyfikacji: 19.09.2023
*********************************************/

#include "opt_alg.h"

void lab0();
void lab1();
void lab2();
void lab3();
void lab4();
void lab5();
void lab6();

int main()
{
	try
	{
		lab1();
	}
	catch (string EX_INFO)
	{
		cerr << "ERROR:\n";
		cerr << EX_INFO << endl << endl;
	}
	system("pause");
	return 0;
}

void lab0()
{
	// Funkcja testowa
	double epsilon = 1e-2;
	int Nmax = 10000;
	matrix lb(2, 1, -5), ub(2, 1, 5), a(2, 1);
	solution opt;
	a(0) = -1;
	a(1) = 2;
	opt = MC(ff0T, 2, lb, ub, epsilon, Nmax, a);
	cout << opt << endl << endl;
	solution::clear_calls();

	// Wahadlo
	Nmax = 1000;
	epsilon = 1e-2;
	lb = 0;
	ub = 5;
	double teta_opt = 1;
	opt = MC(ff0R, 1, lb, ub, epsilon, Nmax, teta_opt);
	cout << opt << endl << endl;
	solution::clear_calls();

	// Zapis symulacji do pliku csv
	matrix Y0 = matrix(2, 1), MT = matrix(2, new double[2] { m2d(opt.x), 0.5 });
	matrix* Y = solve_ode(df0, 0, 0.1, 10, Y0, NAN, MT);
	ofstream Sout("../Results/symulacja_lab0.csv");
	Sout << hcat(Y[0], Y[1]);
	Sout.close();
	Y[0].~matrix();
	Y[1].~matrix();
}

void lab1() 
{
	double epsilon = 1e-2;
	int Nmax = 10000;

	//double* result = expansion(lab1_fun, -100, 1, 1.01, Nmax); //to wypluje minimum lokalne, to po lewej
	double* result = expansion(lab1_fun, 50, 1, 1.01, Nmax); //to wypluje minimum globalne, to po prawej
	solution::clear_calls();
	cout << result[0] << ", " << result[1] << "\n";

	solution resultFib = fib(lab1_fun, result[0], result[1], 0.001);
	std::cout << resultFib << "\n";
	solution::clear_calls();

	solution resultLag = lag(lab1_fun, result[0], result[1], 1e-18, 1e-30, Nmax);
	std::cout << resultLag << "\n";
	solution::clear_calls();

	matrix ud1(8, 1);
	ud1(0) = 0.5; // Pa
	ud1(1) = 90.0; // Ta
	ud1(2) = 1.0; // Pb
	ud1(3) = 36.5665 / 100.0 / 100.0; // Db: cm2 -> m2
	ud1(4) = 10.0 / 1000.0; // F_in: l -> m3
	ud1(5) = 20.0; // T_in
	ud1(6) = 0.98; // a
	ud1(7) = 0.63; // b

	//solution realProblem = fib(f1R, -100, 100, 1e-2, ud1);
	//std::cout << realProblem << "\n";
	//solution::clear_calls();

	matrix x(1, 1);
	x(0) = 0.05;
	matrix result2 = f1R(x, ud1);

	std::cout << m2d(result2) << "\n";
}

void lab2() {}

void lab3() {}

void lab4() {}

void lab5() {}

void lab6() {}
