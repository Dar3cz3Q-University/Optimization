#pragma once

#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>
#include <string>
#include <random>
#include <filesystem>
#include <ctime>
#include <unordered_map>

#ifdef _WIN32
    #define _USE_MATH_DEFINES
    #include <math.h>
#else
    #include <cmath>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
