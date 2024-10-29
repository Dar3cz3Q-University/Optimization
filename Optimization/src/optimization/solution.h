/*********************************************
Kod stanowi uzupe�nienie materia��w do �wicze�
w ramach przedmiotu metody optymalizacji.
Kod udost�pniony na licencji CC BY-SA 3.0
Autor: dr in�. �ukasz Sztangret
Katedra Informatyki Stosowanej i Modelowania
Akademia G�rniczo-Hutnicza
Data ostatniej modyfikacji: 19.09.2023
*********************************************/

// Ten plik nie powinien by� edytowany

#pragma once

#include "ode_solver.h"

class solution {
public:
    matrix x;
    matrix y;
    matrix g;
    matrix H;
    matrix ud;
    int flag;
    static int f_calls;
    static int g_calls;
    static int H_calls;
    static void clear_calls();
    solution(double = NAN);
    solution(const matrix &);
    solution(int, double *); // throw (string);
    solution(const solution &);
    solution &operator=(const solution &);
    matrix fit_fun(matrix (*)(matrix, matrix, matrix), matrix = NAN, matrix = NAN); // throw (string);
    matrix grad(matrix (*)(matrix, matrix, matrix), matrix = NAN, matrix = NAN);    // throw (string);
    matrix hess(matrix (*)(matrix, matrix, matrix), matrix = NAN, matrix = NAN);    // throw (string);
};

int get_dim(const solution &); // throw (string);
ostream &operator<<(ostream &, const solution &);
