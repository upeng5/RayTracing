#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(Point3 cen, double r, shared_ptr<Material> m) : center_(cen), radius_(r), mat_ptr_(m) {};

    virtual bool Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

public:
    Point3 center_;
    double radius_;
    shared_ptr<Material> mat_ptr_;
    TYPE type_ = TYPE::SPHERE;
};

bool Sphere::Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    Vec3 oc = r.Origin() - center_;
    auto a = r.Direction().LengthSquared();
    auto half_b = Dot(oc, r.Direction());
    auto c = oc.LengthSquared() - radius_ * radius_;

    auto discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.At(rec.t);
    Vec3 outward_normal = (rec.p - center_) / radius_;
    rec.SetFaceNormal(r, outward_normal);
    rec.mat_ptr = mat_ptr_;

    return true;
}

#endif