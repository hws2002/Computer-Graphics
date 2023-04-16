#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement functions and add more fields as necessary

class Sphere : public Object3D {
public:
    Sphere() {
        // unit ball at the center
        this->center = Vector3f(0, 0, 0);
        this->radius = 1;
    }

    Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material) {
        // 
        this->center = center;
        this->radius = radius;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        //
        Vector3f l = center - r.getOrigin();
        float tp = Vector3f::dot(l, r.getDirection());
        if ( tp < 0 ) return false;
        float d2 = Vector3f::dot(l, l) - Vector3f::dot(tp,tp);
        float t2 = radius*radius - d2;
        if ( t2<0 ) return false;
        float t = tp - sqrt(t2);

        if ( t < tmin || t > h.getT()) return false;
        Vector3f normal = (r.pointAtParameter(t) - center).normalized();
        h.set(t, material, normal);
        return true;
    }

protected:
    Vector3f center;
    float radius;
};


#endif
