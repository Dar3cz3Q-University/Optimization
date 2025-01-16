// Univeristy File

#pragma once

#include "ode_solver.h"

inline constexpr double EARTH_ACCELERATION = 9.81;

// Lab 0

matrix ff0T(matrix, matrix = NAN, matrix = NAN);

matrix ff0R(matrix, matrix = NAN, matrix = NAN);

matrix df0(double, matrix, matrix = NAN, matrix = NAN);

// Lab 1

matrix lab1_fun(matrix, matrix = NAN, matrix = NAN);

matrix df1(double, matrix, matrix, matrix);

matrix f1R(matrix, matrix = NAN, matrix = NAN);

// Lab 2

matrix lab2_fun(matrix, matrix = NAN, matrix = NAN);

matrix df2(double, matrix, matrix, matrix);

matrix f2R(matrix, matrix = NAN, matrix = NAN);

// Lab 3

matrix lab3_fun_help(matrix, matrix, matrix);

matrix lab3_fun_outer(matrix, matrix, matrix);

matrix lab3_fun_inner(matrix, matrix, matrix);

matrix df3(double, matrix, matrix, matrix);

matrix f3R(matrix, matrix = NAN, matrix = NAN);

// Lab 4

matrix lab4_fun(matrix, matrix = NAN, matrix = NAN);

matrix fT4(matrix, matrix = NAN, matrix = NAN);

matrix lab4_grad(matrix, matrix = NAN, matrix = NAN);

matrix lab4_hes(matrix, matrix = NAN, matrix = NAN);

double sigmoid(matrix, matrix);

matrix cost_function(matrix, matrix = NAN, matrix = NAN);

matrix cost_function_grad(matrix, matrix = NAN, matrix = NAN);

// Lab 5
matrix lab5_fun(matrix, matrix = NAN, matrix = NAN);

matrix fT5(matrix, matrix = NAN, matrix = NAN);

matrix fR5(matrix, matrix = NAN, matrix = NAN);

// Lab 6
matrix lab6_fun(matrix, matrix = NAN, matrix = NAN);

matrix df6(double, matrix, matrix = NAN, matrix = NAN);

matrix fR6(matrix, matrix = NAN, matrix = NAN);
