// Univeristy File

#include "pch.h"

#include "opt_alg.h"
#include "solution.h"

#include "FileSaver.h"

solution MC(matrix(*ff)(matrix, matrix, matrix), int N, matrix lb, matrix ub, double epsilon, int Nmax, matrix ud1, matrix ud2)
{
	try
	{
		solution Xopt;
		while (true)
		{
			Xopt = rand_mat(N);
			for (int i = 0; i < N; ++i)
				Xopt.x(i) = (ub(i) - lb(i)) * Xopt.x(i) + lb(i);
			Xopt.fit_fun(ff, ud1, ud2);
			if (Xopt.y < epsilon)
			{
				Xopt.flag = 1;
				break;
			}
			if (solution::f_calls > Nmax)
			{
				Xopt.flag = 0;
				break;
			}
		}
		return Xopt;
	}
	catch (string ex_info)
	{
		throw("solution MC(...):\n" + ex_info);
	}
}

double* expansion(matrix(*ff)(matrix, matrix, matrix), double x0, double d, double alpha, int Nmax, matrix ud1, matrix ud2)
{
	try
	{
		double* p = new double[2] { 0, 0 };

		int i = 0;

		solution sx0(x0), sx1(x0 + d);

		sx0.fit_fun(ff);
		sx1.fit_fun(ff);

		if (sx0.y == sx1.y)
		{
			p[0] = m2d(sx0.x);
			p[1] = m2d(sx0.y);

			return p;
		}
		else if (sx1.y > sx0.y)
		{
			d = -d;

			sx1 = sx0.x + d;

			if (sx1.y >= sx0.y)
			{
				p[0] = m2d(sx1.x);
				p[1] = m2d(sx0.x) - d;

				return p;
			}
		}

		solution sx2(x0 + d);
		sx2.fit_fun(ff);

		do
		{
			if (sx0.f_calls > Nmax)
				throw("Przekroczono limit wywolan funkcji :)");
			i++;

			sx0 = sx1;
			sx1 = sx2;

			sx2.x = m2d(sx0.x) + pow(alpha, i) * d;
			sx2.fit_fun(ff);
		} while (m2d(sx1.y) > m2d(sx2.y));

		if (d > 0)
		{
			p[0] = m2d(sx0.x);
			p[1] = m2d(sx2.x);

			return p;
		}

		p[0] = m2d(sx2.x);
		p[1] = m2d(sx0.x);

		return p;
	}
	catch (string ex_info)
	{
		throw("double* expansion(...):\n" + ex_info);
	}
}

solution fib(matrix(*ff)(matrix, matrix, matrix), double a, double b, double epsilon, matrix ud1, matrix ud2)
{
	try
	{
		std::vector<double> sigma = { 1, 1 };
		double ratio = (b - a) / epsilon;
		while (true)
		{
			if (sigma.back() > ratio)
				break;

			sigma.push_back(sigma[sigma.size() - 1] + sigma[sigma.size() - 2]);
		}
		int k = static_cast<int>(sigma.size() - 1);

		double a0 = a;
		double b0 = b;
		double c0 = b0 - sigma[k - 1] / sigma[k] * (b0 - a0);
		double d0 = a0 + b0 - c0;

		solution c_sol, d_sol;
		for (int i = 0; i <= k - 3; ++i)
		{
			SAVE_TO_FILE("fib-b-a.txt") << b0 - a0 << "\n";

			c_sol.x = c0;
			c_sol.fit_fun(ff, ud1, ud2);

			d_sol.x = d0;
			d_sol.fit_fun(ff, ud1, ud2);

			if (c_sol.y < d_sol.y)
				b0 = d0;
			else
				a0 = c0;

			c0 = b0 - sigma[k - i - 2] / sigma[k - i - 1] * (b0 - a0);
			d0 = a0 + b0 - c0;
		}

		solution Xopt;
		Xopt.x = c0;
		Xopt.fit_fun(ff, ud1, ud2);

		return Xopt;
	}
	catch (string ex_info)
	{
		throw ("solution fib(...):\n" + ex_info);
	}
}

