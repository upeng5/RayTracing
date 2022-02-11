#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
	public:
		Ray() : orig(0.0, 0.0, 0.0), dir(0.0, 0.0, 0.0) {}
		Ray(Point3 origin, Vec3 direction) : orig{ origin }, dir{direction} {}
		Point3 Origin() const { return orig; }
		Vec3 Direction() const { return dir; }
		Point3 At(double t) const { return orig + t * dir; }
	public:
		Point3 orig;
		Vec3 dir;
};

#endif