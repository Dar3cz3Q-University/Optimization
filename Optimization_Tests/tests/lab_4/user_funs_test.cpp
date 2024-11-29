#include "pch.h"
#include "gtest/gtest.h"

#include "optimization/user_funs.h"
#include "optimization/solution.h"

namespace lab_4
{

TEST(lab4_fun, handles_defined_values)
{
	// Given
	matrix x(2, 1);
	x(0) = 1.0;
	x(1) = 2.0;

	// When
	matrix result = lab4_fun(x, NULL, NULL);

	// Then
	EXPECT_EQ(m2d(result), 5);
}

TEST(fT4, handles_defined_values) // TODO: Fix test when function ready
{
	// Given
	matrix x(2, 1);
	x(0) = 2.0;
	x(1) = 3.0;

	matrix ud2(2, 2);
	ud2(0, 0) = 1;
	ud2(0, 1) = 2;
	ud2(1, 0) = 3;
	ud2(1, 1) = 4;

	// When
	//matrix result = fT4(x, NULL, ud2);

	// Then
	//EXPECT_EQ(m2d(result), 17);
}

TEST(lab4_grad, handles_defined_values)
{
	// Given
	matrix x(2, 1);
	x(0) = 3.0;
	x(1) = 5.0;

	// When
	matrix result = lab4_grad(x, NULL, NULL);

	// Then
	EXPECT_EQ(result(0), 36.0);
	EXPECT_EQ(result(1), 36.0);
}

TEST(lab4_hes, handles_defined_values)
{
	// Given

	// When
	matrix result = lab4_hes(matrix(), NULL, NULL);

	// Then
	EXPECT_EQ(result(0, 0), 10);
	EXPECT_EQ(result(0, 1), 8);
	EXPECT_EQ(result(1, 0), 8);
	EXPECT_EQ(result(1, 1), 10);
}

} // namespace lab_4