solution lag(matrix(*ff)(matrix, matrix, matrix), double a, double b, double epsilon, double gamma, int Nmax, matrix ud1, matrix ud2)
{
	try
	{
		solution Xopt;

		double ai = a;
		double bi = b;
		double ci = (a + b) / 2;
		double di{};

		int i = 0;
		double l{}, m{};
		solution ai_sol, bi_sol, ci_sol, di_sol;
		double l_prev{}, m_prev{}, di_prev{};
		do
		{
			SAVE_TO_FILE("lag-b-a.txt") << bi - ai << "\n";

			ai_sol.x = ai;
			ai_sol.fit_fun(ff, ud1, ud2);

			bi_sol.x = bi;
			bi_sol.fit_fun(ff, ud1, ud2);

			ci_sol.x = ci;
			ci_sol.fit_fun(ff, ud1, ud2);

			l = m2d(ai_sol.y) * (pow(bi, 2) - pow(ci, 2)) + m2d(bi_sol.y) * (pow(ci, 2) - pow(ai, 2)) + m2d(ci_sol.y) * (pow(ai, 2) - pow(bi, 2));
			m = m2d(ai_sol.y) * (bi - ci) + m2d(bi_sol.y) * (ci - ai) + m2d(ci_sol.y) * (ai - bi);

			if (m <= 0)
			{
				Xopt.flag = 0;
				break;
			}

			di = 0.5 * l / m;
			di_sol.x = di;
			di_sol.fit_fun(ff, ud1, ud2);

			if (ai < di && di < ci)
			{
				if (di_sol.y < ci_sol.y)
				{
					bi = ci;
					ci = di;
				}
				else
					ai = di;
			}
			else
			{
				if (ci < di && di < bi)
				{
					if (di_sol.y < ci_sol.y)
					{
						ai = ci;
						ci = di;
					}
					else
						bi = di;
				}
				else
				{
					Xopt.flag = 0;
					break;
				}
			}

			if (ai_sol.f_calls > Nmax)
			{
				Xopt.flag = 0;
				throw std::string("Error message!");
				break;
			}

			if (i > 0)
				di_prev = 0.5 * l_prev / m_prev;

			l_prev = l;
			m_prev = m;

			++i;
		} while (!(bi - ai < epsilon || abs(di - di_prev) < gamma));

		Xopt.x = di;
		Xopt.fit_fun(ff, ud1, ud2);

		return Xopt;
	}
	catch (string& ex_info)
	{
		throw("solution lag(...):\n" + ex_info);
	}
}

solution HJ(matrix(*ff)(matrix, matrix, matrix), matrix x0, double s, double alpha, double epsilon, int Nmax, matrix ud1, matrix ud2)
{
	try
	{
		solution xOpt;
		solution XB = x0;
		solution XB_;
		
		do
		{
			xOpt = HJ_trial(ff, XB, s);

			xOpt.fit_fun(ff, ud1, ud2);
			XB.fit_fun(ff, ud1, ud2);

			if (xOpt.y < XB.y)
			{
				do
				{
					SAVE_TO_FILE("x1.x2-HJ.txt") << m2d(XB.x(0)) << ";" << m2d(XB.x(1)) << "\n";

					XB_ = XB;
					XB = xOpt;
					xOpt.x = 2.0 * XB.x - XB_.x;
					xOpt = HJ_trial(ff, xOpt, s);
				
					xOpt.fit_fun(ff, ud1, ud2);
					XB.fit_fun(ff, ud1, ud2);

					if (solution::f_calls > Nmax)
						throw std::string("Przekroczono limit wywolan funkcji :)");
				
				} while (xOpt.y < XB.y);

				xOpt = XB;
			}
			else
				s *= alpha;

			if (solution::f_calls > Nmax)
				throw std::string("Przekroczono limit wywolan funkcji :)");

		} while (s > epsilon);

		return xOpt;
	}
	catch (string ex_info)
	{
		throw("solution HJ(...):\n" + ex_info);
	}
}

