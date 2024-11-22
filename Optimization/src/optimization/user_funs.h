// Univeristy File

#pragma once

#include "ode_solver.h"

inline constexpr double EARTH_ACCELERATION = 9.81;

matrix ff0T(matrix, matrix = NAN, matrix = NAN);

matrix ff0R(matrix, matrix = NAN, matrix = NAN);

matrix df0(double, matrix, matrix = NAN, matrix = NAN);

matrix lab1_fun(matrix, matrix = NAN, matrix = NAN);

matrix df1(double, matrix, matrix, matrix);

matrix f1R(matrix, matrix = NAN, matrix = NAN);

matrix lab2_fun(matrix, matrix = NAN, matrix = NAN);

matrix df2(double, matrix, matrix, matrix);

matrix f2R(matrix, matrix = NAN, matrix = NAN);

matrix df3(double, matrix, matrix, matrix);

matrix f3R(matrix, matrix = NAN, matrix = NAN);
