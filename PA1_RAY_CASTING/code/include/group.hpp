#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>


// TODO: Implement Group - add data structure to store a list of Object*
class Group : public Object3D {

public:

    Group() {
        objects = std::vector<Object3D*>();
    }

    explicit Group (int num_objects) {
        objects = std::vector<Object3D*>(num_objects);
    }

    ~Group() override {
        for (auto &object : objects) {
            delete object;
        }
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool intersected = false; // tmin is 0
        for (auto obj : objects)
            intersected |= obj->intersect(r, h, tmin); // tmin remains same always
        return intersected;
    }

    void addObject(int index, Object3D *obj) {
        objects[index] = obj;
    }

    int getGroupSize() {
        return objects.size();
    }

private:
    std::vector<Object3D*> objects;
};

#endif //GROUP_H