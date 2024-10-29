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

#include <ctime>
#include <bits/fs_fwd.h>
#include <bits/fs_path.h>

void lab0();
void lab1();
void lab2();
void lab3();
void lab4();
void lab5();
void lab6();

int main()
{
	srand(time(NULL));
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
	double epsilon = 1e-3;
	int Nmax = 10000;

	//double* result = expansion(lab1_fun, -100, 1, 1.01, Nmax); //to wypluje minimum lokalne, to po lewej

	// 1. aplha = 1;
	// 2. aplha = 3.5;
	// 3. alpha = 10;
	// a) with expansion b) without
	double alpha = 1.5;
	std::stringstream ss;
	std::fstream outFile;
	// 1 of 3 iterations, add later for(i < 3)
	for(int exp = 0; exp < 3; exp++) {
		for(int iter = 0; iter < 100; iter++) {

			int start = rand()%100;
			double* result = expansion(lab1_fun, start, 1, alpha, Nmax );
			solution::clear_calls();
			ss << std::setw(15);
			ss << start << ";\t" << result[0] << ";\t"<<result[1] << ";\t" << solution::f_calls << ";\t";
			solution::clear_calls();

			solution resultFib = fib(lab1_fun, result[0], result[1], epsilon);

			ss << m2d(resultFib.x) << ";\t" << m2d(resultFib.y) << ";\t" << solution::f_calls << ";\t" << (resultFib.x > -1 && resultFib.x < 1 ? "lokalne" : "globalne") << ";\t\n";


			// std::cout << resultFib << "\n";
			solution::clear_calls();
			alpha += 1.5;
		}

		//not working
		filesystem::__cxx11::path currentPath = std::filesystem::current_path();
		std::cout << "Current working directory: " << currentPath << std::endl;
		outFile.open("../../../Results/test_alpha_" + std::to_string(alpha)+".csv");
		if(outFile.is_open()) {
			outFile << ss.str();
		}else {
			std::cerr << "ERROR:\n";
		}
		outFile.close();
		ss.str("");
	}

	double* result = expansion(lab1_fun, 50, 1, 1.01, Nmax); //to wypluje minimum globalne, to po prawej
	solution::clear_calls();

	cout << result[0] << ", " << result[1] << "\n";


	// solution resultFib = fib(lab1_fun, result[0], result[1], 0.001);
	// std::cout << resultFib << "\n";
	// solution::clear_calls();

}

void lab2() {}

void lab3() {}

void lab4() {}

void lab5() {}

void lab6() {}
