#include "pch.h"
#include "gtest/gtest.h"

#include "optimization/user_funs.h"

static const double EPSILON = 1e-10;

static struct Point
{
	double x, y;
};

TEST(lab_1_fun, handles_defined_values)
{

	Point points[] = {
		{ 51.6181000476, -0.0702538786 },
		{ 95.8144372901, 0.1836267561 },
		{ -15.7152522079, 0.0049393830 },
		{ 77.9524392580, 0.1155650062 },
		{ -48.6612138586, 0.0473582747 },
		{ -9.2049862353, 0.0016946354 },
		{ -18.9124439630, 0.0071536107 },
		{ 53.3754988810, -0.1823347046 },
		{ 34.4417747893, 0.0240263015 },
		{ -41.2545845742, 0.0340388150 },
	};

	for (const auto& point : points)
	{
		matrix result = lab1_fun(point.x);
		EXPECT_NEAR(point.y, m2d(result), EPSILON);
	}
}

TEST(df1, handles_defined_values)
{
	// TODO: Write some tests ;)
}