#include "pch.h"
#include "gtest/gtest.h"

#include "optimization/user_funs.h"
#include "optimization/solution.h"

namespace lab_2
{
struct Point
{
	double x1, x2, y;
};

TEST(lab2_fun, handles_defined_values)
{
	// Given
	const double EPSILON = 1e-10;

	Point points[] = {
		{0.5, 0.5, 3.9142135623730954},
		{1.0, 1.0, 3.9999999999999996},
		{-1.0, -1.0, 3.9999999999999996},
		{0.25, -0.75, 2.083803899853803},
		{0.75, 0.25, 2.0838038998538035},
		{-0.5, 1.5, 4.500000000000002},
		{1.5, -0.5, 4.500000000000001},
		{0.1, 0.9, 1.4057864376269045},
		{-0.9, 0.1, 1.4057864376269045},
		{0.0, 0.0, 0.0},
		{1.25, -1.25, 6.972759065022574},
		{-1.25, 1.25, 6.972759065022574}
	};

	for (const auto& point : points)
	{
		matrix x(2, 1);
		x(0) = point.x1;
		x(1) = point.x2;

		// When
		matrix result = lab2_fun(x);

		// Then
		EXPECT_NEAR(point.y, m2d(result), EPSILON);
	}
}

TEST(df2, handles_defined_values)
{
	// Given
	const double EPSILON = 1e-3;

	matrix Y(2, 1);
	Y(0) = 0;
	Y(1) = 1;

	matrix ud1(2, 1);
	ud1(0) = M_PI;
	ud1(1) = 0;

	matrix k(2, 1);
	k(0) = 5.0;
	k(1) = 5.0;

	// When
	matrix result = df2(0, Y, ud1, k);

	// Then
	ASSERT_NEAR(result(0), 1.0, EPSILON);
	ASSERT_NEAR(result(1), 1.91399, EPSILON);
}

TEST(f2R, handles_defined_values)
{
	// Given
	const double EPSILON = 1e-3;

	matrix k(2, 1);
	k(0) = 5.0;
	k(1) = 5.0;

	// When
	matrix result = f2R(k);

	// Then
	ASSERT_NEAR(m2d(result), 245.032, EPSILON);
}

} // namespace lab_2