#ifndef MATERIAL_H
#define MATERIAL_H

#include "utility.h"
//#include "hittable.h"

struct HitRecord;

class Material {
public:
    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
    Lambertian(const Color& a) : albedo(a) {}

    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        auto scatter_direction = rec.normal + RandomUnitVectorInSphere();
        // Catch degenerate scatter direction
        if (scatter_direction.NearZero())
            scatter_direction = rec.normal;
        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

public:
    Color albedo;
};

class Metal : public Material {
public:
    Metal(const Color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        Vec3 reflected = Reflect(UnitVector(r_in.Direction()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz * RandomInUnitSphere());
        attenuation = albedo;
        return (Dot(scattered.Direction(), rec.normal) > 0);
    }

public:
    Color albedo;
    double fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered
    ) const override {
        attenuation = Color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        Vec3 unit_direction = UnitVector(r_in.Direction());
        double cos_theta = fmin(Dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vec3 direction;

        if (cannot_refract || Reflectance(cos_theta, refraction_ratio) > RandomDouble())
            direction = Reflect(unit_direction, rec.normal);
        else
            direction = Refract(unit_direction, rec.normal, refraction_ratio);

        scattered = Ray(rec.p, direction);
        return true;
    }

public:
    double ir; // Index of Refraction
private:
    static double Reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif