#include "utility.h"

#include "Color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"
#include "camera.h"
#include "material.h"
#include "stopwatch.h"

#include <iostream>

Color RayColor(const Ray& r, const Hittable& world, int depth) {
    HitRecord rec;
    
    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return Color(0, 0, 0);

    if (world.Hit(r, 0.001, infinity, rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.mat_ptr->Scatter(r, rec, attenuation, scattered))
            return attenuation * RayColor(scattered, world, depth - 1);
        return Color(0, 0, 0);
    }

    Vec3 unit_direction = UnitVector(r.Direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

void GenerateWorldWithTriangles(HittableList& world) {
    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
    shared_ptr<Metal> material_left = make_shared<Metal>(Color(0.8,0.8,0.8), 0.3);
    shared_ptr<Metal> material_right = make_shared<Metal>(Color(0.8,0.6,0.2), 1.0);

    world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<Triangle>(Point3(-1.0, 0.4, -1), Point3(-1.8, -0.3, -1), Point3(-0.8, -0.3, -1), material_left));
    world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

}

HittableList RandomScene() {
    HittableList world;

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = RandomDouble();
            Point3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

            if ((center - Point3(4, 0.2, 0)).Length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::Random() * Color::Random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::Random(0.5, 1);
                    auto fuzz = RandomDouble(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

void Render(int image_width, int image_height, int samples_per_pixel, int max_depth, const HittableList& world, const Camera& cam) {
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_Color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + RandomDouble()) / (image_width - 1);
                auto v = (j + RandomDouble()) / (image_height - 1);
                Ray r = cam.GetRay(u, v);
                pixel_Color += RayColor(r, world, max_depth);
            }
            WriteColor(std::cout, pixel_Color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}

int main() {

    // Image

    const auto kAspectRatio = 3.0 / 2.0;
    const int kImgWidth = 400;
    const int kImgHeight = static_cast<int>(kImgWidth / kAspectRatio);
    const int kSamplesPerPixel = 10;
    const int kMaxDepth = 10;

    // World

    //HittableList world = RandomScene();
    HittableList world;

    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Dielectric>(1.5);
    auto material_left = make_shared<Dielectric>(1.5);
    auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Triangle>(Point3(-3.0, 0.5, -1.0), Point3(-4.0, -0.3, -1.0), Point3(-2.0, -0.3, -1.0), material_left));
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.4, material_left));
    world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));
    
    // Camera

    //Point3 lookfrom(6, 1, 2);
    Point3 lookfrom(4, 1, 10);
    Point3 lookat(0, 0, 0);
    Vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, kAspectRatio, aperture, dist_to_focus);

    // Benchmark
    StopWatch stop_watch;
    stop_watch.Begin();

    // Render

    Render(kImgWidth, kImgHeight, kSamplesPerPixel, kMaxDepth, world, cam);

    double dur = stop_watch.Stop();
    std::cout << "Render duration: " << dur << "s" << std::endl;
}