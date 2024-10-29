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

#include "matrix.h"
#include "user_funs.h"

matrix *solve_ode(matrix (*)(double, matrix, matrix, matrix), double, double, double, matrix, matrix = NAN, matrix = NAN); // throw (string);
