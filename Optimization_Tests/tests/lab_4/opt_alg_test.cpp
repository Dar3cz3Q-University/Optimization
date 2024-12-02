#include "pch.h"
#include "gtest/gtest.h"

#include "optimization/user_funs.h"
#include "optimization/opt_alg.h"

namespace lab_4
{

TEST(SD, lab4_fun__lab4_grad)
{
	// Given
	matrix x(2, 1);
	x(0) = 5;
	x(1) = 2;

	double h0 = .01;
	double epsilon = 1e-8;
	int Nmax = 10000;

	// When
	solution::clear_calls;
	solution result = SD(lab4_fun, lab4_grad, x, h0, epsilon, Nmax);
	
	// Then
	EXPECT_NEAR(result.x(0), 1, 1e-5);
	EXPECT_NEAR(result.x(1), 3, 1e-5);
	EXPECT_EQ(solution::g_calls, 782);
	// TODO: Add comparing y when function will be using fit_fun
}

TEST(CG, lab4_fun__lab4_grad)
{
	// Given
	matrix x(2, 1);
	x(0) = 5;
	x(1) = 2;

	double h0 = .01;
	double epsilon = 1e-8;
	int Nmax = 10000;

	// When
	solution::clear_calls;
	solution result = CG(lab4_fun, lab4_grad, x, h0, epsilon, Nmax);

	// Then
	EXPECT_NEAR(result.x(0), 1, 1e-5);
	EXPECT_NEAR(result.x(1), 3, 1e-5);
	EXPECT_EQ(solution::g_calls, 907);
	// TODO: Add comparing y when function will be using fit_fun
}

TEST(Newton, lab4_fun__lab4_grad)
{
	// Given
	matrix x(2, 1);
	x(0) = 5;
	x(1) = 2;

	double h0 = .01;
	double epsilon = 1e-8;
	int Nmax = 10000;

	// When
	solution::clear_calls;
	solution result = Newton(lab4_fun, lab4_grad, lab4_hes, x, h0, epsilon, Nmax);

	// Then
	EXPECT_NEAR(result.x(0), 1, 1e-5);
	EXPECT_NEAR(result.x(1), 3, 1e-5);
	EXPECT_EQ(solution::g_calls, 2424);
	EXPECT_EQ(solution::H_calls, 1517);
	// TODO: Add comparing y when function will be using fit_fun
}

} // namespace lab_4
