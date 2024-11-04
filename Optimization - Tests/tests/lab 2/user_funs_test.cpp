#include "pch.h"
#include "gtest/gtest.h"

#include "optimization/user_funs.h"

namespace lab_2
{
static const double EPSILON = 1e-10;

struct Point
{
	double x1, x2, y;
};

TEST(lab2_fun, handles_defined_values)
{
	// Given
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
} // namespace lab_2