#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <glut.h>

// TODO (PA2): Copy from PA1

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
        Vector3f rayOrigin = r.getOrigin();
        Vector3f rayDir = r.getDirection().normalized();
        Vector3f l = center - rayOrigin;
        float tp = Vector3f::dot(l, rayDir);
        if ( tp < 0 ) return false;
        float d2 = Vector3f::dot(l, l) - tp*tp;
        float t2 = radius*radius - d2;
        if ( t2<0 ) return false;
        float t = tp - sqrt(t2);

        if ( t < tmin || t > h.getT()) return false;
        Vector3f normal = (rayOrigin + rayDir * t - center).normalized();
        h.set(t, material, normal);
        return true;
    }

    void drawGL() override {
        Object3D::drawGL();
        glMatrixMode(GL_MODELVIEW); glPushMatrix();
        glTranslatef(center.x(), center.y(), center.z());
        glutSolidSphere(radius, 80, 80);
        glPopMatrix();
    }

protected:
    Vector3f center;
    float radius;

};


#endif
