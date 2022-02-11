#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "matrix3.h"

class Triangle : public Hittable {
	public:
		Triangle(const Point3& a, const Point3& b, const Point3& c, shared_ptr<Material> m) {
			a_ = a;
			b_ = b;
			c_ = c;
			mat_ptr_ = m;
		}

		Point3 a() const { return a_; }
		Point3 b() const { return b_; }
		Point3 c() const { return c_; }

		bool Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

	public:
		Point3 a_;
		Point3 b_;
		Point3 c_;
		shared_ptr<Material> mat_ptr_;
		TYPE type_ = TYPE::TRIANGLE;
};


bool Triangle::Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
	bool hit = false;
	double beta = 0.0, gamma = 0.0, t = 0.0;
	Point3 e = r.Origin();
	Point3 d = r.Direction();
	Point3 intersection;

	double xa_xe = a_.x() - e.x(), xa_xc = a_.x() - c_.x(), xd = d.x();
	double ya_ye = a_.y() - e.y(), ya_yc = a_.y() - c_.y(), yd = d.y();
	double za_ze = a_.z() - e.z(), za_zc = a_.z() - c_.z(), zd = d.z();

	double xa_xb = a_.x() - b_.x();
	double ya_yb = a_.y() - b_.y();
	double za_zb = a_.z() - b_.z();


	Matrix3 beta_mat{ xa_xe, xa_xc, xd,
					  ya_ye, ya_yc, yd,
					  za_ze, za_zc, zd
	};

	Matrix3 gamma_mat{ xa_xb, xa_xe, xd,
					   ya_yb, ya_ye, yd,
					   za_zb, za_ze, zd
	};

	Matrix3 t_mat{ xa_xb, xa_xc, xa_xe,
				   ya_yb, ya_yc, ya_ye,
				   za_zb, za_zc, za_ze
	};


	Matrix3 A{ a_.x() - b_.x(), a_.x() - c_.x(), d.x(),
			   a_.y() - b_.y(), a_.y() - c_.y(), d.y(),
			   a_.z() - b_.z(), a_.z() - c_.z(), d.z() };

	// Solve for beta, gamma and t

	double det_a = Determinant(A);

	beta = Determinant(beta_mat) / det_a;
	gamma = Determinant(gamma_mat) / det_a;
	t = Determinant(t_mat) / det_a;

	if (beta > 0 && gamma > 0 && (beta + gamma) < 1) {
		intersection = a_ + beta * (b_ - a_) + gamma * (c_ - a_);
		hit = true;

		// Compute triangle normal
		Vec3 U = b_ - a_, V = c_ - a_;

		auto nx = (U.y() * V.z()) - (U.z() * V.y());
		auto ny = (U.z() * V.x()) - (U.x() * V.z());
		auto nz = (U.x() * V.y()) - (U.y() * V.x());

		Vec3 N = Vec3(nx,ny,nz);
		
		rec.t = t;
		rec.p = r.At(rec.t);
		rec.normal = N;
		rec.mat_ptr = mat_ptr_;

	}

	return hit;
}

#endif // !TRIANGLE_H

