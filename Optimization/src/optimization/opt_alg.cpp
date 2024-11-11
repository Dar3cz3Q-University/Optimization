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

		// cout << "f(" << m2d(sx0.x) << ") = " << m2d(sx0.y) << endl;
		// cout << "f(" << m2d(sx1.x) << ") = " << m2d(sx1.y) << endl;

		if (sx0.y == sx1.y)
		{
			p[0] = m2d(sx0.x);
			p[1] = m2d(sx0.y); // czy to w sumie jest dobrze? imo powinno byc sx1.x zgodnie z "konwencją" nazewnictwa tego programu x^(id) to sxID.x, sx0.y byloby f(x^(0)) a jest w pseudokodzie x^(1)

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
		sx2.fit_fun(ff); // tego brakowalo, inaczej wychodzila wartosc sx2.y = -nan(ind)

		do
		{
			if (sx0.f_calls > Nmax)
				throw("Przekroczono limit wywolan funkcji :)");
			i++;

			sx0 = sx1;
			sx1 = sx2;

			sx2.x = m2d(sx0.x) + pow(alpha, i) * d;
			sx2.fit_fun(ff);
			// cout << m2d(sx1.y) << " > " << m2d(sx2.y) << " ? " << endl;
		} while (m2d(sx1.y) > m2d(sx2.y)); // tu teraz porownuje przed m2d (przedtem brakowalo, tak jest bezpieczniej imo)

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
			{
				di_prev = 0.5 * l_prev / m_prev;
			}

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
			{
				s *= alpha;
			}

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
#if 1
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
#else
			if (ff(XB.x + s * base[i], base, ud2) < ff(XB.x, base, ud2))
				XB.x = XB.x + s * base[i];
			else
				if (ff(XB.x - s * base[i], base, ud2) < ff(XB.x, base, ud2))
					XB.x = XB.x - s * base[i];
#endif
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
			for (int j = 0; j < n; j++)
			{
				XB2 = XB.x + s(j) * base[j];
				XB2.fit_fun(ff, ud1, ud2);
				XB.fit_fun(ff, ud1, ud2);
				//if (ff(XB + s(j) * base[j], ud1, ud2) < ff(XB, ud1, ud2)) // TODO: Rewrite function to use fit_fun
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
		solution Xopt;
		// Tu wpisz kod funkcji

		return Xopt;
	}
	catch (string ex_info)
	{
		throw("solution pen(...):\n" + ex_info);
	}
}

solution sym_NM(matrix(*ff)(matrix, matrix, matrix), matrix x0, double s, double alpha, double beta, double gamma, double delta, double epsilon, int Nmax, matrix ud1, matrix ud2)
{
	try
	{
		solution Xopt;
		// Tu wpisz kod funkcji

		return Xopt;
	}
	catch (string ex_info)
	{
		throw("solution sym_NM(...):\n" + ex_info);
	}
}

solution SD(matrix(*ff)(matrix, matrix, matrix), matrix(*gf)(matrix, matrix, matrix), matrix x0, double h0, double epsilon, int Nmax, matrix ud1, matrix ud2)
{
	try
	{
		solution Xopt;
		// Tu wpisz kod funkcji

		return Xopt;
	}
	catch (string ex_info)
	{
		throw("solution SD(...):\n" + ex_info);
	}
}

solution CG(matrix(*ff)(matrix, matrix, matrix), matrix(*gf)(matrix, matrix, matrix), matrix x0, double h0, double epsilon, int Nmax, matrix ud1, matrix ud2)
{
	try
	{
		solution Xopt;
		// Tu wpisz kod funkcji

		return Xopt;
	}
	catch (string ex_info)
	{
		throw("solution CG(...):\n" + ex_info);
	}
}

solution Newton(matrix(*ff)(matrix, matrix, matrix), matrix(*gf)(matrix, matrix, matrix),
	matrix(*Hf)(matrix, matrix, matrix), matrix x0, double h0, double epsilon, int Nmax, matrix ud1, matrix ud2)
{
	try
	{
		solution Xopt;
		// Tu wpisz kod funkcji

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
