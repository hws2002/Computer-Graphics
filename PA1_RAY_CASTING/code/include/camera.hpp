#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera {
public:
    Camera(const Vector3f &center, const Vector3f &direction, const Vector3f &up, int imgW, int imgH) {
        this->center = center;
        this->direction = direction.normalized();
        this->horizontal = Vector3f::cross(this->direction, up).normalized();
        this->up = Vector3f::cross(this->horizontal, this->direction); // norm of up would be 1 as direction and horizontal are already normalized
        this->width = imgW;
        this->height = imgH;
    }

    // Generate rays for each screen-space coordinate
    virtual Ray generateRay(const Vector2f &point) = 0;
    virtual ~Camera() = default;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

protected:
    // Extrinsic parameters
    Vector3f center;
    Vector3f direction;
    Vector3f up;
    Vector3f horizontal;
    // Intrinsic parameters
    int width;
    int height;
};

// TODO: Implement Perspective camera
// You can add new functions or variables whenever needed.
class PerspectiveCamera : public Camera {

public:
    PerspectiveCamera(const Vector3f &center, const Vector3f &direction,
            const Vector3f &up, int imgW, int imgH, float angle) : Camera(center, direction, up, imgW, imgH) {
        // angle is in radian. (input is degree, converted to radian in scene_parser.cpp)
        this->angle = angle;
        this->aspect = (float)width / (float)height; 
        this->fx = imgW / (2.0f*(aspect * tan(angle / 2.0f)));
        this->fy = imgH / ((2.0f*tan(angle / 2.0f)));
    }

    Ray generateRay(const Vector2f &point) override {
        float Camera_x = (point.x() - width / 2.0f) * fx;
        float Camera_y = (point.y() - height / 2.0f) * fy;
        Vector3f rayDirOnCamera = Vector3f(Camera_x, Camera_y, fy).normalized(); // direction of ray in Camera space
        Matrix3f world2camera(horizontal,up,direction);

        Vector3f rayDir = (world2camera.inverse() * rayDirOnCamera).normalized(); // direction of ray in world space
        Vector3f rayOrigin = center; // Origin in world space , (0,0,10) for e.g
        return Ray(rayOrigin,rayDir);
    }

private:
    float angle; // in Radian
    float aspect; //Image Aspect Ratio, always 1 in PA1
    float fx;
    float fy;
};

#endif //CAMERA_H
