#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double Clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline double RandomDouble() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * RandomDouble();
}

inline double DegreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

// Common Headers

#include "Ray.h"
#include "Vec3.h"
#include "Matrix3.h"

// Triangle Specific Function
Vec3 RandomInUnitPlane() {
    return Vec3();
}


#endif