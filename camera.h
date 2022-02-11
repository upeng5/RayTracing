#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"

class Camera {
public:
    Camera(Point3 lookfrom, Point3 lookat, Vec3 vup ,double vfov, double aspect_ratio,double aperture,double focus_dist) {
        auto theta = DegreesToRadians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = UnitVector(lookfrom - lookat);
        u = UnitVector(Cross(vup, w));
        v = Cross(w, u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

        lens_radius = aperture / 2;
    }

    Ray GetRay(double s, double t) const {
        Vec3 rd = lens_radius * RandomInUnitDisk();
        Vec3 offset = u * rd.x() + v * rd.y();

        return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }

private:
    Point3 origin;
    Point3 lower_left_corner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    double lens_radius;
};
#endif