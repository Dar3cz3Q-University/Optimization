#include "pch.h"

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

#include "RandomNumberGenerator.h"
#include "FileSaver.h"
#include "FileReaderFactory.h"
#include "StudentFileReader.h"

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
		lab4();
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
	double epsilon = 1e-20;
	double gamma = 1e-30;
	int Nmax = 10000;
	double a = 1.0 * 0.01 * 0.01; // 1 cm2 = 0.0001 m2
	double b = 100.0 * 0.01 * 0.01; // 100 cm2 = 0.01 m2

	matrix ud1(8, 1);
	ud1(0) = 0.5; // Pa
	ud1(1) = 90.0; // Ta
	ud1(2) = 1.0; // Pb
	ud1(3) = 36.5665 * 0.0001; // Db: cm2 -> m2
	ud1(4) = 10.0 * 0.001; // F_in: l -> m3
	ud1(5) = 20.0; // T_in
	ud1(6) = 0.98; // a
	ud1(7) = 0.63; // b

	solution resultFib = fib(f1R, a, b, epsilon, ud1);
	std::cout << resultFib << "\n";
	solution::clear_calls();

	{
		solution resultLag = lag(f1R, a, b, epsilon, gamma, Nmax, ud1);
		std::cout << resultLag << "\n";
		solution::clear_calls();
	}

	for (double alpha = 1.5; alpha <= 4.5; alpha += 1.5)
	{
		for (int i = 0; i < 100; i++)
		{
			// Expansion
			double x0 = RandomNumberGenerator::Get().Double(0, 100);
			double* expansionResult = expansion(lab1_fun, x0, 1, alpha, Nmax);
			SAVE_TO_FILE("expansion-" + std::to_string(alpha) + ".txt") << x0 << ";" << expansionResult[0] << ";" << expansionResult[1] << ";" << solution::f_calls << "\n";
			solution::clear_calls();

			// Fibonacci
			solution fibonacciResult = fib(lab1_fun, expansionResult[0], expansionResult[1], epsilon, Nmax);
			SAVE_TO_FILE("fibonacci-" + std::to_string(alpha) + ".txt") << fibonacciResult.x << ";" << fibonacciResult.y << ";" << solution::f_calls << "\n";
			solution::clear_calls();

			// Lagrange
			solution lagrangeResult = lag(lab1_fun, expansionResult[0], expansionResult[1], epsilon, gamma, Nmax);
			SAVE_TO_FILE("lagrange-" + std::to_string(alpha) + ".txt") << lagrangeResult.x << ";" << lagrangeResult.y << ";" << solution::f_calls << "\n";
			solution::clear_calls();

			delete[] expansionResult;
		}
	}

	solution fibonacciResult = fib(lab1_fun, -100, 100, epsilon, Nmax);
	std::cout << fibonacciResult << "\n";
	solution::clear_calls();

	solution lagrangeResult = lag(lab1_fun, -100, 100, epsilon, gamma, Nmax);
	std::cout << lagrangeResult << "\n";
	solution::clear_calls();

	solution realProblem = fib(f1R, -100, 100, 1e-2, ud1);
	std::cout << realProblem << "\n";
	solution::clear_calls();

	solution resultLag = lag(f1R, a, b, epsilon, gamma, Nmax, ud1);
	std::cout << resultLag << "\n";
	solution::clear_calls();

	matrix matFib = f1R(resultFib.x, ud1);
	std::cout << "Dla minimum z fibonacci, Da = " << resultFib.x << " cm^2, (max - 50) = " << m2d(matFib) << "\n";

	matrix matLag = f1R(resultLag.x, ud1);
	std::cout << "Dla minimum z lagrange, Da = " << resultLag.x << " cm^2, (max - 50) = " << m2d(matLag) << "\n";

	matrix y(1, 1);
	matrix y0 = matrix(3, new double[3] { 5.0, 1.0, 20.0 });

	matrix* simulationFib = solve_ode(df1, 0, 1, 2000, y0, ud1, resultFib.x);
	SAVE_TO_FILE("wynik_proj1_fib.txt") << simulationFib[1];

	matrix* simulationLag = solve_ode(df1, 0, 1, 2000, y0, ud1, resultLag.x);
	SAVE_TO_FILE("wynik_proj1_lag.txt") << simulationLag[1];
}