solution HJ_trial(matrix(*ff)(matrix, matrix, matrix), solution XB, double s, matrix ud1, matrix ud2)
{
	try
	{
		matrix base(2, 2);
		base(0, 0) = 1.0;
		base(1, 1) = 1.0;
		solution tempSolution;

		for (int i = 0; i < get_len(XB.x); i++)
		{
			XB.fit_fun(ff, base, ud2);
			matrix y1 = XB.y;

			tempSolution.x = XB.x + s * base[i];
			if (tempSolution.fit_fun(ff, base, ud2) < y1)
				XB.x = XB.x + s * base[i];
			else
			{
				tempSolution.x = XB.x - s * base[i];
				if (tempSolution.fit_fun(ff, base, ud2) < y1)
					XB.x = XB.x = XB.x - s * base[i];
			}
		}

		return XB;
	}
	catch (string ex_info)
	{
		throw("solution HJ_trial(...):\n" + ex_info);
	}
}

solution Rosen(matrix(*ff)(matrix, matrix, matrix), matrix x0, matrix s0, double alpha, double beta, double epsilon, int Nmax, matrix ud1, matrix ud2)
{
	const int n = 2;

	try
	{
		solution Xopt;

		matrix base(n, n);
		base(0, 0) = 1.0;
		base(1, 1) = 1.0;

		matrix lambda(n, new double[n]{ 0.0 });
		matrix p(n, new double[n] { 0.0 });
		solution XB = x0;
		solution XB2 = 0;

		matrix s = s0;
		double max_s = 0.0;

		int i = 0;
		do
		{
			SAVE_TO_FILE("x1.x2-Rosen.txt") << m2d(XB.x(0)) << ";" << m2d(XB.x(1)) << "\n";

			for (int j = 0; j < n; j++)
			{
				XB2 = XB.x + s(j) * base[j];
				XB2.fit_fun(ff, ud1, ud2);
				XB.fit_fun(ff, ud1, ud2);
				if (XB2.y < XB.y)
				{
					XB.x = XB.x + s(j) * base[j];
					lambda(j) += s(j);
					s(j) *= alpha;
				}
				else
				{
					s(j) *= -beta;
					p(j) += 1;
				}
			}

			i++;
			Xopt.x = XB.x;

			bool changeBase = true;
			for (int j = 0; j < n; j++)
			{
				if (lambda(j) == 0 || p(j) == 0)
				{
					changeBase = false;
					break;
				}
			}

			if (changeBase)
			{
				matrix lambdaMatrix(n, n);
				int l = 0;
				for (int k = 0; k < n; ++k)
				{
					for (int j = 0; j <= k; ++j)
						lambdaMatrix(k, j) = lambda(l);
					++l;
				}

				matrix Q = base * lambdaMatrix;

				matrix v1 = Q[0];
				double v1_norm = norm(v1);
				v1 = v1 / v1_norm;

				base[0] = v1;

				matrix v2 = Q[1] - (trans(Q[1]) * base[0]) * base[0];
				double v2_norm = norm(v2);
				v2 = v2 / v2_norm;

				base[1] = v2;

				lambda = matrix(n, new double[n] { 0.0 });
				p = matrix(n, new double[n] { 0.0 });
				s = s0;
			}

			if (solution::f_calls > Nmax)
				throw string("Przekroczono limit wywolan funkcji :)");

			max_s = 0.0;
			for (int j = 0; j < n; j++)
				if (abs(s(j)) > max_s)
					max_s = abs(s(j));
			
		} while (max_s > epsilon);

		Xopt.fit_fun(ff, ud1, ud2);

		return Xopt;
	}
	catch (string ex_info)
	{
		throw("solution Rosen(...):\n" + ex_info);
	}
}

