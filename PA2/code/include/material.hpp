#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <glut.h>

// TODO (PA2): Copy from PA1.
class Material {
public:

    explicit Material(const Vector3f &d_color, const Vector3f &s_color = Vector3f::ZERO, float s = 0) :
            diffuseColor(d_color), specularColor(s_color), shininess(s) {

    }

    virtual ~Material() = default;

    virtual Vector3f getDiffuseColor() const {
        return diffuseColor;
    }


    Vector3f Shade(const Ray &ray, const Hit &hit,
                   const Vector3f &dirToLight, const Vector3f &lightColor) {
        Vector3f shaded = Vector3f::ZERO;

        Vector3f N = hit.getNormal().normalized();
        Vector3f intersection = ray.pointAtParameter(hit.getT());
        Vector3f L = dirToLight.normalized();
        Vector3f V = -ray.getDirection().normalized();
        Vector3f R = (2 * Vector3f::dot(L, N) * N - L).normalized();
        shaded = lightColor * 
                (diffuseColor * clamp(Vector3f::dot(L, N)) +
                 specularColor * pow(clamp(Vector3f::dot(R, V)), shininess));
        return shaded;
        return shaded;
    }

    // For OpenGL, this is fully implemented
    void Use() {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Vector4f(diffuseColor, 1.0f));
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Vector4f(specularColor, 1.0f));
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, Vector2f(shininess * 4.0, 1.0f));
    }

protected:
    Vector3f diffuseColor;
    Vector3f specularColor;
    float shininess;
    float clamp(float x) { return std::max((float)0, x); }
};


#endif // MATERIAL_H