void lab2() 
{	
	double alpha_1 = 0.1;
	double alpha_2 = 1.1;
	double beta = 0.50;
	double epsilon = 1e-6;
	int Nmax = 2000;

	double s = 0.1;
	matrix sv(2, 1);
	sv(0) = s;
	sv(1) = s;

	for (int k = 1; k <= 3; k++) 
	{
		s = 0.1 * k;
		sv(0) = s;
		sv(1) = s;
		for (int i = 0; i < 100; i++)
		{
			matrix x(2, 1);
			x(0) = RandomNumberGenerator::Get().Double(-1.0, 1.0);
			x(1) = RandomNumberGenerator::Get().Double(-1.0, 1.0);

			SAVE_TO_FILE("args-" + std::to_string(k) + ".txt") << x(0) << ";" << x(1) << "\n";
			solution::clear_calls();
			
			// Hooke
			solution hookeResult = HJ(lab2_fun, x, s, alpha_1, epsilon, Nmax);
			SAVE_TO_FILE("hooke-" + std::to_string(k) + ".txt") << hookeResult.x(0) << ";" << hookeResult.x(1) << ";" << hookeResult.y(0) << ";" << solution::f_calls << "\n";
			solution::clear_calls();
			
			// Rosen
			solution rosenResult = Rosen(lab2_fun, x, sv, alpha_2, beta, epsilon, Nmax);
			SAVE_TO_FILE("rosen-" + std::to_string(k) + ".txt") << rosenResult.x(0) << ";" << rosenResult.x(1) << ";" << rosenResult.y(0) << ";" << solution::f_calls << "\n";
			solution::clear_calls();
		}
	}

	// liczenie po iteracjach - jest to pierwszy obliczony przypadek z tab1, dla s = 0.2
	s = 0.2;
	sv(0) = s;
	sv(1) = s;

	matrix x(2, 1);
	x(0) = 0.2388;
	x(1) = -0.74822;

	solution hookeResult = HJ(lab2_fun, x, s, alpha_1, epsilon, Nmax);
	solution::clear_calls();

	solution rosenResult = Rosen(lab2_fun, x, sv, alpha_2, beta, epsilon, Nmax);
	solution::clear_calls();


	// Symulacja
	{
		double alpha1 = 0.5; // for HJ
		double alpha2 = 1.3; // for Rosen
		double beta = 0.5; // for Rosen
		double epsilon = 1e-6;
		int Nmax = 10000;

		double s = 0.4;
		matrix sv(2, 1);
		sv(0) = s;
		sv(1) = s;

		matrix ud1(2, 1);
		ud1(0) = M_PI;
		ud1(1) = 0;

		matrix k(2, 1);
		k(0) = 1.0;
		k(1) = 1.0;

		matrix y0(2, 1);
		y0(0) = 0;
		y0(1) = 0;

		solution HJResult_sim = HJ(f2R, k, s, alpha1, epsilon, Nmax, ud1); // znajduje optymalne k1 k2 rozwiązanie funkcjonału Q(k1,k2)
		std::cout << "Simulation HJ result: " << HJResult_sim; 
		solution::clear_calls();

		solution RosenResult_sim = Rosen(f2R, k, sv, alpha2, beta, epsilon, Nmax, ud1); // znajduje optymalne k1 k2 rozwiązanie funkcjonału Q(k1,k2)
		std::cout << "Simulation Rosen result: " << RosenResult_sim;
		solution::clear_calls();

		matrix* simulationHooke = solve_ode(df2, 0, 0.1, 100, y0, ud1, HJResult_sim.x);
		SAVE_TO_FILE("simulation-HJ.txt") << simulationHooke[1];

		matrix* simulationRosen = solve_ode(df2, 0, 0.1, 100, y0, ud1, RosenResult_sim.x);
		SAVE_TO_FILE("simulation-Rosen.txt") << simulationRosen[1];
	}
}