solution pen(matrix(*ff)(matrix, matrix, matrix), matrix x0, double c, double dc, double epsilon, int Nmax, matrix ud1, matrix ud2)
{
	try
	{
		solution XB;
		XB.x = x0;
		XB.fit_fun(ff, ud1, c);

		solution XT;
		XT = XB;

		double s = ud2(0);
		double alpha = ud2(1);
		double beta = ud2(2);
		double gamma = ud2(3);
		double delta = ud2(4);

		while (true)
		{
			XT = sym_NM(ff, XB.x, s, alpha, beta, gamma, delta, epsilon, Nmax, ud1, c);
			c *= dc;

			if (solution::f_calls > Nmax)
				throw std::string("Przekroczono limit wywolan funkcji :)");

			if (norm(XT.x - XB.x) < epsilon)
				break;

			XB = XT;
		};

		return XT;
	}
	catch (string ex_info)
	{
		throw("solution pen(...):\n" + ex_info);
	}
}

solution sym_NM(matrix(*ff)(matrix, matrix, matrix), matrix x0, double s, double alpha, double beta, double gamma, double delta, double epsilon, int Nmax, matrix ud1, matrix ud2)
{
#if 1
	try
	{
		//Funkcja pomocnicza do znajdywania maksymum normy
		auto max = [&](std::vector<solution> sim, int i_min) -> double
			{
				double result = 0.0;
				for (int i = 0; i < sim.size(); ++i)
				{
					double normal = norm(sim[i_min].x - sim[i].x);
					if (result < normal)
						result = normal;
				}
				return result;
			};

		int n = get_len(x0);

		//Tworzenie bazy ortogonalnej
		matrix d = matrix(n, n);
		for (int i = 0; i < n; ++i)
			d(i, i) = 1.0;

		//Tworzenie simplexu i uzupe³nianie go danymi
		std::vector<solution> simplex;
		simplex.resize(n + 1);
		simplex[0].x = x0;
		simplex[0].fit_fun(ff, ud1, ud2);
		for (int i = 1; i < simplex.size(); ++i)
		{
			simplex[i].x = simplex[0].x + s * d[i - 1];
			simplex[i].fit_fun(ff, ud1, ud2);
		}

		//Indeks najmniejszej wartoœci wierzcho³ka simplexu
		int i_min{};
		//Indeks najwiêkszej wartoœci wierzcho³ka simplexu
		int i_max{};

		while (max(simplex, i_min) >= epsilon)
		{
			//Wyznaczanie maksymalnego i minimalnego indeksu
			i_min = 0;
			i_max = 0;
			for (int i = 1; i < simplex.size(); ++i)
			{
				if (simplex[i].y < simplex[i_min].y)
					i_min = i;
				if (simplex[i].y > simplex[i_max].y)
					i_max = i;
			}

			//Wyznaczenie œrodka ciê¿koœci
			matrix simplex_CoG{};
			for (int i = 0; i < simplex.size(); ++i)
			{
				if (i == i_max)
					continue;
				simplex_CoG = simplex_CoG + simplex[i].x;
			}
			simplex_CoG = simplex_CoG / simplex.size();

			//Obliczanie wartoœci funkcji odbitego simplexu
			solution simplex_reflected{};
			simplex_reflected.x = simplex_CoG + alpha * (simplex_CoG - simplex[i_max].x);
			simplex_reflected.fit_fun(ff, ud1, ud2);

			if (simplex_reflected.y < simplex[i_min].y)
			{
				//Obliczanie wartoœci funkcji powiêkszonego simplexu
				solution simplex_expansion{};
				simplex_expansion.x = simplex_CoG + gamma * (simplex_reflected.x - simplex_CoG);
				simplex_expansion.fit_fun(ff, ud1, ud2);
				if (simplex_expansion.y < simplex_reflected.y)
					simplex[i_max] = simplex_expansion;
				else
					simplex[i_max] = simplex_reflected;
			}
			else
			{
				if (simplex[i_min].y <= simplex_reflected.y && simplex_reflected.y < simplex[i_max].y)
					simplex[i_max] = simplex_reflected;
				else
				{
					//Obliczanie wartoœci funkcji pomniejszonego simplexu
					solution simplex_narrowed{};
					simplex_narrowed.x = simplex_CoG + beta * (simplex[i_max].x - simplex_CoG);
					simplex_narrowed.fit_fun(ff, ud1, ud2);
					if (simplex_narrowed.y >= simplex[i_max].y)
					{
						for (int i = 0; i < simplex.size(); ++i)
						{
							if (i == i_min)
								continue;
							simplex[i].x = delta * (simplex[i].x + simplex[i_min].x);
							simplex[i].fit_fun(ff, ud1, ud2);
						}
					}
					else
						simplex[i_max] = simplex_narrowed;
				}
			}

			if (solution::f_calls > Nmax)
			{
				simplex[i_min].flag = 0;
				throw std::string("Maximum amount of f_calls reached!");
			}

		}

		return simplex[i_min];
	}
	catch (string ex_info)
	{
		throw ("solution sym_NM(...):\n" + ex_info);
	}
#else
	try
	{
		solution Xopt;
		int n = 3;
		matrix base(3, 3);
		base(0, 0) = 1.0;
		base(1, 1) = 1.0;
		base(2, 2) = 1.0;

		std::vector<solution> p;
		p.reserve(n);
		p.push_back(x0);

		for (int i = 1; i < n; i++)
		{
			double* coords = new double[2]
				{
					p[0].x(0) + s * base(i, i),
						p[0].x(1) + s * base(i, i)
				};

			solution newP(2, coords);
			p.push_back(newP);
		}

		int max_index{};
		int min_index{};

		solution p_(matrix(2, 1, 0.0));

		solution p_odb;
		solution p_z;
		solution p_e;

		double max = 0;

		do
		{
			// POLECENIE: oblicz wartosci funkcji w wierzcholkach sympleksu p0 p1 .. pn
			for (int i = 0; i < n; i++)
				p[i].fit_fun(ff, ud1, ud2);

			// POLECENIE: wyznacz p_min i p_max (min =/= max)
			min_index = 0;
			max_index = 0;

			for (int i = 0; i < n; i++)
			{
				if (p[i].y > p[max_index].y)
					max_index = i;

				if (p[i].y < p[min_index].y)
					min_index = i;
			}

			// KONIEC POLECENIE

			for (int i = 0; i < n; i++)
				if (i != max_index)
					p_.x = p_.x + p[i].x;

			p_.x = p_.x / n;

			p_odb.x = p_.x + alpha * (p_.x - p[max_index].x);
			p_odb.fit_fun(ff, ud1, ud2);

			if (p_odb.y < p[min_index].y)
			{
				p_e.x = p_.x + gamma * (p_odb.x - p_.x);
				p_e.fit_fun(ff, ud1, ud2);

				if (p_e.y < p_odb.y)
					p[max_index] = p_e;
				else
					p[max_index] = p_odb;
			}
			else
			{
				if (p[min_index].y <= p_odb.y && p_odb.y < p[max_index].y)
				{
					p[max_index] = p_odb;
				}
				else
				{
					p_z.x = p_.x + beta * (p[max_index].x - p_.x);
					p_z.fit_fun(ff, ud1, ud2);

					if (p_z.y >= p[max_index].y)
					{
						for (int i = 0; i < n; i++)
						{
							if (i != min_index)
							{
								p[i].x = delta * (p[i].x + p[min_index].x);
								p[i].fit_fun(ff, ud1, ud2);
							}
						}
					}
					else
					{
						p[max_index] = p_z;
					}
				}
			}

			max = 0;

			for (int i = 0; i < n; i++)
			{
				double value = abs(m2d(p[min_index].y) - m2d(p[i].y));

				if (value > max)
					max = value;
			}

			if (solution::f_calls > Nmax)
			{
				throw std::string("Przekroczono limit wywolan funkcji :)");
			}
		} while (max > epsilon);

		return p[min_index];
		}
	catch (string ex_info)
	{
		throw("solution sym_NM(...):\n" + ex_info);
	}
#endif
}

