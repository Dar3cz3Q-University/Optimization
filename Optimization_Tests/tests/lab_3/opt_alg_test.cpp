#include "pch.h"
#include "gtest/gtest.h"

#include "optimization/user_funs.h"
#include "optimization/opt_alg.h"

namespace lab_3
{

TEST(pen, handles_defined_values_outer)
{
	// Given
	matrix userData(5, 1);
	userData(0) = 0.5;
	userData(1) = 1.0;
	userData(2) = 0.5;
	userData(3) = 2.0;
	userData(4) = 0.5;

	matrix x(2, 1);
	x(0) = 1.0;
	x(1) = 2.0;

	double a = 5.0;

	double epsilon = 10e-4;
	int Nmax = 10000;

	double c = 1.0;
	double dc = 1.5;

	// When
	solution::clear_calls();
	solution result = pen(lab3_fun_outer, x, c, dc, epsilon, Nmax, a, userData);

	// Then
	EXPECT_NEAR(result.x(0), 1.1582, 1e-4);
	EXPECT_NEAR(result.x(1), 4.3418, 1e-4);
	EXPECT_NEAR(result.y(0), -0.217234, 1e-6);
	EXPECT_EQ(solution::f_calls, 255);
}

TEST(pen, handles_defined_values_inner)
{
	// Given
	matrix userData(5, 1);
	userData(0) = 0.5;
	userData(1) = 1.0;
	userData(2) = 0.5;
	userData(3) = 2.0;
	userData(4) = 0.5;

	matrix x(2, 1);
	x(0) = 1.0;
	x(1) = 2.0;

	double a = 5.0;

	double epsilon = 10e-4;
	int Nmax = 10000;

	double c = 10.0;
	double dc = 0.2;

	// When
	solution::clear_calls();
	solution result = pen(lab3_fun_inner, x, c, dc, epsilon, Nmax, a, userData);

	// Then
	EXPECT_NEAR(result.x(0), 3.75, 1e-2);
	EXPECT_NEAR(result.x(1), 2.47461, 1e-5);
	EXPECT_NEAR(result.y(0), -0.217218, 1e-6);
	EXPECT_EQ(solution::f_calls, 432);
}

TEST(sym_NM, handles_defined_values_outer)
{
	// Given
	double s = 0.5;
	double alpha = 1.0;
	double beta = 0.5;
	double gamma = 2.0;
	double delta = 0.5;

	matrix x(2, 1);
	x(0) = 1.0;
	x(1) = 2.0;

	double a = 5.0;

	double epsilon = 10e-4;
	int Nmax = 10000;

	// When
	solution::clear_calls();
	solution result = sym_NM(lab3_fun_outer, x, s, alpha, beta, gamma, delta, epsilon, Nmax, a, matrix());

	// Then
	EXPECT_EQ(result.x(0), 1);
	EXPECT_EQ(result.x(1), 2.5);
	EXPECT_NEAR(result.y(0), 0.161211, 1e-6);
	EXPECT_EQ(solution::f_calls, 43
	);
}

TEST(sym_NM, handles_defined_values_inner)
{
	// Given
	double s = 0.5;
	double alpha = 1.0;
	double beta = 0.5;
	double gamma = 2.0;
	double delta = 0.5;

	matrix x(2, 1);
	x(0) = 1.0;
	x(1) = 2.0;

	double a = 5.0;

	double epsilon = 10e-4;
	int Nmax = 10000;

	// When
	solution::clear_calls();
	solution result = sym_NM(lab3_fun_inner, x, s, alpha, beta, gamma, delta, epsilon, Nmax, a, matrix());

	// Then
	EXPECT_NEAR(result.x(0), 1.25, 1e-2);
	EXPECT_NEAR(result.x(1), 2.25, 1e-2);
	EXPECT_NEAR(result.y(0), 0.208897, 1e-6);
	EXPECT_EQ(solution::f_calls, 43);
}

} // namespace lab_3