void lab3() 
{
	double s = 0.5; 
	double alpha = 1.0; 
	double beta = 0.5; 
	double gamma = 2.0;
	double delta = 0.5; 
	
	matrix userData(5, 1);
	userData(0) = s;
	userData(1) = alpha;
	userData(2) = beta;
	userData(3) = gamma;
	userData(4) = delta;

	double epsilon = 10e-4;
	int Nmax = 10000;

	double c_inner = 10.0;
	double dc_inner = 0.2;

	double c_outer = 1.0;
	double dc_outer = 1.5;

	double a_tab[] = {
		4.0,
		4.4934,
		5.0
	};

#if 0

	for (double a : a_tab)
	{
		for (int i = 0; i < 100; i++)
		{
			matrix x(2, 1);
			x(0) = RandomNumberGenerator::Get().Double(1.5, 5.5);
			x(1) = RandomNumberGenerator::Get().Double(1.5, 5.5);

			SAVE_TO_FILE("x-" + std::to_string(a) + ".txt") << x(0) << ";" << x(1) << "\n";

			solution penResultOut = pen(lab3_fun_outer, x, c_outer, dc_outer, epsilon, Nmax, a, userData);
			SAVE_TO_FILE("pen_result-outer-" + std::to_string(a) + ".txt") << penResultOut.x(0) << ";" << penResultOut.x(1) << ";" << penResultOut.y(0) << ";" << solution::f_calls << "\n";
			solution::clear_calls();

			solution penResultIn = pen(lab3_fun_inner, x, c_inner, dc_inner, epsilon, Nmax, a, userData);
			SAVE_TO_FILE("pen_result-inner-" + std::to_string(a) + ".txt") << penResultIn.x(0) << ";" << penResultIn.x(1) << ";" << penResultIn.y(0) << ";" << solution::f_calls << "\n";
			solution::clear_calls();
		}
	}

#endif

	// Symulacja

	{
		matrix x0(2, 1);
		x0(0) = 5.0;
		x0(1) = 10.0;

		double c = 1.0;
		double dc = 1.5;

		solution optimal = pen(f3R, x0, c, dc, epsilon, Nmax, NULL, userData);
		solution::clear_calls();

		std::cout << optimal << "\n";

		matrix Y0(4, new double[4] { 0.0, optimal.x(0), 100, 0 });
		matrix* Y = solve_ode(df3, 0.0, 0.01, 7.0, Y0, NULL, optimal.x(1));

		std::cout << hcat(Y[0], Y[1]);
	}
}

void lab4() 
{
	//// Dopisac testy
	//matrix x(2, 1);
	//x(0) = 5;
	//x(1) = 2;

	//double h0 = .05;
	//double epsilon = 1e-8;
	//int Nmax = 10000;

	//solution simpleGrad = SD(fT4, lab4_grad, x, h0, epsilon, Nmax);
	//cout << "Simple grad \n";
	//cout << simpleGrad;
	//solution::clear_calls();

	//solution complexGrad = CG(fT4, lab4_grad, x, h0, epsilon, Nmax);
	//cout << "Complex grad \n";
	//cout << complexGrad;
	//solution::clear_calls();

	//solution hesjan = Newton(fT4, lab4_grad, lab4_hes, x, h0, epsilon, Nmax);
	//cout << "hesjan \n";
	//cout << hesjan;
	//solution::clear_calls();

	FileReaderFactory fileFactory; 
	unique_ptr<FileReader> reader = fileFactory.CreateFileReader(FileTypeEnum::Lab4);
	reader->Read(vector<filesystem::path>{"../Input/Project 4/XData.txt"});
}

void lab5() {}

void lab6() {}