// Gradient prosty
solution SD(matrix(*ff)(matrix, matrix, matrix), matrix(*gf)(matrix, matrix, matrix), matrix x0, double h0, double epsilon, int Nmax, matrix ud1, matrix ud2)
{
	try
	{
		solution Xopt;
		Xopt.x = x0;

		solution X_prev;
	
		matrix d;

		do
		{
			X_prev = Xopt;
			Xopt.grad(gf, ud1, ud2);
			d = -Xopt.g;

			// Tutaj liczylibysmy h_i ale Michal mowi ze nie trzeba 

			Xopt.x = X_prev.x + h0 * d;

			if (solution::g_calls > Nmax)
				throw std::string("Przekroczono limit wywolan funkcji :(");

		} while (norm(Xopt.x - X_prev.x) > epsilon);

		return Xopt;
	}
	catch (string ex_info)
	{
		throw("solution SD(...):\n" + ex_info);
	}
}

// Gradient sprzezony
solution CG(matrix(*ff)(matrix, matrix, matrix), matrix(*gf)(matrix, matrix, matrix), matrix x0, double h0, double epsilon, int Nmax, matrix ud1, matrix ud2)
{
	// TODO: Dla h0 =< 0 krok zmienny a dla h0 > 0 krok staly
	try
	{
		solution Xopt = x0;
		solution X_prev;

		Xopt.grad(gf, ud1, ud2);

		matrix d = -Xopt.g;

		matrix di;

		do
		{
			X_prev = Xopt;
			d = di;

			Xopt.grad(gf, ud1, ud2);
			double beta = pow(norm(Xopt.g), 2) / pow(norm(X_prev.g), 2);

			di = -Xopt.g + beta * d;

			Xopt.x = X_prev.x + h0 * di;

			if (solution::g_calls > Nmax)
				throw std::string("Przekroczono limit wywolan funkcji :(");

		} while (norm(Xopt.x - X_prev.x) > epsilon);

		return Xopt;
	}
	catch (string ex_info)
	{
		throw("solution CG(...):\n" + ex_info);
	}
}

