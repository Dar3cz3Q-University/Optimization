#include "pch.h"
#include "gtest/gtest.h"

#include "optimization/user_funs.h"
#include "optimization/solution.h"

namespace lab_3
{

TEST(lab3_fun_help, handles_defined_values)
{
	// Given
	matrix x(2, 1);
	x(0) = 1.0;
	x(1) = 2.0;

	// When
	matrix result = lab3_fun_help(x, matrix(), matrix());

	// Then
	EXPECT_NEAR(m2d(result), 0.3518, 1e-4);
}

TEST(lab3_fun_outer, handles_defined_values)
{
	// Given
	matrix userData(5, 1);
	userData(0) = 0.5;
	userData(1) = 1.0;
	userData(2) = 0.5;
	userData(3) = 2.0;
	userData(4) = 0.5;

	matrix x(2, 1);
	x(0) = 5.5;
	x(1) = 3.2;

	double a = 4.0;

	// When
	matrix result = lab3_fun_outer(x, a, userData);
	
	// Then
	EXPECT_NEAR(result(0), 2.80486, 1e-5);
	EXPECT_NEAR(result(1), 5.59715, 1e-5);
	EXPECT_NEAR(result(2), 2.80486, 1e-5);
	EXPECT_NEAR(result(3), 11.1818, 1e-4);
	EXPECT_NEAR(result(4), 2.80486, 1e-5);
}

TEST(lab3_fun_inner, handles_defined_values)
{
	// Given
	matrix x(2, 1);
	x(0) = 5.5;
	x(1) = 3.2;

	double a = 4.0;
	double c = 10.0;

	// When
	matrix result = lab3_fun_inner(x, a, c);
	
	// Then
	EXPECT_EQ(m2d(result), 1e10);
}

} // namespace lab_3