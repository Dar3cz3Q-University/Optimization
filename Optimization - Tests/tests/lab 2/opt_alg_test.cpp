#include "pch.h"
#include "gtest/gtest.h"

#include "optimization/user_funs.h"
#include "optimization/opt_alg.h"
#include "utils/RandomNumberGenerator.h"

namespace lab_2
{
static const double EPSILON = 1e-4;

TEST(HJ, handles_defined_values)
{
	// Given
	matrix x(2, 1);
	x(0) = 0.5;
	x(1) = 0.75;

	double s = 0.1;
	double alpha = 0.1;
	double epsilon = 1e-10;
	double Nmax = 10000;

	// When
	solution result = HJ(lab2_fun, x, s, alpha, epsilon, Nmax);

	// Then
	EXPECT_NEAR(result.x(0), 0.774716, EPSILON);
	EXPECT_NEAR(result.x(1), 0.774716, EPSILON);
	EXPECT_NEAR(m2d(result.y), 1.23967, EPSILON);
}

TEST(HJ_trial, handles_defined_values)
{
	// TODO: Write some tests ;)
}

TEST(Rosen, handles_defined_values)
{
	// TODO: Write some tests ;)
}
} // namespace lab_2
