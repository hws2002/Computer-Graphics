#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
using namespace std;

// TODO: implement this class and add more fields as necessary,
class Triangle: public Object3D {

public:
	Triangle() = delete;

    // a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m) : Object3D(m) {
		this->vertices[0] = a;
		this->vertices[1] = b;
		this->vertices[2] = c;
		this->normal = Vector3f::cross(b - a, c - a).normalized();
	}

	bool intersect( const Ray& ray,  Hit& hit , float tmin) override {
        Vector3f rayOrigin = ray.getOrigin();
		Vector3f rayDir = ray.getDirection().normalized();

        // 特殊情况 ： 平行
        float cos = Vector3f::dot(normal, rayDir);
        if (fabs(cos)<1e-6) return false;
        // Check if the ray intersects the plane
        float D = Vector3f::dot(normal, vertices[0]);
        float t = (D - Vector3f::dot(normal, rayOrigin)) / Vector3f::dot(normal, rayDir);
        if (t < tmin || t > hit.getT()) return false;

		// Check if the intersection point is inside the triangle
        Vector3f p(rayOrigin + rayDir * t);
        if (!inTriangle(p)) return false;
        hit.set(t, material, normal);
        return true;
	}
	Vector3f normal;
	Vector3f vertices[3];
protected:
    bool inTriangle(const Vector3f& p) {
        return Vector3f::dot(Vector3f::cross((vertices[0] - p), (vertices[1] - p)), normal) >= -1e-6 &&
               Vector3f::dot(Vector3f::cross((vertices[1] - p), (vertices[2] - p)), normal) >= -1e-6 &&
               Vector3f::dot(Vector3f::cross((vertices[2] - p), (vertices[0] - p)), normal) >= -1e-6;
    }
};

#endif //TRIANGLE_H