// Gradient z Hesjanem :)... kim jest ten Hesjan?
solution Newton(matrix(*ff)(matrix, matrix, matrix), matrix(*gf)(matrix, matrix, matrix),
	matrix(*Hf)(matrix, matrix, matrix), matrix x0, double h0, double epsilon, int Nmax, matrix ud1, matrix ud2)
{
	try
	{
		solution Xopt = x0;
		solution X_prev;

		matrix d;

		do
		{
			X_prev = Xopt;

			Xopt.hess(Hf, ud1, ud2);
			Xopt.grad(gf, ud1, ud2);

			d = -inv(Xopt.H) * Xopt.g;

			Xopt.x = X_prev.x + h0 * d;

			if (solution::H_calls > Nmax)
				throw std::string("Przekroczono limit wywolan funkcji hess() :(");

			if (solution::g_calls > Nmax)
				throw std::string("Przekroczono limit wywolan funkcji grad() :(");

		} while (norm(Xopt.x - X_prev.x) > epsilon);

		return Xopt;
	}
	catch (string ex_info)
	{
		throw("solution Newton(...):\n" + ex_info);
	}
}

solution golden(matrix(*ff)(matrix, matrix, matrix), double a, double b, double epsilon, int Nmax, matrix ud1, matrix ud2)
{
	try
	{
		solution Xopt;
		// Tu wpisz kod funkcji

		return Xopt;
	}
	catch (string ex_info)
	{
		throw("solution golden(...):\n" + ex_info);
	}
}

solution Powell(matrix(*ff)(matrix, matrix, matrix), matrix x0, double epsilon, int Nmax, matrix ud1, matrix ud2)
{
	try
	{
		solution Xopt;
		// Tu wpisz kod funkcji

		return Xopt;
	}
	catch (string ex_info)
	{
		throw("solution Powell(...):\n" + ex_info);
	}
}

solution EA(matrix(*ff)(matrix, matrix, matrix), int N, matrix lb, matrix ub, int mi, int lambda, matrix sigma0, double epsilon, int Nmax, matrix ud1, matrix ud2)
{
	try
	{
		solution Xopt;
		// Tu wpisz kod funkcji

		return Xopt;
	}
	catch (string ex_info)
	{
		throw("solution EA(...):\n" + ex_info);
	}
}
