#ifndef MATRIX3_H
#define MATRIX3_H

#include "vec3.h"

#include <ostream>

class Matrix3 {
public:
	Matrix3() : e{ 0,0,0,0,0,0,0,0,0 } {};
	Matrix3(double e1, double e2, double e3, double e4, double e5, double e6, double e7, double e8, double e9) {
		e[0] = e1, e[1] = e2, e[2] = e3, e[3] = e4, e[4] = e5, e[5] = e6, e[6] = e7, e[7] = e8, e[8] = e9;
	}
	Matrix3(const Vec3& v1, const Vec3& v2, const Vec3& v3) {
		e[0] = v1.e[0], e[1] = v1.e[1], e[2] = v1.e[2];
		e[3] = v2.e[0], e[4] = v2.e[1], e[5] = v2.e[2];
		e[6] = v3.e[0], e[7] = v3.e[1], e[8] = v3.e[2];
	}

	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	Matrix3& operator+=(const Matrix3& o) {
		e[0] += o.e[0];
		e[1] += o.e[1];
		e[2] += o.e[2];
		e[3] += o.e[3];
		e[4] += o.e[4];
		e[5] += o.e[5];
		e[6] += o.e[6];
		e[7] += o.e[7];
		e[8] += o.e[8];
		return *this;
	}

	Matrix3 operator-() const {
		return Matrix3(-e[0], -e[1], -e[2],
			-e[3], -e[4], -e[5],
			-e[6], -e[7], -e[8]);
	}

	Matrix3& operator*=(const double t) {
		e[0] *= t, e[1] *= t, e[2] *= t;
		e[3] *= t, e[4] *= t, e[5] *= t;
		e[6] *= t, e[7] *= t, e[8] *= t;
		return *this;
	}

	Matrix3& operator/=(const double t) {
		return *this *= 1 / t;
	}

public:
	double e[9];
};

// Utility

std::ostream& operator<<(std::ostream& os, const Matrix3& mat) {
	os << mat.e[0] << " " << mat.e[1] << " " << mat.e[2] << "\n"
		<< mat.e[3] << " " << mat.e[4] << " " << mat.e[5] << "\n"
		<< mat.e[6] << " " << mat.e[7] << " " << mat.e[8] << "\n";
	return os;
}

inline Matrix3 operator+(const Matrix3& o, const Matrix3& v) {
	return Matrix3{ o.e[0] + v.e[0], o.e[1] + v.e[1], o.e[2] + v.e[2],
					o.e[3] + v.e[3], o.e[4] + v.e[4], o.e[5] + v.e[5],
					o.e[6] + v.e[6], o.e[7] + v.e[7], o.e[8] + v.e[8] };
}

inline Matrix3 operator+(const Matrix3& o, double t) {
	return Matrix3{ o.e[0] + t, o.e[1] + t, o.e[2] + t,
					o.e[3] + t, o.e[4] + t, o.e[5] + t,
					o.e[6] + t, o.e[7] + t, o.e[8] + t };
}

inline Matrix3 operator-(const Matrix3& o, const Matrix3& v) {
	return Matrix3{ o.e[0] - v.e[0], o.e[1] - v.e[1], o.e[2] - v.e[2],
					o.e[3] - v.e[3], o.e[4] - v.e[4], o.e[5] - v.e[5],
					o.e[6] - v.e[6], o.e[7] - v.e[7], o.e[8] - v.e[8] };
}

inline Matrix3 operator-(const Matrix3& o, double t) {
	return Matrix3{ o.e[0] - t, o.e[1] - t, o.e[2] - t,
					o.e[3] - t, o.e[4] - t, o.e[5] - t,
					o.e[6] - t, o.e[7] - t, o.e[8] - t };
}

inline Matrix3 operator*(const Matrix3& o, double t) {
	return Matrix3{ o.e[0] * t, o.e[1] * t, o.e[2] * t,
					o.e[3] * t, o.e[4] * t, o.e[5] * t,
					o.e[6] * t, o.e[7] * t, o.e[8] * t };
}

inline Matrix3 operator*(double t, const Matrix3& o) {
	return Matrix3{ t * o.e[0], t * o.e[1], t * o.e[2],
					t * o.e[3], t * o.e[4], t * o.e[5],
					t * o.e[6], t * o.e[7], t * o.e[8] };
}

inline Matrix3 operator/(const Matrix3& mat, double t) {
	return (1 / t) * mat;
}

inline Vec3 Dot(const Matrix3& mat, const Vec3& vec) {
	double x = mat.e[0] * vec.e[0] + mat.e[1] * vec.e[1] + mat.e[2] * vec.e[2];
	double y = mat.e[3] * vec.e[0] + mat.e[4] * vec.e[1] + mat.e[5] * vec.e[2];
	double z = mat.e[6] * vec.e[0] + mat.e[7] * vec.e[1] + mat.e[8] * vec.e[2];
	return Vec3{ x,y,z };
}

inline double Determinant(const Matrix3& mat) {
	double det = 0.0;

	// Multipliers
	double a = mat.e[0], b = mat.e[1], c = mat.e[2];

	double e = mat.e[4], f = mat.e[5], h = mat.e[7], i = mat.e[8], d = mat.e[3], g = mat.e[6];

	auto det_a = a * (e * i - f * h);
	auto det_b = b * (d * i - f * g);
	auto det_c = c * (d * h - e * g);

	det = det_a - det_b + det_c;

	return det;
}

#endif
