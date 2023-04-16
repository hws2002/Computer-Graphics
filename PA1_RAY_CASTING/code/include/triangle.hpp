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
		//TODO: 
        Vector3f o(ray.getOrigin()), dir(ray.getDirection());
        // dir.normalize();
        float cos = Vector3f::dot(normal, dir);
        // 平行
        if (fabs(cos)<1e-6) return false;
        // d = n.o + t*n.dir => t = (d-n.o)/(n.dir)
        float d = Vector3f::dot(normal, vertices[0]);
        float t = (d - Vector3f::dot(normal, o)) / Vector3f::dot(normal, dir);
        if (t < tmin || t > hit.getT()) return false;
        Vector3f p(o + dir * t);
        if (!inTriangle(p)) return false;
        hit.set(t, material, normal);
        return true;
        return false;
	}
	Vector3f normal;
	Vector3f vertices[3];
protected:
    bool inTriangle(const Vector3f& p) {
        return Vector3f::dot(Vector3f::cross((vertices[1] - p), (vertices[2] - p)), normal) >= -1e-6 &&
               Vector3f::dot(Vector3f::cross((vertices[2] - p), (vertices[0] - p)), normal) >= -1e-6 &&
               Vector3f::dot(Vector3f::cross((vertices[0] - p), (vertices[1] - p)), normal) >= -1e-6;
    }
};

#endif //TRIANGLE_H
