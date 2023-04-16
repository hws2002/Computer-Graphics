#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement Plane representing an infinite plane
// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions

class Plane : public Object3D {
public:
    Plane() {

    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
        this->normal = normal;
        this->d = d;
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        //TODO : 
        Vector3f P_o = r.getOrigin();
        Vector3f P_dir = r.getDirection();


        // 平行
        float cos = Vector3f::dot(normal, P_dir);
        if (fabs(cos) < 1e-6) return false;

        float t = (d - Vector3f::dot(normal, P_o)) / Vector3f::dot(normal, P_dir);
        if (t < tmin || t > h.getT()) return false;
        h.set(t, material, normal);
        return true;
        return false;
    }

protected:
    Vector3f normal;
    float d;
};

#endif //PLANE_H
		

